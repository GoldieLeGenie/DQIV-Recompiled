#include "g3d_internal.h"

static inline fx32 Mul32x64c(fx32 v32, fx64 v64c) {
    fx64 tmp = v64c * v32 + 0x80000000LL;
    return (fx32)(tmp >> 32);
}

// Initializes the 3D engine and the G3D global state
void func_0206dfa4(void) {
    func_02065088();
    func_0206ac78();
#ifndef DQIV_NATIVE
    // Hardware FIFO interrupt selection; native command submission is synchronous.
    *(vu32*)0x04000600 = (*(vu32*)0x04000600 & ~0xc0000000) | 0x80000000;
#endif
}

#ifdef DQIV_NATIVE
const int* pcScreenProjection(const int* current);
#endif
// Converts a world position to a screen position, returns -1 when outside of the view volume
int func_0206dfcc(const VecFx32* world, int* px, int* py) {
    int ret;
    int x1;
    int dx;
    int y1;
    fx32 w;
    fx32 x;
    const MtxFx44* proj;
    VecFx32 vec;
    int dy;
    int x2;
    fx64 inv;
    int y2;
    fx32 y;

    proj = &data_0210cf28.projMtx;
#ifdef DQIV_NATIVE
    proj = (const MtxFx44*)pcScreenProjection((const int*)proj);
#endif
    func_020626a0(world, &data_0210cf28.cameraMtx, &vec);
    {
        fx64 tw = (fx64)vec.x * proj->_03 + (fx64)vec.y * proj->_13 + (fx64)vec.z * proj->_23;
        w = (fx32)(tw >> 12) + proj->_33;
    }
    FX_InvAsync(w);
    {
        fx64 tx = (fx64)vec.x * proj->_00 + (fx64)vec.y * proj->_10 + (fx64)vec.z * proj->_20;
        fx64 ty = (fx64)vec.x * proj->_01 + (fx64)vec.y * proj->_11 + (fx64)vec.z * proj->_21;
        x = (fx32)(tx >> 12) + proj->_30;
        y = (fx32)(ty >> 12) + proj->_31;
    }
    inv = FX_GetDivResultFx64c();
    x = (Mul32x64c(x, inv) + FX32_ONE) / 2;
    y = (Mul32x64c(y, inv) + FX32_ONE) / 2;

    ret = 0;
    if (x < 0 || y < 0 || x > FX32_ONE || y > FX32_ONE) {
        ret = -1;
    }

    func_0206b294(&x1, &y1, &x2, &y2);
    dx = x2 - x1;
    dy = y2 - y1;
    *px = x1 + ((x * dx + 0x800) >> 12);
    *py = 191 - y1 - ((y * dy + 0x800) >> 12);
    return ret;
}

// Converts a screen position to a line in world space
int func_0206e154(int px, int py, VecFx32* pNear, VecFx32* pFar) {
    int x1, y1, x2, y2;
    fx32 x, y;
    int ret;
    const MtxFx44* m;
    fx32 t;
    fx32 nx, ny, nz;
    fx32 fw, fx, fy, fz;
    fx64 inv;
    int h;

    func_0206b294(&x1, &y1, &x2, &y2);
    h = y2 - y1;
    x = FX_Divide((px - x1) << 12, (x2 - x1) << 12);
    y = FX_Divide((py + y1 - 191) << 12, -h << 12);

    if (x < 0 || y < 0 || x > FX32_ONE || y > FX32_ONE) {
        ret = -1;
    } else {
        ret = 0;
    }

    x = (x - 0x800) * 2;
    y = (y - 0x800) * 2;

    m = func_0206b22c();
    t = (fx32)(((fx64)x * m->_03 + (fx64)y * m->_13) >> 12) + m->_33;
    FX_InvAsync(t - m->_23);
    nx = (fx32)(((fx64)x * m->_00 + (fx64)y * m->_10) >> 12) + m->_30;
    ny = (fx32)(((fx64)x * m->_01 + (fx64)y * m->_11) >> 12) + m->_31;
    nz = (fx32)(((fx64)x * m->_02 + (fx64)y * m->_12) >> 12) + m->_32;

    if (pFar) {
        fx = nx + m->_20;
        fy = ny + m->_21;
        fz = nz + m->_22;
        fw = t + m->_23;
    }
    nx -= m->_20;
    ny -= m->_21;
    nz -= m->_22;

    inv = FX_GetDivResultFx64c();
    if (pFar) {
        FX_InvAsync(fw);
    }
    pNear->x = Mul32x64c(nx, inv);
    pNear->y = Mul32x64c(ny, inv);
    pNear->z = Mul32x64c(nz, inv);

    if (pFar) {
        inv = FX_GetDivResultFx64c();
        pFar->x = Mul32x64c(fx, inv);
        pFar->y = Mul32x64c(fy, inv);
        pFar->z = Mul32x64c(fz, inv);
    }
    return ret;
}

void* func_0206e3d8(void* allocator) {
    return func_02068a1c(allocator, sizeof(NNSG3dRenderObj));
}

void func_0206e3e8(void* allocator, NNSG3dRenderObj* obj) {
    func_02068a30(allocator, obj);
}

void* func_0206e3f4(void* allocator, const void* anm, const NNSG3dResMdl* mdl) {
    return func_02068a1c(allocator, func_0206a26c(anm, mdl));
}

void func_0206e418(void* allocator, NNSG3dAnmObj* obj) {
    func_02068a30(allocator, obj);
}
