#include "g3d_anm.h"

/* material color animation (nsbma) */
typedef struct {
    u32 tagDiffuse;                 /* 0x00 */
    u32 tagAmbient;                 /* 0x04 */
    u32 tagSpecular;                /* 0x08 */
    u32 tagEmission;                /* 0x0C */
    u32 tagPolygonAlpha;            /* 0x10 */
} UnkMatCAnmData;

extern UnkAnmFunc data_020c3dd4;

/* average of two RGB555 colors */
static inline u16 Blend2_(const u16* p)
{
    u32 c0 = p[0];
    u32 c1 = p[1];
    u32 g = ((c0 & 0x3e0) + (c1 & 0x3e0)) >> 1;
    u32 rb = 0x7c1f & (((c0 & 0x7c1f) + (c1 & 0x7c1f)) >> 1);
    return (u16)(rb | (g & 0x3e0));
}

/* RGB555 color of a material color animation track */
u16 func_0206ff6c(const UnkResAnm* anm, u32 info, u32 frame)
{
    const u16* data;
    u32 last;
    u32 idx;

    if (info & 0x20000000) {
        return (u16)info;
    }

    data = (const u16*)((const u8*)anm + (u16)info);
    if (!(info & 0xc0000000)) {
        return data[frame];
    }

    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last) {
                return *(data + (last >> 1) + 1);
            }
            idx = frame >> 1;
        } else {
            return data[frame >> 1];
        }
    } else {
        idx = frame & 3;
        if (idx != 0) {
            if (frame > last) {
                return (data + (last >> 2))[idx];
            }
            if (frame & 1) {
                u32 idx0, idx1;
                u32 c0, c1;
                u32 g, rb;

                if (frame & 2) {
                    idx1 = frame >> 2;
                    idx0 = idx1 + 1;
                } else {
                    idx0 = frame >> 2;
                    idx1 = idx0 + 1;
                }
                c0 = data[idx0];
                c1 = data[idx1];
                g = ((c0 & 0x3e0) + (c0 & 0x3e0) + (c0 & 0x3e0) + (c1 & 0x3e0)) >> 2;
                rb = 0x7c1f & (((c0 & 0x7c1f) + (c0 & 0x7c1f) + (c0 & 0x7c1f) + (c1 & 0x7c1f)) >> 2);
                return (u16)(rb | (g & 0x3e0));
            }
            idx = frame >> 2;
        } else {
            return data[frame >> 2];
        }
    }
    return Blend2_(&data[idx]);
}

/* polygon alpha of a material color animation track */
u16 func_020700d4(const UnkResAnm* anm, u32 info, u32 frame)
{
    const u8* data;
    u32 last;

    if (info & 0x20000000) {
        return (u16)info;
    }

    data = (const u8*)anm + (u16)info;
    if (!(info & 0xc0000000)) {
        return data[frame];
    }

    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last) {
                return *(data + (last >> 1) + 1);
            }
            return (u16)((data[frame >> 1] + *(data + (frame >> 1) + 1)) >> 1);
        }
        return data[frame >> 1];
    } else {
        u32 rem = frame & 3;
        if (rem != 0) {
            if (frame > last) {
                return (data + (last >> 2))[rem];
            }
            if (frame & 1) {
                u32 idx0, idx1;

                if (frame & 2) {
                    idx0 = (frame >> 2) + 1;
                    idx1 = frame >> 2;
                } else {
                    idx0 = frame >> 2;
                    idx1 = (frame >> 2) + 1;
                }
                return (u16)((data[idx0] + data[idx0] + data[idx0] + data[idx1]) >> 2);
            }
            return (u16)((data[frame >> 2] + *(data + (frame >> 2) + 1)) >> 1);
        }
        return data[frame >> 2];
    }
}

void func_020701c0(UnkAnmObj* obj, const UnkResAnm* anm, const UnkResMdl* mdl)
{
    u32 i;
    const UnkResMat* mat = (const UnkResMat*)((const u8*)mdl + mdl->ofsMat);

    obj->funcAnm = data_020c3dd4;
    obj->numMapData = mdl->info.numMat;
    MI_CpuFillU16(0, obj->mapData, obj->numMapData * sizeof(u16));

    for (i = 0; i < anm->dict.numEntry; i++) {
        int idx = func_0206e7a0(&mat->dict, UnkGetResNameByIdx_(&anm->dict, i));
        if (idx >= 0) {
            obj->mapData[idx] = (u16)(i | 0x100);
        }
    }
}

void func_02070258(UnkMatAnmResult* result, const UnkAnmObj* obj, u32 dataIdx)
{
    const UnkResAnm* anm = obj->resAnm;
    u32 frame = obj->frame >> 12;
    const UnkMatCAnmData* data = UnkGetResDataByIdx_(&anm->dict, (u16)dataIdx);
    u32 diff, amb, spec, emi;

    diff = func_0206ff6c(anm, data->tagDiffuse, frame);
    amb = func_0206ff6c(anm, data->tagAmbient, frame);
    result->prmMatColor0 = (result->prmMatColor0 & 0x8000) | diff | (amb << 16);

    emi = func_0206ff6c(anm, data->tagEmission, frame);
    spec = func_0206ff6c(anm, data->tagSpecular, frame);
    result->prmMatColor1 = (result->prmMatColor1 & 0x8000) | spec | (emi << 16);

    result->prmPolygonAttr = (result->prmPolygonAttr & ~0x1f0000) | (func_020700d4(anm, data->tagPolygonAlpha, frame) << 16);
}
