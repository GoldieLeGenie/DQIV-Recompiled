#include <nitro/native_g3.h>
#ifndef DQIV_NATIVE
#include "gx_internal.h"
#endif

#define REG_MTX_MODE       (*(vu32 *)0x04000440)
#define REG_MTX_LOAD_4x4   (*(vu32 *)0x04000458)
#define REG_MTX_LOAD_4x3   (*(vu32 *)0x0400045c)
#define REG_MTX_MULT_3x3   (*(vu32 *)0x04000468)
#define REG_DIV_NUMER_V    (*(u64 *)0x04000290)
#define REG_DIV_DENOM_V    (*(u64 *)0x04000298)

typedef struct {
    s32 x, y, z;
} UnkVec32;

typedef struct {
    s32 _00, _01, _02, _03;
    s32 _10, _11, _12, _13;
    s32 _20, _21, _22, _23;
    s32 _30, _31, _32, _33;
} UnkMtx44;

typedef struct {
    s32 _00, _01, _02;
    s32 _10, _11, _12;
    s32 _20, _21, _22;
    s32 _30, _31, _32;
} UnkMtx43;

s32 FX_Divide(s32 numer, s32 denom);
s64 FX_GetDivResultFx64c(void);
s32 FX_GetDivResult(void);
void FX_InvAsync(s32 denom);
s32 func_02062fcc(const UnkVec32 *a, const UnkVec32 *b);
void func_02063008(const UnkVec32 *a, const UnkVec32 *b, UnkVec32 *dest);
void func_020630ec(const UnkVec32 *src, UnkVec32 *dest);

#ifdef DQIV_NATIVE
void FX_DivAsync(s32, s32);
#endif
static inline void UnkDivAsyncFx(s32 numer, s32 denom) {
#ifdef DQIV_NATIVE
    FX_DivAsync(numer, denom);
#else
    REG_DIV_NUMER_V = (s64)numer << 32;
    REG_DIV_DENOM_V = (u32)denom;
#endif
}

static inline s32 UnkMulFx(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 UnkMul32x64c(s32 v32, s64 v64c) {
    s64 tmp = v64c * v32 + 0x80000000LL;
    return (s32)(tmp >> 32);
}

/* Perspective projection matrix (sent to the geometry engine and/or stored) */
void func_02065604(s32 fovySin, s32 fovyCos, s32 aspect, s32 n, s32 f, s32 scaleW, BOOL draw, UnkMtx44 *mtx) {
    s32 fovyCot, m00, m22, m32;
    s64 inv;
    unsigned int fifo = 0;

    fovyCot = FX_Divide(fovyCos, fovySin);
    if (scaleW != 0x1000) {
        fovyCot = fovyCot * scaleW / 0x1000;
    }
    UnkDivAsyncFx(fovyCot, aspect);

    if (draw) {
        G3_WRITE(0x10, 0);
        fifo = 0x16;
    }
    if (mtx != NULL) {
        mtx->_01 = 0;
        mtx->_02 = 0;
        mtx->_03 = 0;
        mtx->_10 = 0;
        mtx->_12 = 0;
        mtx->_13 = 0;
        mtx->_20 = 0;
        mtx->_21 = 0;
        mtx->_23 = -scaleW;
        mtx->_30 = 0;
        mtx->_31 = 0;
        mtx->_33 = 0;
    }

    m00 = FX_GetDivResult();
    UnkDivAsyncFx(0x1000, n - f);
    if (draw) {
        G3_WRITE(fifo, m00);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, fovyCot);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
    }
    if (mtx != NULL) {
        mtx->_00 = m00;
        mtx->_11 = fovyCot;
    }

    inv = FX_GetDivResultFx64c();
    if (scaleW != 0x1000) {
        inv = inv * scaleW / 0x1000;
    }
    m22 = UnkMul32x64c(f + n, inv);
    m32 = UnkMul32x64c(UnkMulFx(n * 2, f), inv);
    if (draw) {
        G3_WRITE(fifo, m22);
        G3_WRITE(fifo, -scaleW);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, m32);
        G3_WRITE(fifo, 0);
    }
    if (mtx != NULL) {
        mtx->_22 = m22;
        mtx->_32 = m32;
    }
}

/* Orthographic projection matrix (sent to the geometry engine and/or stored) */
void func_020657d4(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW, BOOL draw, UnkMtx44 *mtx) {
    s64 invLR, invTB, invNF;
    s32 m00, m11, m22, m30, m31, m32;
    unsigned int fifo = 0;

    FX_InvAsync(r - l);
    if (draw) {
        G3_WRITE(0x10, 0);
        fifo = 0x16;
    }
    if (mtx != NULL) {
        mtx->_01 = 0;
        mtx->_02 = 0;
        mtx->_03 = 0;
        mtx->_10 = 0;
        mtx->_12 = 0;
        mtx->_13 = 0;
        mtx->_20 = 0;
        mtx->_21 = 0;
        mtx->_23 = 0;
        mtx->_33 = scaleW;
    }

    invLR = FX_GetDivResultFx64c();
    UnkDivAsyncFx(0x1000, t - b);
    if (scaleW != 0x1000) {
        invLR = invLR * scaleW / 0x1000;
    }
    m00 = UnkMul32x64c(0x2000, invLR);
    if (draw) {
        G3_WRITE(fifo, m00);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
    }
    if (mtx != NULL) {
        mtx->_00 = m00;
    }

    invTB = FX_GetDivResultFx64c();
    UnkDivAsyncFx(0x1000, n - f);
    if (scaleW != 0x1000) {
        invTB = invTB * scaleW / 0x1000;
    }
    m11 = UnkMul32x64c(0x2000, invTB);
    if (draw) {
        G3_WRITE(fifo, m11);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
        G3_WRITE(fifo, 0);
    }
    if (mtx != NULL) {
        mtx->_11 = m11;
    }

    invNF = FX_GetDivResultFx64c();
    if (scaleW != 0x1000) {
        invNF = invNF * scaleW / 0x1000;
    }
    m22 = UnkMul32x64c(0x2000, invNF);
    if (draw) {
        G3_WRITE(fifo, m22);
        G3_WRITE(fifo, 0);
    }
    if (mtx != NULL) {
        mtx->_22 = m22;
    }

    m30 = UnkMul32x64c(-r - l, invLR);
    m31 = UnkMul32x64c(-t - b, invTB);
    m32 = UnkMul32x64c(f + n, invNF);
    if (draw) {
        G3_WRITE(fifo, m30);
        G3_WRITE(fifo, m31);
        G3_WRITE(fifo, m32);
        G3_WRITE(fifo, scaleW);
    }
    if (mtx != NULL) {
        mtx->_30 = m30;
        mtx->_31 = m31;
        mtx->_32 = m32;
    }
}

/* View matrix from camera position, up vector and target */
void func_02065a98(const UnkVec32 *camPos, const UnkVec32 *camUp, const UnkVec32 *target, BOOL draw, UnkMtx43 *mtx) {
    UnkVec32 vLook, vRight, vUp;
    s32 tx, ty, tz;
    unsigned int fifo = 0;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    func_020630ec(&vLook, &vLook);
    func_02063008(camUp, &vLook, &vRight);
    func_020630ec(&vRight, &vRight);
    func_02063008(&vLook, &vRight, &vUp);

    if (draw) {
        G3_WRITE(0x10, 2);
        fifo = 0x17;
        G3_WRITE(fifo, vRight.x);
        G3_WRITE(fifo, vUp.x);
        G3_WRITE(fifo, vLook.x);
        G3_WRITE(fifo, vRight.y);
        G3_WRITE(fifo, vUp.y);
        G3_WRITE(fifo, vLook.y);
        G3_WRITE(fifo, vRight.z);
        G3_WRITE(fifo, vUp.z);
        G3_WRITE(fifo, vLook.z);
    }

    tx = -func_02062fcc(camPos, &vRight);
    ty = -func_02062fcc(camPos, &vUp);
    tz = -func_02062fcc(camPos, &vLook);
    if (draw) {
        G3_WRITE(fifo, tx);
        G3_WRITE(fifo, ty);
        G3_WRITE(fifo, tz);
    }
    if (mtx != NULL) {
        mtx->_00 = vRight.x;
        mtx->_01 = vUp.x;
        mtx->_02 = vLook.x;
        mtx->_10 = vRight.y;
        mtx->_11 = vUp.y;
        mtx->_12 = vLook.y;
        mtx->_20 = vRight.z;
        mtx->_21 = vUp.z;
        mtx->_22 = vLook.z;
        mtx->_30 = tx;
        mtx->_31 = ty;
        mtx->_32 = tz;
    }
}

/* Multiply the current matrix by a rotation around X */
void func_02065c24(s32 s, s32 c) {
    G3_WRITE(0x1a, 0x1000);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, c);
    G3_WRITE(0x1a, s);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, -s);
    G3_WRITE(0x1a, c);
}

/* Multiply the current matrix by a rotation around Y */
void func_02065c60(s32 s, s32 c) {
    G3_WRITE(0x1a, c);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, -s);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0x1000);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, s);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, c);
}

/* Multiply the current matrix by a rotation around Z */
void func_02065c9c(s32 s, s32 c) {
    G3_WRITE(0x1a, c);
    G3_WRITE(0x1a, s);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, -s);
    G3_WRITE(0x1a, c);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0);
    G3_WRITE(0x1a, 0x1000);
}
