#include "g3d_anm.h"

/* visibility animation (nsbva) */
typedef struct {
    u8 category0;                   /* 0x00 */
    u8 revision;                    /* 0x01 */
    u16 category1;                  /* 0x02 */
    u16 numFrame;                   /* 0x04 */
    u16 numNode;                    /* 0x06 */
    u16 flag;                       /* 0x08 */
    u16 dummy_;                     /* 0x0A */
    u32 visData[1];                 /* 0x0C */
} UnkResVisAnm;

typedef struct {
    BOOL isVisible;                 /* 0x00 */
} UnkVisAnmResult;

extern UnkAnmFunc data_020c3dc4;

void func_0207091c(UnkAnmObj* obj, const UnkResVisAnm* anm, const UnkResMdl* mdl)
{
    u32 i;

    obj->funcAnm = data_020c3dc4;
    obj->numMapData = mdl->info.numNode;
    obj->resAnm = (void*)anm;

    for (i = 0; i < obj->numMapData; i++) {
        obj->mapData[i] = (u16)(i | 0x100);
    }
}

void func_02070968(UnkVisAnmResult* result, const UnkAnmObj* obj, u32 dataIdx)
{
    const UnkResVisAnm* anm = obj->resAnm;
    u32 pos = (obj->frame >> 12) * anm->numNode + dataIdx;

    result->isVisible = anm->visData[pos >> 5] & (1 << (pos & 31));
}
