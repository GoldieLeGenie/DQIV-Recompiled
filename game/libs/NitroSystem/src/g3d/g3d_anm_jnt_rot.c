#include "g3d_anm.h"

/* joint animation (nsbca) rotation helpers */

typedef struct {
    u32 info;                       /* 0x00 */
    u32 offset;                     /* 0x04 */
} UnkJntAnmRInfo;

typedef struct {
    u8 category0;                   /* 0x00 */
    u8 revision;                    /* 0x01 */
    u16 category1;                  /* 0x02 */
    u16 numFrame;                   /* 0x04 */
    u16 numNode;                    /* 0x06 */
    u32 flag;                       /* 0x08 */
    u32 ofsRot3;                    /* 0x0C */
    u32 ofsRot5;                    /* 0x10 */
} UnkResJntAnm;

extern const u8 data_020ba604[][4];

BOOL func_0206fe00(UnkMtxFx33* rot, const fx16* pivotData, const fx16* rot5Data, u32 info);

/* row 2 = row 0 x row 1 */
static inline void CalcRow2_(UnkMtxFx33* m)
{
    fx32 x = (m->m[0][1] * m->m[1][2] - m->m[0][2] * m->m[1][1]) >> 12;
    fx32 y = (m->m[0][2] * m->m[1][0] - m->m[0][0] * m->m[1][2]) >> 12;
    fx32 z = (m->m[0][0] * m->m[1][1] - m->m[0][1] * m->m[1][0]) >> 12;

    m->m[2][0] = x;
    m->m[2][1] = y;
    m->m[2][2] = z;
}

/* rotation of a joint at a given frame (no interpolation between stored frames) */
void func_0206f6c4(UnkMtxFx33* rot, fx32 frame, const UnkJntAnmRInfo* rinfo, const UnkResJntAnm* anm)
{
    u32 info;
    u32 ofsRot5;
    u32 frameIdx;
    u32 ofsRot3;
    const u16* data;
    u32 idx;
    BOOL notDone;
    UnkMtxFx33 tmpA;
    UnkMtxFx33 tmpB;

    info = rinfo->info;
    frameIdx = frame >> 12;
    data = (const u16*)((const u8*)anm + rinfo->offset);
    ofsRot3 = anm->ofsRot3;
    ofsRot5 = anm->ofsRot5;

    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (frameIdx & 1) {
                if (frameIdx > last) {
                    frameIdx = (last >> 1) + 1;
                    goto single;
                }
                idx = frameIdx >> 1;
                goto avg2;
            }
            frameIdx >>= 1;
            goto single;
        } else {
            u32 rem = frameIdx & 3;
            if (rem != 0) {
                if (frameIdx > last) {
                    frameIdx = rem + (last >> 2);
                    goto single;
                }
                if (frameIdx & 1) {
                    u32 idx0;
                    BOOL notDone3 = FALSE;
                    if (frameIdx & 2) {
                        idx = frameIdx >> 2;
                        idx0 = idx + 1;
                    } else {
                        idx0 = frameIdx >> 2;
                        idx = idx0 + 1;
                    }
                    notDone3 |= func_0206fe00(rot, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[idx0]);
                    notDone3 |= func_0206fe00(&tmpA, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[idx]);

                    rot->m[0][0] = rot->m[0][0] * 3 + tmpA.m[0][0];
                    rot->m[0][1] = rot->m[0][1] * 3 + tmpA.m[0][1];
                    rot->m[0][2] = rot->m[0][2] * 3 + tmpA.m[0][2];
                    rot->m[1][0] = rot->m[1][0] * 3 + tmpA.m[1][0];
                    rot->m[1][1] = rot->m[1][1] * 3 + tmpA.m[1][1];
                    rot->m[1][2] = rot->m[1][2] * 3 + tmpA.m[1][2];
                    func_020630ec((UnkVecFx32*)&rot->m[0][0], (UnkVecFx32*)&rot->m[0][0]);
                    func_020630ec((UnkVecFx32*)&rot->m[1][0], (UnkVecFx32*)&rot->m[1][0]);

                    if (!notDone3) {
                        rot->m[2][0] = rot->m[2][0] * 3 + tmpA.m[2][0];
                        rot->m[2][1] = rot->m[2][1] * 3 + tmpA.m[2][1];
                        rot->m[2][2] = rot->m[2][2] * 3 + tmpA.m[2][2];
                        func_020630ec((UnkVecFx32*)&rot->m[2][0], (UnkVecFx32*)&rot->m[2][0]);
                    } else {
                        CalcRow2_(rot);
                    }
                    return;
                }
                idx = frameIdx >> 2;
                goto avg2;
            } else {
                frameIdx >>= 2;
                goto single;
            }
        }
    }

    goto single;

avg2:
    notDone = FALSE;
    notDone |= func_0206fe00(rot, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[idx]);
    notDone |= func_0206fe00(&tmpB, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), *(data + idx + 1));

    rot->m[0][0] += tmpB.m[0][0];
    rot->m[0][1] += tmpB.m[0][1];
    rot->m[0][2] += tmpB.m[0][2];
    rot->m[1][0] += tmpB.m[1][0];
    rot->m[1][1] += tmpB.m[1][1];
    rot->m[1][2] += tmpB.m[1][2];
    func_020630ec((UnkVecFx32*)&rot->m[0][0], (UnkVecFx32*)&rot->m[0][0]);
    func_020630ec((UnkVecFx32*)&rot->m[1][0], (UnkVecFx32*)&rot->m[1][0]);

    if (!notDone) {
        rot->m[2][0] += tmpB.m[2][0];
        rot->m[2][1] += tmpB.m[2][1];
        rot->m[2][2] += tmpB.m[2][2];
        func_020630ec((UnkVecFx32*)&rot->m[2][0], (UnkVecFx32*)&rot->m[2][0]);
    } else {
        CalcRow2_(rot);
    }
    return;

single:
    if (func_0206fe00(rot, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[frameIdx])) {
        CalcRow2_(rot);
    }

}

/* rotation of a joint at a given frame (with interpolation) */
void func_0206facc(UnkMtxFx33* rot, fx32 frame, const UnkJntAnmRInfo* rinfo, const UnkResJntAnm* anm)
{
    u32 ofsRot5;
    u32 frameIdx;
    u32 ofsRot3;
    u32 idx1;
    u32 info;
    fx32 t;
    fx32 w;
    const u16* data;
    BOOL notDone;
    UnkMtxFx33 tmp0;
    UnkMtxFx33 tmp1;

    data = (const u16*)((const u8*)anm + rinfo->offset);
    frameIdx = frame >> 12;
    ofsRot3 = anm->ofsRot3;
    ofsRot5 = anm->ofsRot5;
    info = rinfo->info;

    if (anm->numFrame - 1 == frameIdx) {
        if (info & 0xc0000000) {
            if (info & 0x40000000) {
                frameIdx = (frameIdx & 1) + (frameIdx >> 1);
            } else {
                frameIdx = (frameIdx & 3) + (frameIdx >> 2);
            }
        }
        if (anm->flag & 2) {
            idx1 = 0;
            goto step1;
        }
        if (func_0206fe00(rot, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[frameIdx])) {
            CalcRow2_(rot);
        }
        return;
    }

    if (info & 0xc0000000) {
        u32 last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (frameIdx >= last) {
                frameIdx = last >> 1;
                idx1 = frameIdx + 1;
                goto step1;
            }
            frameIdx >>= 1;
            idx1 = frameIdx + 1;
            t = frame & 0x1fff;
            w = 2;
        } else {
            if (frameIdx >= last) {
                frameIdx = (frameIdx & 3) + (frameIdx >> 2);
                idx1 = frameIdx + 1;
                goto step1;
            }
            frameIdx >>= 2;
            idx1 = frameIdx + 1;
            t = frame & 0x3fff;
            w = 4;
        }
    } else {
        idx1 = frameIdx + 1;
step1:
        t = frame & 0xfff;
        w = 1;
    }

    notDone = FALSE;
    notDone |= func_0206fe00(&tmp0, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[frameIdx]);
    notDone |= func_0206fe00(&tmp1, (const fx16*)((const u8*)anm + ofsRot3), (const fx16*)((const u8*)anm + ofsRot5), data[idx1]);

    rot->m[0][0] = tmp0.m[0][0] * w + (((tmp1.m[0][0] - tmp0.m[0][0]) * t) >> 12);
    rot->m[0][1] = tmp0.m[0][1] * w + (((tmp1.m[0][1] - tmp0.m[0][1]) * t) >> 12);
    rot->m[0][2] = tmp0.m[0][2] * w + (((tmp1.m[0][2] - tmp0.m[0][2]) * t) >> 12);
    rot->m[1][0] = tmp0.m[1][0] * w + (((tmp1.m[1][0] - tmp0.m[1][0]) * t) >> 12);
    rot->m[1][1] = tmp0.m[1][1] * w + (((tmp1.m[1][1] - tmp0.m[1][1]) * t) >> 12);
    rot->m[1][2] = tmp0.m[1][2] * w + (((tmp1.m[1][2] - tmp0.m[1][2]) * t) >> 12);
    func_020630ec((UnkVecFx32*)&rot->m[0][0], (UnkVecFx32*)&rot->m[0][0]);
    func_020630ec((UnkVecFx32*)&rot->m[1][0], (UnkVecFx32*)&rot->m[1][0]);

    if (!notDone) {
        rot->m[2][0] = tmp0.m[2][0] * w + (((tmp1.m[2][0] - tmp0.m[2][0]) * t) >> 12);
        rot->m[2][1] = tmp0.m[2][1] * w + (((tmp1.m[2][1] - tmp0.m[2][1]) * t) >> 12);
        rot->m[2][2] = tmp0.m[2][2] * w + (((tmp1.m[2][2] - tmp0.m[2][2]) * t) >> 12);
        func_020630ec((UnkVecFx32*)&rot->m[2][0], (UnkVecFx32*)&rot->m[2][0]);
    } else {
        CalcRow2_(rot);
    }
}

/* decodes one compressed rotation matrix; returns TRUE when row 2 still has to be computed */
BOOL func_0206fe00(UnkMtxFx33* rot, const fx16* pivotData, const fx16* rot5Data, u32 info)
{
    if (info & 0x8000) {
        fx32* m = &rot->m[0][0];
        u32 idxPivot;
        const fx16* data;
        fx32 A;
        fx32 B;

        rot->m[0][0] = rot->m[0][1] = rot->m[0][2] =
        rot->m[1][0] = rot->m[1][1] = rot->m[1][2] =
        rot->m[2][0] = rot->m[2][1] = rot->m[2][2] = 0;

        data = pivotData + (info & 0x7fff) * 3;
        idxPivot = data[0] & 0xf;
        A = data[1];
        B = data[2];

        m[idxPivot] = (data[0] & 0x10) ? -0x1000 : 0x1000;
        m[data_020ba604[idxPivot][0]] = A;
        m[data_020ba604[idxPivot][1]] = B;
        m[data_020ba604[idxPivot][2]] = (data[0] & 0x20) ? -B : B;
        m[data_020ba604[idxPivot][3]] = (data[0] & 0x40) ? -A : A;
        return FALSE;
    } else {
        const fx16* data = rot5Data + (info & 0x7fff) * 5;
        s32 v;
        s16 t;

        v = data[4];
        rot->m[1][1] = v >> 3;
        t = (s16)(v & 7);
        v = data[0];
        rot->m[0][0] = v >> 3;
        t = (s16)((t << 3) | (v & 7));
        v = data[1];
        rot->m[0][1] = v >> 3;
        t = (s16)((t << 3) | (v & 7));
        v = data[2];
        rot->m[0][2] = v >> 3;
        t = (s16)((t << 3) | (v & 7));
        v = data[3];
        rot->m[1][0] = v >> 3;
        t = (s16)((t << 3) | (v & 7));
        rot->m[1][2] = ((s32)t << 19) >> 19;
        return TRUE;
    }
}
