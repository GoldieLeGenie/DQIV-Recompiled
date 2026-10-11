#ifndef G3D_F_H
#define G3D_F_H

#include "../nns_internal.h"

typedef struct {
    fx32 m[4][4];
} UnkMtxFx44;

typedef struct {
    fx32 m[3][3];
} UnkMtxFx33;

/* plain fixed-point products */
#define UNK_MUL64(a, b)  ((fx32)(((s64)(a) * (b)) >> 12))
#define UNK_MUL64_8(a, b) ((fx32)(((s64)(a) * (b)) >> 8))

/* joint animation result */
typedef struct {
    u32 flag;                       /* 0x00 */
    UnkVecFx32 scale;               /* 0x04 */
    UnkVecFx32 scaleEx0;            /* 0x10 */
    UnkVecFx32 scaleEx1;            /* 0x1C */
    UnkMtxFx33 rot;                 /* 0x28 */
    UnkVecFx32 trans;               /* 0x4C */
} UnkJntAnmResult;

/* material animation result */
typedef struct {
    u32 flag;                       /* 0x00 */
    u32 prmMatColor0;               /* 0x04 */
    u32 prmMatColor1;               /* 0x08 */
    u32 prmPolygonAttr;             /* 0x0C */
    u32 prmTexImage;                /* 0x10 */
    u32 prmTexPltt;                 /* 0x14 */
    fx32 scaleS;                    /* 0x18 */
    fx32 scaleT;                    /* 0x1C */
    fx16 sinR;                      /* 0x20 */
    fx16 cosR;                      /* 0x22 */
    fx32 transS;                    /* 0x24 */
    fx32 transT;                    /* 0x28 */
    u16 origW;                      /* 0x2C */
    u16 origH;                      /* 0x2E */
    fx32 magW;                      /* 0x30 */
    fx32 magH;                      /* 0x34 */
} UnkMatAnmResult;

/* rendering state (pointer in data_0210d18c) */
typedef struct {
    u8 unk_00[0xc4];
    u32 isScaleCacheOne[8];         /* 0xC4 */
} UnkG3dRS;

/* per-node scale cache entry (data_0210d190.scaleCache) */
typedef struct {
    UnkVecFx32 s;                   /* 0x00 */
    UnkVecFx32 inv;                 /* 0x0C */
} UnkScaleCache;

/* command buffer sent by the texture SRT senders */
typedef struct {
    u32 cmd;                        /* 0x00 */
    u32 mtxMode;                    /* 0x04 */
    UnkMtxFx44 m;                   /* 0x08 */
    u32 mtxMode2;                   /* 0x48 */
} UnkTexMtxCmd;

typedef struct {
    fx32 m[4][3];
} UnkMtxFx43;

/* command buffer sent by the texture SRT sender (4x3 variant) */
typedef struct {
    u32 cmd;                        /* 0x00 */
    u32 mtxMode;                    /* 0x04 */
    UnkMtxFx43 m;                   /* 0x08 */
    u32 mtxMode2;                   /* 0x38 */
} UnkTexMtxCmd43;

typedef void (*UnkCalcTexMtxFunc)(UnkMtxFx44* m, const UnkMatAnmResult* anm);

extern UnkG3dRS* data_0210d18c;
/* render state buffers (data_0210d190) */
typedef struct {
    UnkMatAnmResult matCache[64];      /* 0x0000 */
    UnkScaleCache   scaleCache[64];    /* 0x0E00 */
    u8              evpCache[64][0x64]; /* 0x1400 */
} UnkG3dRSOnGlb;

extern UnkG3dRSOnGlb data_0210d190;

void func_0206de34(u32 op, const void* args, u32 num);

#endif
