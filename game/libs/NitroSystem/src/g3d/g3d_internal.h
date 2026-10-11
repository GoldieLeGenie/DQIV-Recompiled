#ifndef G3D_INTERNAL_H
#define G3D_INTERNAL_H

#include <nitro/types.h>
#include <nitro/reg.h>

typedef s32 fx32;
typedef s16 fx16;
typedef s64 fx64;

#define FX32_ONE 0x1000

typedef struct { fx32 x, y, z; } VecFx32;
typedef struct { fx32 _00, _01, _02, _10, _11, _12, _20, _21, _22; } MtxFx33;
typedef struct { fx32 _00, _01, _02, _10, _11, _12, _20, _21, _22, _30, _31, _32; } MtxFx43;
typedef struct { fx32 _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; } MtxFx44;

/* resource structures */
typedef struct NNSG3dResDict {
    u8 revision;     // 0x00
    u8 numEntry;     // 0x01
    u16 sizeDictBlk; // 0x02
    u16 unk_04;      // 0x04
    u16 ofsEntry;    // 0x06
} NNSG3dResDict;

typedef struct UnkResDictTreeNode {
    u8 refBit;   // 0x00
    u8 idx[2];   // 0x01 (left, right)
    u8 idxEntry; // 0x03
} UnkResDictTreeNode;

typedef struct UnkResDictEntryHeader {
    u16 sizeUnit; // 0x00
    u16 ofsName;  // 0x02
    u8 data[4];   // 0x04
} UnkResDictEntryHeader;

typedef struct NNSG3dResName {
    u32 val[4];
} NNSG3dResName;

static inline void* GetResDataByIdx(const NNSG3dResDict* dict, u32 idx) {
    UnkResDictEntryHeader* p = (UnkResDictEntryHeader*)((u8*)dict + dict->ofsEntry);
    return (void*)((u8*)&p->data[0] + p->sizeUnit * idx);
}

typedef struct NNSG3dResMdlInfo {
    u8 sbcType;      // 0x00
    u8 scalingRule;  // 0x01
    u8 texMtxMode;   // 0x02
    u8 numNode;      // 0x03
    u8 numMat;       // 0x04
    u8 numShp;       // 0x05
    u8 unk_06;       // 0x06
    u8 unk_07;       // 0x07
    fx32 posScale;   // 0x08
    fx32 invPosScale; // 0x0C
    u8 unk_10[0x2c - 0x10];
} NNSG3dResMdlInfo;

typedef struct NNSG3dResMdl {
    u32 size;           // 0x00
    u32 ofsSbc;         // 0x04
    u32 ofsMat;         // 0x08
    u32 ofsShp;         // 0x0C
    u32 ofsEvpMtx;      // 0x10
    NNSG3dResMdlInfo info; // 0x14
    NNSG3dResDict nodeInfo; // 0x40
} NNSG3dResMdl;

typedef struct NNSG3dResMat {
    u16 unk_00;         // 0x00
    u16 unk_02;         // 0x02
    NNSG3dResDict dict; // 0x04
} NNSG3dResMat;

typedef struct NNSG3dResMatData {
    u16 itemTag;      // 0x00
    u16 size;         // 0x02
    u32 diffAmb;      // 0x04
    u32 specEmi;      // 0x08
    u32 polyAttr;     // 0x0C
    u32 polyAttrMask; // 0x10
    u32 texImageParam; // 0x14
    u32 unk_18;       // 0x18
    u16 texPlttBase;  // 0x1C
    u16 flag;         // 0x1E
    u16 origWidth;    // 0x20
    u16 origHeight;   // 0x22
    fx32 magW;        // 0x24
    fx32 magH;        // 0x28
    u8 texMtx[4];     // 0x2C
} NNSG3dResMatData;

typedef struct NNSG3dResShp {
    NNSG3dResDict dict; // 0x00
} NNSG3dResShp;

typedef struct NNSG3dResShpData {
    u16 itemTag; // 0x00
    u16 size;    // 0x02
    u32 flag;    // 0x04
    u32 ofsDL;   // 0x08
    u32 sizeDL;  // 0x0C
} NNSG3dResShpData;

typedef struct UnkResNodeData {
    u16 flag; // 0x00
    s16 _00;  // 0x02
} UnkResNodeData;

/* animation results */
typedef struct NNSG3dMatAnmResult {
    u32 flag;          // 0x00
    u32 prmMatColor0;  // 0x04
    u32 prmMatColor1;  // 0x08
    u32 prmPolygonAttr; // 0x0C
    u32 prmTexImage;   // 0x10
    u32 prmTexPltt;    // 0x14
    fx32 scaleS;       // 0x18
    fx32 scaleT;       // 0x1C
    fx16 rotSin;       // 0x20
    fx16 rotCos;       // 0x22
    fx32 transS;       // 0x24
    fx32 transT;       // 0x28
    u16 origWidth;     // 0x2C
    u16 origHeight;    // 0x2E
    fx32 magW;         // 0x30
    fx32 magH;         // 0x34
} NNSG3dMatAnmResult;

typedef struct NNSG3dJntAnmResult {
    u32 flag;        // 0x00
    VecFx32 scale;   // 0x04
    VecFx32 scaleEx0; // 0x10
    VecFx32 scaleEx1; // 0x1C
    MtxFx33 rot;     // 0x28
    VecFx32 trans;   // 0x4C
} NNSG3dJntAnmResult;

typedef struct NNSG3dVisAnmResult {
    BOOL isVisible;
} NNSG3dVisAnmResult;

typedef struct NNSG3dAnmObj NNSG3dAnmObj;
typedef void (*UnkAnmFunc)(void* result, NNSG3dAnmObj* obj, u32 idx);
typedef BOOL (*UnkBlendFunc)(void* result, NNSG3dAnmObj* obj, u32 idx);

struct NNSG3dAnmObj {
    fx32 frame;            // 0x00
    fx32 ratio;            // 0x04
    void* resAnm;          // 0x08
    void* funcAnm;         // 0x0C
    NNSG3dAnmObj* next;    // 0x10
    void* resTex;          // 0x14
    u8 priority;           // 0x18
    u8 numMapData;         // 0x19
    u16 mapData[1];        // 0x1A
};

typedef struct NNSG3dRS NNSG3dRS;
typedef void (*UnkSbcCallback)(NNSG3dRS* rs);

typedef struct NNSG3dRenderObj {
    u32 flag;                    // 0x00
    NNSG3dResMdl* resMdl;        // 0x04
    NNSG3dAnmObj* anmMat;        // 0x08
    UnkBlendFunc funcBlendMat;   // 0x0C
    NNSG3dAnmObj* anmJnt;        // 0x10
    UnkBlendFunc funcBlendJnt;   // 0x14
    NNSG3dAnmObj* anmVis;        // 0x18
    UnkBlendFunc funcBlendVis;   // 0x1C
    UnkSbcCallback cbFunc;       // 0x20
    u8 cbCmd;                    // 0x24
    u8 cbTiming;                 // 0x25
    u16 unk_26;                  // 0x26
    UnkSbcCallback cbInitFunc;   // 0x28
    void* unk_2c;                // 0x2C
    u8* ptrUserSbc;              // 0x30
    NNSG3dJntAnmResult* recJntAnm; // 0x34
    NNSG3dMatAnmResult* recMatAnm; // 0x38
    u32 hintMatAnmExist[2];      // 0x3C
    u32 hintJntAnmExist[2];      // 0x44
    u32 hintVisAnmExist[2];      // 0x4C
} NNSG3dRenderObj;

typedef void (*UnkJntScaleFunc)(NNSG3dJntAnmResult* r, const void* p, const u8* cmd, u32 flag);
typedef void (*UnkJntMtxFunc)(const NNSG3dJntAnmResult* r);
typedef void (*UnkTexMtxFunc)(NNSG3dMatAnmResult* r);

struct NNSG3dRS {
    u8* c;                            // 0x000
    NNSG3dRenderObj* pRenderObj;      // 0x004
    u32 flag;                         // 0x008
    UnkSbcCallback cbVecFunc[32];     // 0x00C
    u8 cbVecTiming[32];               // 0x08C
    u8 currentNode;                   // 0x0AC
    u8 currentMat;                    // 0x0AD
    u8 currentNodeDesc;               // 0x0AE
    u8 unk_af;                        // 0x0AF
    NNSG3dMatAnmResult* pMatAnmResult; // 0x0B0
    NNSG3dJntAnmResult* pJntAnmResult; // 0x0B4
    NNSG3dVisAnmResult* pVisAnmResult; // 0x0B8
    u32 isMatCached[4];               // 0x0BC
    u32 isScaleCacheOne[2];           // 0x0CC
    NNSG3dResDict* pResNodeInfo;      // 0x0D4
    NNSG3dResMat* pResMat;            // 0x0D8
    NNSG3dResShp* pResShp;            // 0x0DC
    fx32 posScale;                    // 0x0E0
    fx32 invPosScale;                 // 0x0E4
    UnkJntScaleFunc funcJntScale;     // 0x0E8
    UnkJntMtxFunc funcJntMtx;         // 0x0EC
    UnkTexMtxFunc funcTexMtx;         // 0x0F0
    NNSG3dMatAnmResult tmpMatAnmResult; // 0x0F4
    NNSG3dJntAnmResult tmpJntAnmResult; // 0x12C
    NNSG3dVisAnmResult tmpVisAnmResult; // 0x184
};

typedef void (*UnkSbcFunc)(NNSG3dRS* rs, u32 opt);
typedef void (*UnkMatFunc)(NNSG3dRS* rs, u32 opt, const NNSG3dResMatData* mat, u32 idx);
typedef void (*UnkShpFunc)(NNSG3dRS* rs, u32 opt, const NNSG3dResShpData* shp, u32 idx);

typedef struct NNSG3dGlb {
    u32 unk_000[2];
    MtxFx44 projMtx;    // 0x008
    u32 unk_048;        // 0x048
    MtxFx43 cameraMtx;  // 0x04C
    u8 unk_07c[0x94 - 0x7c];
    u32 prmMatColor0;   // 0x094
    u32 prmMatColor1;   // 0x098
    u32 prmPolygonAttr; // 0x09C
    u8 unk_0a0[0xbc - 0xa0];
    MtxFx33 prmBaseRot; // 0x0BC
    VecFx32 unk_0e0;    // 0x0E0
    u8 unk_0ec[0xfc - 0xec];
    u32 flag;           // 0x0FC
} NNSG3dGlb;

typedef struct NNSG3dGeBuffer {
    u32 idx;        // 0x000
    u32 data[0xc0]; // 0x004
} NNSG3dGeBuffer;

/* per-node scale cache entry */
typedef struct UnkScaleCache {
    VecFx32 s;   // 0x00
    VecFx32 inv; // 0x0C
} UnkScaleCache;

/* per-joint envelope matrix cache entry */
typedef struct UnkEvpCache {
    MtxFx44 m; // 0x00
    MtxFx33 n; // 0x40
} UnkEvpCache;

/* render state buffers (data_0210d190) */
typedef struct UnkG3dRSOnGlb {
    NNSG3dMatAnmResult matCache[64];   // 0x0000
    UnkScaleCache      scaleCache[64]; // 0x0E00
    UnkEvpCache        evpCache[64];   // 0x1400
} UnkG3dRSOnGlb;

typedef struct UnkGeState {
    NNSG3dGeBuffer* buf; // 0x00
    volatile u32 busy;   // 0x04
    u32 useDmaAsync;     // 0x08
} UnkGeState;

static inline NNSG3dResMatData* GetMatDataByIdx(const NNSG3dResMat* mat, u32 idx) {
    const u32* p = (const u32*)GetResDataByIdx(&mat->dict, idx);
    return (NNSG3dResMatData*)((u8*)mat + *p);
}

static inline NNSG3dResShpData* GetShpDataByIdx(const NNSG3dResShp* shp, u32 idx) {
    const u32* p = (const u32*)GetResDataByIdx(&shp->dict, idx);
    return (NNSG3dResShpData*)((u8*)shp + *p);
}

static inline UnkResNodeData* GetNodeDataByIdx(const NNSG3dResDict* info, u32 idx) {
    const u32* p = (const u32*)GetResDataByIdx(info, idx);
    return (UnkResNodeData*)((u8*)info + *p);
}

/* callback helpers (inlined) */
static inline u32 CheckCallBack(NNSG3dRS* rs, u32 cmd) {
    return rs->cbVecFunc[cmd] ? rs->cbVecTiming[cmd] : 0;
}

static inline BOOL CallBackCheck_A(NNSG3dRS* rs, u32 cmd, u32* pTiming) {
    *pTiming = CheckCallBack(rs, cmd);
    if (*pTiming == 1) {
        rs->flag &= ~0x40;
        rs->cbVecFunc[cmd](rs);
        *pTiming = CheckCallBack(rs, cmd);
        return rs->flag & 0x40;
    }
    return FALSE;
}

static inline BOOL CallBackCheck_B(NNSG3dRS* rs, u32 cmd, u32* pTiming) {
    if (*pTiming == 2) {
        rs->flag &= ~0x40;
        rs->cbVecFunc[cmd](rs);
        *pTiming = CheckCallBack(rs, cmd);
        return rs->flag & 0x40;
    }
    return FALSE;
}

static inline BOOL CallBackCheck_C(NNSG3dRS* rs, u32 cmd, u32 timing) {
    if (timing == 3) {
        rs->flag &= ~0x40;
        rs->cbVecFunc[cmd](rs);
        return rs->flag & 0x40;
    }
    return FALSE;
}

/* external functions */
void MI_CpuFill(u32 value, void* dest, u32 size);
void MI_CpuFillU16(u16 value, void* dest, u32 size);
void MI_CpuFillFromSrc(const void* src, volatile void* dest, u32 size);
void MI_CpuCopy(const void* src, void* dest, u32 size);
void MI_CpuClear24(void* dest);
void func_0206785c(u32 value, void* dest, u32 size);
void func_02067540(u32 dmaNo, const void* src, u32 size, void (*cb)(void*), void* arg);
void func_02067744(u32 dmaNo, const void* src, u32 size, void (*cb)(void*), void* arg);
void func_02061f80(const MtxFx43* src, MtxFx44* dst);
void func_020627a0(const MtxFx44* a, const MtxFx44* b, MtxFx44* ab);
fx32 func_0206308c(const VecFx32* v);
void func_020630ec(const VecFx32* src, VecFx32* dst);
void func_020626a0(const VecFx32* v, const MtxFx43* m, VecFx32* dst);
void func_0206276c(const MtxFx44* src, MtxFx43* dst);
fx32 FX_Divide(fx32 a, fx32 b);
void FX_InvAsync(fx32 d);
fx64 FX_GetDivResultFx64c(void);
void func_02065034(const MtxFx43* m);
void func_02065050(const MtxFx43* m);
void func_0206506c(const MtxFx33* m);
void func_02065088(void);
BOOL func_020653a8(MtxFx44* m);
BOOL func_020653d8(MtxFx33* m);
void func_0206ac78(void);
const MtxFx43* func_0206aed4(void);
const MtxFx43* func_0206b1bc(void);
const MtxFx43* func_0206b1f4(void);
const MtxFx44* func_0206b22c(void);
void func_0206b294(int* x1, int* y1, int* x2, int* y2);
u32 func_0206a26c(const void* anm, const NNSG3dResMdl* mdl);
void* func_02068a1c(void* allocator, u32 size);
void func_02068a30(void* allocator, void* p);

/* functions of this range */
void G3d_RenderSBCCommands(NNSG3dRS* rs);
void func_0206b768(NNSG3dRS* rs, NNSG3dRenderObj* obj);
void func_0206b8e4(u32* hint, const NNSG3dAnmObj* anm);
void G3d_Render(NNSG3dRenderObj* obj);
void func_0206dcf0(void);
void func_0206dd4c(void);
void func_0206dd64(void* arg);
void func_0206dd80(const u32* dl, u32 size);
void func_0206de34(u32 op, const void* args, u32 num);
void func_0206df18(MtxFx43* m, MtxFx33* n);

/* data */
extern UnkSbcFunc data_020c3f5c[];   // SBC command table
extern NNSG3dRS* data_0210d18c;      // current render state
extern UnkG3dRSOnGlb data_0210d190;  // render state buffers
extern NNSG3dGlb data_0210cf28;
extern UnkGeState data_0210fe90;
extern u32 data_020c3dbc;

#endif
