#include "g3d_types.h"

/* Default blend functions for material / joint / visibility animation results. */

typedef struct UnkVecFx32 {
    s32 unk_00;                 // 0x00 x
    s32 unk_04;                 // 0x04 y
    s32 unk_08;                 // 0x08 z
} UnkVecFx32;

/* Joint animation result (0x58 bytes). */
typedef struct UnkJntAnmResult {
    u32 unk_00;                 // 0x00 flags: 1 scale one, 2 rotation zero, 4 translation zero, 8/0x10 extra scales one
    UnkVecFx32 unk_04;          // 0x04 scale
    UnkVecFx32 unk_10;          // 0x10 extra scale 0
    UnkVecFx32 unk_1c;          // 0x1C extra scale 1
    UnkVecFx32 unk_28;          // 0x28 rotation row 0
    UnkVecFx32 unk_34;          // 0x34 rotation row 1
    UnkVecFx32 unk_40;          // 0x40 rotation row 2
    UnkVecFx32 unk_4c;          // 0x4C translation
} UnkJntAnmResult;

extern void MI_CpuFill(u32 value, void* dest, u32 size);
extern s32 FX_Divide(s32 numer, s32 denom);
extern void func_02063008(const UnkVecFx32* a, const UnkVecFx32* b, UnkVecFx32* axb);
extern void func_020630ec(const UnkVecFx32* src, UnkVecFx32* dst);

BOOL func_0206b308(void* result, UnkAnmObj* anm, int matID) {
    BOOL ret = FALSE;

    do {
        if (anm->unk_1a[matID] & 0x100) {
            anm->unk_0c(result, anm, anm->unk_1a[matID] & 0xff);
            ret = TRUE;
        }
        anm = anm->unk_10;
    } while (anm != NULL);
    return ret;
}

/* Adds src * ratio to dst (or ratio * 1.0 when isOne is set). */
void func_0206b358(UnkVecFx32* dst, const UnkVecFx32* src, s32 ratio, u32 isOne) {
    if (isOne) {
        dst->unk_00 += ratio;
        dst->unk_04 += ratio;
        dst->unk_08 += ratio;
    } else {
        dst->unk_00 += (ratio * src->unk_00) >> 12;
        dst->unk_04 += (ratio * src->unk_04) >> 12;
        dst->unk_08 += (ratio * src->unk_08) >> 12;
    }
}

BOOL func_0206b3c8(UnkJntAnmResult* result, UnkAnmObj* anm, int jntID) {
    s32 sumRatio;
    UnkAnmObj* p;
    int num;
    UnkAnmObj* last;

    if (anm->unk_10 == NULL) {
        u16 data = anm->unk_1a[jntID];

        if ((data & 0x300) != 0x100) {
            return FALSE;
        }
        anm->unk_0c(result, anm, data & 0xff);
        return TRUE;
    }

    sumRatio = 0;
    num = 0;
    p = anm;
    do {
        if ((p->unk_1a[jntID] & 0x300) == 0x100) {
            last = p;
            sumRatio += p->unk_04;
            num++;
        }
        p = p->unk_10;
    } while (p != NULL);

    if (sumRatio == 0) {
        return FALSE;
    }
    if (num == 1) {
        last->unk_0c(result, last, last->unk_1a[jntID] & 0xff);
        return TRUE;
    }

    MI_CpuFill(0, result, sizeof(UnkJntAnmResult));
    result->unk_00 = 0xffffffff;
    do {
        UnkJntAnmResult r;
        u16 data = anm->unk_1a[jntID];

        if ((data & 0x300) == 0x100 && anm->unk_04 > 0) {
            s32 ratio;

            anm->unk_0c(&r, anm, data & 0xff);
            if (sumRatio == 0x1000) {
                ratio = anm->unk_04;
            } else {
                ratio = FX_Divide(anm->unk_04, sumRatio);
            }
            func_0206b358(&result->unk_04, &r.unk_04, ratio, r.unk_00 & 1);
            func_0206b358(&result->unk_10, &r.unk_10, ratio, r.unk_00 & 8);
            func_0206b358(&result->unk_1c, &r.unk_1c, ratio, r.unk_00 & 0x10);
            if (!(r.unk_00 & 4)) {
                result->unk_4c.unk_00 += (s32)(((s64)ratio * r.unk_4c.unk_00) >> 12);
                result->unk_4c.unk_04 += (s32)(((s64)ratio * r.unk_4c.unk_04) >> 12);
                result->unk_4c.unk_08 += (s32)(((s64)ratio * r.unk_4c.unk_08) >> 12);
            }
            if (!(r.unk_00 & 2)) {
                result->unk_28.unk_00 += (ratio * r.unk_28.unk_00) >> 12;
                result->unk_28.unk_04 += (ratio * r.unk_28.unk_04) >> 12;
                result->unk_28.unk_08 += (ratio * r.unk_28.unk_08) >> 12;
                result->unk_34.unk_00 += (ratio * r.unk_34.unk_00) >> 12;
                result->unk_34.unk_04 += (ratio * r.unk_34.unk_04) >> 12;
                result->unk_34.unk_08 += (ratio * r.unk_34.unk_08) >> 12;
            } else {
                result->unk_28.unk_00 += ratio;
                result->unk_34.unk_04 += ratio;
            }
            result->unk_00 &= r.unk_00;
        }
        anm = anm->unk_10;
    } while (anm != NULL);

    func_02063008(&result->unk_28, &result->unk_34, &result->unk_40);
    func_020630ec(&result->unk_28, &result->unk_28);
    func_020630ec(&result->unk_40, &result->unk_40);
    func_02063008(&result->unk_40, &result->unk_28, &result->unk_34);
    return TRUE;
}

BOOL func_0206b6b0(u32* result, UnkAnmObj* anm, int nodeID) {
    BOOL ret;

    *result = 0;
    ret = FALSE;
    do {
        if (anm->unk_1a[nodeID] & 0x100) {
            u32 r;

            anm->unk_0c(&r, anm, anm->unk_1a[nodeID] & 0xff);
            *result |= r;
            ret = TRUE;
        }
        anm = anm->unk_10;
    } while (anm != NULL);
    return ret;
}

/* default animation functions */
void func_02070258(void* result, UnkAnmObj* anm, u32 dataIdx); // material (g3d_anm_mat.c)
void func_020708a8(void* result, UnkAnmObj* anm, u32 dataIdx); // texture pattern (g3d_anm_texpat.c)
void func_020706ac(void* result, UnkAnmObj* anm, u32 dataIdx); // texture SRT (g3d_anm_texsrt.c)
void func_0206ea74(void* result, UnkAnmObj* anm, u32 dataIdx); // joint (g3d_anm_jnt.c)
void func_02070968(void* result, UnkAnmObj* anm, u32 dataIdx); // visibility (g3d_anm_vis.c)
/* animation object init functions */
void func_020701c0(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl); // material color (g3d_anm_mat.c)
void func_020706f0(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl); // texture pattern (g3d_anm_texpat.c)
void func_02070614(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl); // texture SRT (g3d_anm_texsrt.c)
void func_0207091c(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl); // visibility (g3d_anm_vis.c)
void func_0206e9f8(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl); // joint (g3d_anm_jnt.c)

/* default blend functions (set into render objects by g3d_kernel.c) */
UnkBlendFunc data_020c3de0 = (UnkBlendFunc)func_0206b308; // material
UnkBlendFunc data_020c3ddc = (UnkBlendFunc)func_0206b3c8; // joint
UnkBlendFunc data_020c3dd8 = (UnkBlendFunc)func_0206b6b0; // visibility

/* default animation functions (set into animation objects by their init functions) */
UnkAnmFunc data_020c3dd4 = func_02070258; // material
UnkAnmFunc data_020c3dd0 = func_020708a8; // texture pattern
UnkAnmFunc data_020c3dcc = func_020706ac; // texture SRT
UnkAnmFunc data_020c3dc8 = func_0206ea74; // joint
UnkAnmFunc data_020c3dc4 = func_02070968; // visibility

/* number of used entries in data_020c3de4 */
u32 data_020c3dc0 = 5;

/* animation object init functions per resource category ('M' / 'J' / 'V') and sub category */
UnkAnmInitEntry data_020c3de4[10] = {
    {'M', 0x4d41, func_020701c0},
    {'M', 0x5450, func_020706f0},
    {'M', 0x5441, func_02070614},
    {'V', 0x5641, func_0207091c},
    {'J', 0x4341, func_0206e9f8},
};
