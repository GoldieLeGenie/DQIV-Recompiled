#include <nitro/types.h>

/* Global 3D state: camera / projection matrices, lights, base transform and cached inverse matrices. */

typedef struct UnkVecFx32 {
    s32 unk_00;                 // 0x00 x
    s32 unk_04;                 // 0x04 y
    s32 unk_08;                 // 0x08 z
} UnkVecFx32;

typedef struct UnkMtx33 {
    s32 unk_00[9];              // 0x00
} UnkMtx33;

typedef struct UnkMtx43 {
    s32 unk_00[12];             // 0x00
} UnkMtx43;

typedef struct UnkMtx44 {
    s32 unk_00[4][4];           // 0x00
} UnkMtx44;

typedef struct UnkG3dGlb {
    u32 unk_00;                 // 0x000 packed commands
    u32 unk_04;                 // 0x004 matrix mode
    UnkMtx44 unk_08;            // 0x008 projection matrix
    u32 unk_48;                 // 0x048 matrix mode
    UnkMtx43 unk_4c;            // 0x04C camera matrix
    u32 unk_7c;                 // 0x07C packed commands
    u32 unk_80[4];              // 0x080 light vectors
    u32 unk_90;                 // 0x090 packed commands
    u32 unk_94;                 // 0x094 diffuse / ambient
    u32 unk_98;                 // 0x098 specular / emission
    u32 unk_9c;                 // 0x09C polygon attributes
    u32 unk_a0;                 // 0x0A0 viewport
    u32 unk_a4;                 // 0x0A4 packed commands
    u32 unk_a8[4];              // 0x0A8 light colors
    u32 unk_b8;                 // 0x0B8 packed commands
    UnkMtx33 unk_bc;            // 0x0BC base rotation
    UnkVecFx32 unk_e0;          // 0x0E0 base translation
    UnkVecFx32 unk_ec;          // 0x0EC base scale
    u32 unk_f8;                 // 0x0F8
    u32 unk_fc;                 // 0x0FC flags (bits set = cached matrix valid)
    UnkMtx43 unk_100;           // 0x100 inverse camera matrix
    UnkMtx43 unk_130;           // 0x130 base * camera matrix
    UnkMtx43 unk_160;           // 0x160 inverse of unk_130
    u8 unk_190[0x30];           // 0x190
    UnkMtx44 unk_1c0;           // 0x1C0 inverse projection matrix
    UnkMtx44 unk_200;           // 0x200 inverse (projection * camera) matrix
    UnkVecFx32 unk_240;         // 0x240 camera position
    UnkVecFx32 unk_24c;         // 0x24C camera up
    UnkVecFx32 unk_258;         // 0x258 camera target
} UnkG3dGlb;                    // 0x264

UnkG3dGlb data_0210cf28;

extern void func_02061b88(UnkMtx33* m);
extern void func_02061f58(UnkMtx43* m);
extern void func_02061f80(const UnkMtx43* src, UnkMtx44* dst);
extern void func_02061fb4(const UnkMtx43* src, UnkMtx43* dst, s32 x, s32 y, s32 z);
extern void func_02062040(const UnkMtx43* src, UnkMtx43* dst);
extern void func_020623cc(const UnkMtx33* a, const UnkMtx43* b, UnkMtx43* ab);
extern void func_02062740(UnkMtx44* m);
extern void func_020627a0(const UnkMtx44* a, const UnkMtx44* b, UnkMtx44* ab);
extern void func_0206de34(u32 cmd, const void* param, u32 nParams);
extern void MI_CpuCopy64(const void* src, void* dest);
extern s64 FX_Divide64(s32 denom);

void func_0206ac78(void) {
    data_0210cf28.unk_00 = 0x17101610;
    data_0210cf28.unk_04 = 0;
    data_0210cf28.unk_48 = 2;
    data_0210cf28.unk_7c = 0x32323232;
    data_0210cf28.unk_90 = 0x60293130;
    data_0210cf28.unk_a4 = 0x33333333;
    data_0210cf28.unk_b8 = 0x2a1b19;
    func_02061f58(&data_0210cf28.unk_4c);
    func_02062740(&data_0210cf28.unk_08);
    data_0210cf28.unk_80[0] = 0x2d8b62d8;
    data_0210cf28.unk_80[1] = 0x40000200;
    data_0210cf28.unk_80[2] = 0x800001ff;
    data_0210cf28.unk_80[3] = 0xc0080000;
    data_0210cf28.unk_94 = 0x4210c210;
    data_0210cf28.unk_98 = 0x4210c210;
    data_0210cf28.unk_9c = 0x1f008f;
    data_0210cf28.unk_a0 = 0xbfff0000;
    data_0210cf28.unk_a8[0] = 0x7fff;
    data_0210cf28.unk_a8[1] = 0x4000001f;
    data_0210cf28.unk_a8[2] = 0x800003e0;
    data_0210cf28.unk_a8[3] = 0xc0007c00;
    data_0210cf28.unk_e0.unk_00 = 0;
    data_0210cf28.unk_e0.unk_04 = 0;
    data_0210cf28.unk_e0.unk_08 = 0;
    func_02061b88(&data_0210cf28.unk_bc);
    data_0210cf28.unk_ec.unk_00 = 0x1000;
    data_0210cf28.unk_ec.unk_04 = 0x1000;
    data_0210cf28.unk_ec.unk_08 = 0x1000;
    data_0210cf28.unk_f8 = 0;
    data_0210cf28.unk_fc = 0;
    data_0210cf28.unk_240.unk_08 = 0;
    data_0210cf28.unk_240.unk_04 = 0;
    data_0210cf28.unk_240.unk_00 = 0;
    data_0210cf28.unk_24c.unk_08 = 0;
    data_0210cf28.unk_24c.unk_00 = 0;
    data_0210cf28.unk_24c.unk_04 = 0x1000;
    data_0210cf28.unk_258.unk_04 = 0;
    data_0210cf28.unk_258.unk_00 = 0;
    data_0210cf28.unk_258.unk_08 = -0x1000;
}

/* Sends the projection / camera state to the geometry engine. */
void func_0206adcc(void) {
    func_0206de34(data_0210cf28.unk_00, (u32*)&data_0210cf28 + 1, 0x3e);
    data_0210cf28.unk_fc &= ~1;
    data_0210cf28.unk_fc &= ~2;
}

void func_0206ae08(const UnkVecFx32* trans) {
    data_0210cf28.unk_e0 = *trans;
    data_0210cf28.unk_fc &= ~0xa4;
}

void func_0206ae30(const UnkVecFx32* scale) {
    data_0210cf28.unk_ec = *scale;
    data_0210cf28.unk_fc &= ~0xa4;
}

void func_0206ae58(int lightID, s16 x, s16 y, s16 z) {
    data_0210cf28.unk_80[lightID] =
        ((x >> 3) & 0x3ff) | (((y >> 3) & 0x3ff) << 10) | (((z >> 3) & 0x3ff) << 20) | (lightID << 30);
}

void func_0206ae94(int lightID, u16 rgb) {
    data_0210cf28.unk_a8[lightID] = rgb | (lightID << 30);
}

void func_0206aea8(int light, int polyMode, int cullMode, int polygonID, int alpha, int misc) {
    data_0210cf28.unk_9c = (light | (polyMode << 4) | (cullMode << 6)) | misc | (polygonID << 24) | (alpha << 16);
}

UnkMtx43* func_0206aed4(void) {
    if (!(data_0210cf28.unk_fc & 8)) {
        func_02062040(&data_0210cf28.unk_4c, &data_0210cf28.unk_100);
        data_0210cf28.unk_fc |= 8;
    }
    return &data_0210cf28.unk_100;
}

/* Inverts a 4x4 matrix by Gauss-Jordan elimination; returns -1 when it is singular. */
int func_0206af18(const UnkMtx44* src, UnkMtx44* dst) {
    UnkMtx44 tmp;
    int i;
    int j;
    int k;

    MI_CpuCopy64(src, &tmp);
    func_02062740(dst);

    for (i = 0; i < 4; i++) {
        s32 max = 0;
        int maxRow = i;
        s64 inv;

        for (j = i; j < 4; j++) {
            s32 v = tmp.unk_00[j][i];

            if (v < 0) {
                v = -v;
            }
            if (v > max) {
                maxRow = j;
                max = v;
            }
        }
        if (max == 0) {
            return -1;
        }

        if (maxRow != i) {
            int k;

            for (k = 0; k < 4; k++) {
                s32 t = tmp.unk_00[i][k];
                tmp.unk_00[i][k] = tmp.unk_00[maxRow][k];
                tmp.unk_00[maxRow][k] = t;

                t = dst->unk_00[i][k];
                dst->unk_00[i][k] = dst->unk_00[maxRow][k];
                dst->unk_00[maxRow][k] = t;
            }
        }

        inv = FX_Divide64(tmp.unk_00[i][i]);
        for (k = 0; k < 4; k++) {
            tmp.unk_00[i][k] = (s32)((inv * tmp.unk_00[i][k] + 0x80000000LL) >> 32);
            dst->unk_00[i][k] = (s32)((inv * dst->unk_00[i][k] + 0x80000000LL) >> 32);
        }

        for (j = 0; j < 4; j++) {
            if (j != i) {
                s32 f = tmp.unk_00[j][i];

                for (k = 0; k < 4; k++) {
                    tmp.unk_00[j][k] = tmp.unk_00[j][k] - (((s64)f * tmp.unk_00[i][k]) >> 12);
                    dst->unk_00[j][k] = dst->unk_00[j][k] - (((s64)f * dst->unk_00[i][k]) >> 12);
                }
            }
        }
    }
    return 0;
}

UnkMtx44* func_0206b120(void) {
    if (!(data_0210cf28.unk_fc & 0x10)) {
        func_0206af18(&data_0210cf28.unk_08, &data_0210cf28.unk_1c0);
        data_0210cf28.unk_fc |= 0x10;
    }
    return &data_0210cf28.unk_1c0;
}

void func_0206b164(void) {
    func_020623cc(&data_0210cf28.unk_bc, &data_0210cf28.unk_4c, &data_0210cf28.unk_130);
    func_02061fb4(&data_0210cf28.unk_130, &data_0210cf28.unk_130, data_0210cf28.unk_ec.unk_00,
                  data_0210cf28.unk_ec.unk_04, data_0210cf28.unk_ec.unk_08);
    func_02062040(&data_0210cf28.unk_130, &data_0210cf28.unk_160);
}

UnkMtx43* func_0206b1bc(void) {
    if (!(data_0210cf28.unk_fc & 0x80)) {
        func_0206b164();
        data_0210cf28.unk_fc |= 0x80;
    }
    return &data_0210cf28.unk_130;
}

UnkMtx43* func_0206b1f4(void) {
    if (!(data_0210cf28.unk_fc & 0x80)) {
        func_0206b164();
        data_0210cf28.unk_fc |= 0x80;
    }
    return &data_0210cf28.unk_160;
}

UnkMtx44* func_0206b22c(void) {
    if (!(data_0210cf28.unk_fc & 0x40)) {
        UnkMtx43* invCamera = func_0206aed4();
        UnkMtx44* invProj = func_0206b120();
        UnkMtx44 tmp;

        func_02061f80(invCamera, &tmp);
        func_020627a0(invProj, &tmp, &data_0210cf28.unk_200);
        data_0210cf28.unk_fc |= 0x40;
    }
    return &data_0210cf28.unk_200;
}

void func_0206b294(int* x1, int* y1, int* x2, int* y2) {
    if (x1 != NULL) {
        *x1 = data_0210cf28.unk_a0 & 0xff;
    }
    if (y1 != NULL) {
        *y1 = (data_0210cf28.unk_a0 >> 8) & 0xff;
    }
    if (x2 != NULL) {
        *x2 = (data_0210cf28.unk_a0 >> 16) & 0xff;
    }
    if (y2 != NULL) {
        *y2 = (data_0210cf28.unk_a0 >> 24) & 0xff;
    }
}
