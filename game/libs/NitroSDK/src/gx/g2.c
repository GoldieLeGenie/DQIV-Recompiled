#include "gx_internal.h"

/* 2x2 fx32 matrix */
typedef struct {
    s32 _00, _01, _10, _11;
} UnkMtx22;

/* Write a BG affine matrix (PA/PB/PC/PD) and reference point (X/Y) */
void func_02064f40(vu32 *reg, const UnkMtx22 *mtx, s32 centerX, s32 centerY, s32 x1, s32 y1) {
    s32 dx, dy, x, y;

    reg[0] = (u16)(s16)(mtx->_00 >> 4) | ((u16)(s16)(mtx->_01 >> 4) << 16);
    reg[1] = (u16)(s16)(mtx->_10 >> 4) | ((u16)(s16)(mtx->_11 >> 4) << 16);

    dx = x1 - centerX;
    dy = y1 - centerY;
    x = mtx->_00 * dx + mtx->_01 * dy + (centerX << 12);
    y = mtx->_10 * dx + mtx->_11 * dy + (centerY << 12);
    reg[2] = x >> 4;
    reg[3] = y >> 4;
}

void _G2_SetBlend(vu32 *reg, s32 plane1, s32 plane2, s32 ev1, s32 ev2) {
    *reg = (plane1 | 0x40 | (plane2 << 8)) | ((ev1 | (ev2 << 8)) << 16);
}

/* Set the brightness up/down effect */
void func_0206500c(vu16 *reg, s32 plane, s32 brightness) {
    if (brightness < 0) {
        reg[0] = plane | 0xc0;
        reg[2] = -brightness;
    } else {
        reg[0] = plane | 0x80;
        reg[2] = brightness;
    }
}
