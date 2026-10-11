#include "g3d_anm.h"

/* texture SRT animation (nsbta) */
typedef struct {
    u32 scaleSInfo;                 /* 0x00 */
    u32 scaleSData;                 /* 0x04 */
    u32 scaleTInfo;                 /* 0x08 */
    u32 scaleTData;                 /* 0x0C */
    u32 rotInfo;                    /* 0x10 */
    u32 rotData;                    /* 0x14 */
    u32 transSInfo;                 /* 0x18 */
    u32 transSData;                 /* 0x1C */
    u32 transTInfo;                 /* 0x20 */
    u32 transTData;                 /* 0x24 */
} UnkTexSRTAnmData;

typedef struct {
    fx16 sin;
    fx16 cos;
} UnkRotData;

extern UnkAnmFunc data_020c3dcc;

/* scale/translation value of a texture SRT track */
fx32 func_02070324(const u8* p, u32 info, u32 data, u32 frame)
{
    u32 last;

    if (info & 0x20000000) {
        return (fx32)data;
    }

    p += data;
    if (info & 0xc0000000) {
        last = (u16)info;
        if (info & 0x40000000) {
            if (frame & 1) {
                if (frame > last) {
                    frame = (last >> 1) + 1;
                } else {
                    frame >>= 1;
                    goto avg2;
                }
            } else {
                frame >>= 1;
            }
        } else {
            data = frame & 3;
            if (data != 0) {
                if (frame > last) {
                    frame = data + (last >> 2);
                } else if (frame & 1) {
                    u32 idx0;
                    fx32 v0, v1;

                    if (frame & 2) {
                        frame = frame >> 2;
                        idx0 = frame + 1;
                    } else {
                        idx0 = frame >> 2;
                        frame = idx0 + 1;
                    }
                    if (info & 0x10000000) {
                        v0 = ((const fx16*)p)[idx0];
                        v1 = ((const fx16*)p)[frame];
                    } else {
                        v0 = ((const fx32*)p)[idx0];
                        v1 = ((const fx32*)p)[frame];
                    }
                    return (v0 + v0 + v0 + v1) >> 2;
                } else {
                    frame >>= 2;
                    goto avg2;
                }
            } else {
                frame >>= 2;
            }
        }
    }

    if (info & 0x10000000) {
        return ((const fx16*)p)[frame];
    } else {
        return ((const fx32*)p)[frame];
    }

avg2:
    {
        fx32 v0, v1;
        if (info & 0x10000000) {
            const fx16* q = (const fx16*)p + frame;
            v0 = ((const fx16*)p)[frame];
            v1 = q[1];
        } else {
            const fx32* q = (const fx32*)p + frame;
            v0 = q[0];
            v1 = q[1];
        }
        return (v0 + v1) >> 1;
    }
}

/* rotation value (cos << 16 | sin) of a texture SRT track */
u32 func_02070424(const u8* pData, u32 info, u32 data, u32 frame)
{
    const UnkRotData* p;
    u32 last;

    if (info & 0x20000000) {
        return data;
    }

    pData += data;
    p = (const UnkRotData*)pData;
    if (info & 0xc0000000) {
        last = (u16)info;
        if (info & 0x40000000) {
            if (frame & 1) {
                if (frame > last) {
                    frame = (last >> 1) + 1;
                } else {
                    info = frame >> 1;
                    goto avg2;
                }
            } else {
                frame >>= 1;
            }
        } else {
            info = frame & 3;
            if (info != 0) {
                if (frame > last) {
                    frame = info + (last >> 2);
                } else if (frame & 1) {
                    if (frame & 2) {
                        frame = frame >> 2;
                        info = frame + 1;
                    } else {
                        info = frame >> 2;
                        frame = info + 1;
                    }
                    return (u16)((p[info].sin + p[info].sin + p[info].sin + p[frame].sin) >> 2) |
                           (((p[info].cos + p[info].cos + p[info].cos + p[frame].cos) >> 2) << 16);
                } else {
                    info = frame >> 2;
                    goto avg2;
                }
            } else {
                frame >>= 2;
            }
        }
    }
    return ((const u32*)pData)[frame];

avg2:
    return (u16)((p[info].sin + (p + info)[1].sin) >> 1) | ((((p + info)->cos + (p + info)[1].cos) >> 1) << 16);
}

void func_02070530(const UnkResAnm* anm, u32 dataIdx, u32 frame, UnkMatAnmResult* result)
{
    const UnkTexSRTAnmData* data = UnkGetResDataByIdx_(&anm->dict, dataIdx);
    u32 flag = result->flag;
    fx32 s, t;
    u32 rot;

    s = func_02070324((const u8*)anm, data->transSInfo, data->transSData, frame);
    t = func_02070324((const u8*)anm, data->transTInfo, data->transTData, frame);
    if (s == 0 && t == 0) {
        flag |= 4;
    } else {
        result->transS = s;
        result->transT = t;
        flag &= ~4;
    }

    rot = func_02070424((const u8*)anm, data->rotInfo, data->rotData, frame);
    if (rot == 0x10000000) {
        flag |= 2;
    } else {
        result->sinR = (fx16)rot;
        result->cosR = (fx16)(rot >> 16);
        flag &= ~2;
    }

    s = func_02070324((const u8*)anm, data->scaleSInfo, data->scaleSData, frame);
    t = func_02070324((const u8*)anm, data->scaleTInfo, data->scaleTData, frame);
    if (s == 0x1000 && t == 0x1000) {
        flag |= 1;
    } else {
        result->scaleS = s;
        result->scaleT = t;
        flag &= ~1;
    }

    result->flag = flag;
}

void func_02070614(UnkAnmObj* obj, const UnkResAnm* anm, const UnkResMdl* mdl)
{
    u32 i;
    const UnkResMat* mat = (const UnkResMat*)((const u8*)mdl + mdl->ofsMat);

    obj->funcAnm = data_020c3dcc;
    obj->numMapData = mdl->info.numMat;
    MI_CpuFillU16(0, obj->mapData, obj->numMapData * sizeof(u16));

    for (i = 0; i < anm->dict.numEntry; i++) {
        int idx = func_0206e7a0(&mat->dict, UnkGetResNameByIdx_(&anm->dict, i));
        if (idx >= 0) {
            obj->mapData[idx] = (u16)(i | 0x100);
        }
    }
}

void func_020706ac(UnkMatAnmResult* result, const UnkAnmObj* obj, u32 dataIdx)
{
    func_02070530(obj->resAnm, (u16)dataIdx, obj->frame >> 12, result);
    result->prmTexImage = (result->prmTexImage & ~0xc0000000) | 0x40000000;
    result->flag |= 8;
}
