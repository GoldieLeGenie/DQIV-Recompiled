#include "g3d_anm.h"

/* texture pattern animation (nsbtp) */
typedef struct {
    u8 category0;                   /* 0x00 */
    u8 revision;                    /* 0x01 */
    u16 category1;                  /* 0x02 */
    u16 numFrame;                   /* 0x04 */
    u8 numTex;                      /* 0x06 */
    u8 numPltt;                     /* 0x07 */
    u16 ofsTexName;                 /* 0x08 */
    u16 ofsPlttName;                /* 0x0A */
    UnkResDict dict;                /* 0x0C */
} UnkResTexPatAnm;

typedef struct {
    u16 idxFrame;                   /* 0x00 */
    u8 idTex;                       /* 0x02 */
    u8 idPltt;                      /* 0x03 */
} UnkTexPatAnmFV;

typedef struct {
    u32 vramKey;                    /* 0x00 */
    u16 sizeTex;                    /* 0x04 */
    u16 ofsDict;                    /* 0x06 */
    u16 flag;                       /* 0x08 */
    u16 dummy_;                     /* 0x0A */
    u32 ofsTex;                     /* 0x0C */
} UnkResTexInfo;

typedef struct {
    u32 vramKey;                    /* 0x00 */
    u16 sizeTex;                    /* 0x04 */
    u16 ofsDict;                    /* 0x06 */
    u16 flag;                       /* 0x08 */
    u16 dummy_;                     /* 0x0A */
    u32 ofsTex;                     /* 0x0C */
    u32 ofsTexPlttIdx;              /* 0x10 */
} UnkResTex4x4Info;

typedef struct {
    u32 vramKey;                    /* 0x00 */
    u16 sizePltt;                   /* 0x04 */
    u16 flag;                       /* 0x06 */
    u16 ofsDict;                    /* 0x08 */
    u16 dummy_;                     /* 0x0A */
    u32 ofsPlttData;                /* 0x0C */
} UnkResPlttInfo;

typedef struct {
    u32 kind;                       /* 0x00 */
    u32 size;                       /* 0x04 */
    UnkResTexInfo texInfo;          /* 0x08 */
    UnkResTex4x4Info tex4x4Info;    /* 0x18 */
    UnkResPlttInfo plttInfo;        /* 0x2C */
    UnkResDict dict;                /* 0x3C */
} UnkResTex;

typedef struct {
    u32 texImageParam;              /* 0x00 */
    u32 extraParam;                 /* 0x04 */
} UnkResDictTexData;

typedef struct {
    u16 offset;                     /* 0x00 */
    u16 flag;                       /* 0x02 */
} UnkResDictPlttData;

extern UnkAnmFunc data_020c3dd0;

const UnkTexPatAnmFV* func_0206e968(const UnkResTexPatAnm* anm, u32 idx, u32 frame);
const UnkResName* func_0206e948(const UnkResTexPatAnm* anm, u32 idx);
const UnkResName* func_0206e958(const UnkResTexPatAnm* anm, u32 idx);

void func_020706f0(UnkAnmObj* obj, const UnkResTexPatAnm* anm, const UnkResMdl* mdl)
{
    u32 i;
    const UnkResMat* mat = (const UnkResMat*)((const u8*)mdl + mdl->ofsMat);

    obj->funcAnm = data_020c3dd0;
    obj->numMapData = mdl->info.numMat;
    obj->resAnm = (void*)anm;
    MI_CpuFillU16(0, obj->mapData, obj->numMapData * sizeof(u16));

    for (i = 0; i < anm->dict.numEntry; i++) {
        int idx = func_0206e7a0(&mat->dict, UnkGetResNameByIdx_(&anm->dict, i));
        if (idx >= 0) {
            obj->mapData[idx] = (u16)(i | 0x100);
        }
    }
}

/* sets the texture of a texture pattern frame */
void func_02070790(const UnkResTex* tex, const UnkResName* name, UnkMatAnmResult* result)
{
    const UnkResDictTexData* data = func_0206e664(&tex->dict, name);
    u32 vramAddr;
    u32 w, h;

    if ((data->texImageParam & 0x1c000000) != 0x14000000) {
        vramAddr = (u16)tex->texInfo.vramKey;
    } else {
        vramAddr = (u16)tex->tex4x4Info.vramKey;
    }

    result->prmTexImage &= 0xc00f0000;
    result->prmTexImage |= data->texImageParam + vramAddr;
    result->origW = data->extraParam & 0x7ff;
    result->origH = (data->extraParam & (0x7ff << 11)) >> 11;

    w = data->extraParam & 0x7ff;
    h = (data->extraParam >> 11) & 0x7ff;
    result->magW = (w == result->origW) ? 0x1000 : FX_Divide(w << 12, result->origW << 12);
    result->magH = (h == result->origH) ? 0x1000 : FX_Divide(h << 12, result->origH << 12);
}

/* sets the palette of a texture pattern frame */
void func_02070858(const UnkResTex* tex, const UnkResName* name, UnkMatAnmResult* result)
{
    const UnkResDictPlttData* data = func_0206e664((const UnkResDict*)((const u8*)tex + tex->plttInfo.ofsDict), name);
    u16 base = (u16)tex->plttInfo.vramKey;
    u16 offset = data->offset;

    if (!(data->flag & 1)) {
        offset = (u16)(offset >> 1);
        base = (u16)(base >> 1);
    }
    result->prmTexPltt = offset + base;
}

void func_020708a8(UnkMatAnmResult* result, const UnkAnmObj* obj, u32 dataIdx)
{
    const UnkResTexPatAnm* anm = obj->resAnm;
    const UnkTexPatAnmFV* fv = func_0206e968(anm, (u16)dataIdx, (u16)(obj->frame >> 12));

    func_02070790(obj->resTex, func_0206e948(anm, fv->idTex), result);
    if (fv->idPltt != 0xff) {
        func_02070858(obj->resTex, func_0206e958(anm, fv->idPltt), result);
    }
}
