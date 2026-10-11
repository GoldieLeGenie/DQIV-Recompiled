#include "fx_internal.h"

/* Sets a 4x3 matrix to identity. */
#ifndef DQIV_NATIVE
asm void func_02061f58(register Mtx43* m) {
    mov     r2, #0x1000
    mov     r3, #0
    stmia   r0!, {r2, r3}
    mov     r1, #0
    stmia   r0!, {r1, r3}
    stmia   r0!, {r2, r3}
    stmia   r0!, {r1, r3}
    stmia   r0!, {r2, r3}
    stmia   r0!, {r1, r3}
    bx      lr
}
#endif

/* Copies a 4x3 matrix into a 4x4 matrix (last column 0, 0, 0, 1). */
#ifndef DQIV_NATIVE
asm void func_02061f80(register const Mtx43* src, register Mtx44* dst) {
    stmdb   sp!, {r4}
    mov     r12, #0
    ldmia   r0!, {r2-r4}
    stmia   r1!, {r2-r4, r12}
    ldmia   r0!, {r2-r4}
    stmia   r1!, {r2-r4, r12}
    ldmia   r0!, {r2-r4}
    stmia   r1!, {r2-r4, r12}
    mov     r12, #0x1000
    ldmia   r0!, {r2-r4}
    stmia   r1!, {r2-r4, r12}
    ldmia   sp!, {r4}
    bx      lr
}
#endif

/* Scales the rows of the rotation part of a 4x3 matrix, keeps the translation. */
void func_02061fb4(const Mtx43* src, Mtx43* dst, s32 x, s32 y, s32 z) {
    func_02061bac((const Mtx33*)src, (Mtx33*)dst, x, y, z);
    dst->_30 = src->_30;
    dst->_31 = src->_31;
    dst->_32 = src->_32;
}

#pragma thumb on

/* Rotation around the X axis. */
#ifndef DQIV_NATIVE
asm void func_02061fe8(register Mtx43* m, register s32 sinVal, register s32 cosVal) {
    str     r1, [r0, #0x14]
    neg     r1, r1
    str     r1, [r0, #0x1c]
    mov     r1, #1
    lsl     r1, r1, #0xc
    stmia   r0!, {r1}
    mov     r3, #0
    mov     r1, #0
    stmia   r0!, {r1, r3}
    stmia   r0!, {r1, r2}
    str     r1, [r0, #0x4]
    add     r0, #0xc
    stmia   r0!, {r2, r3}
    stmia   r0!, {r1, r3}
    bx      lr
}
#endif

/* Rotation around the Y axis. */
#ifndef DQIV_NATIVE
asm void func_02062008(register Mtx43* m, register s32 sinVal, register s32 cosVal) {
    str     r1, [r0, #0x18]
    mov     r3, #0
    stmia   r0!, {r2, r3}
    neg     r1, r1
    stmia   r0!, {r1, r3}
    mov     r1, #1
    lsl     r1, r1, #0xc
    stmia   r0!, {r1, r3}
    add     r0, #4
    mov     r1, #0
    stmia   r0!, {r1, r2, r3}
    stmia   r0!, {r1, r3}
    bx      lr
}
#endif

/* Rotation around the Z axis. */
#ifndef DQIV_NATIVE
asm void func_02062024(register Mtx43* m, register s32 sinVal, register s32 cosVal) {
    stmia   r0!, {r2}
    mov     r3, #0
    stmia   r0!, {r1, r3}
    neg     r1, r1
    stmia   r0!, {r1, r2, r3}
    mov     r1, #0
    mov     r2, #0
    mov     r3, #1
    lsl     r3, r3, #0xc
    stmia   r0!, {r1, r2, r3}
    mov     r3, #0
    stmia   r0!, {r1, r2, r3}
    bx      lr
}
#endif

#pragma thumb off

/* Inverse of a 4x3 matrix; returns -1 if it is singular. */
s32 func_02062040(const Mtx43* src, Mtx43* dst) {
    Mtx43  tmp;
    Mtx43* m;
    s32    det0, det1, det2;
    s32    det;
    s32    t01, t02, t11, t12;

    if (src == dst) {
        m = &tmp;
    } else {
        m = dst;
    }

    det0 = (s32)(((s64)src->_11 * src->_22 - (s64)src->_12 * src->_21 + (s64)(0x1000 >> 1)) >> 12);
    det1 = (s32)(((s64)src->_10 * src->_22 - (s64)src->_12 * src->_20 + (s64)(0x1000 >> 1)) >> 12);
    det2 = (s32)(((s64)src->_10 * src->_21 - (s64)src->_11 * src->_20 + (s64)(0x1000 >> 1)) >> 12);
    det  = (s32)(((s64)src->_00 * det0 - (s64)src->_01 * det1 + (s64)src->_02 * det2 + (s64)(0x1000 >> 1)) >> 12);

    if (det == 0) {
        return -1;
    }

    FX_InvAsync(det);

    t01 = (s32)(((s64)src->_01 * src->_22 - (s64)src->_21 * src->_02) >> 12);
    t02 = (s32)(((s64)src->_01 * src->_12 - (s64)src->_11 * src->_02) >> 12);
    t11 = (s32)(((s64)src->_00 * src->_22 - (s64)src->_20 * src->_02) >> 12);
    t12 = (s32)(((s64)src->_00 * src->_12 - (s64)src->_10 * src->_02) >> 12);

    det = FX_GetDivResult();

    m->_00 = (s32)(((s64)det * det0) >> 12);
    m->_01 = -(s32)(((s64)det * t01) >> 12);
    m->_02 = (s32)(((s64)det * t02) >> 12);

    m->_10 = -(s32)(((s64)det * det1) >> 12);
    m->_11 = (s32)(((s64)det * t11) >> 12);
    m->_12 = -(s32)(((s64)det * t12) >> 12);

    m->_20 = (s32)(((s64)det * det2) >> 12);
    m->_21 = -(s32)(((s64)det * (s32)(((s64)src->_00 * src->_21 - (s64)src->_20 * src->_01) >> 12)) >> 12);
    m->_22 = (s32)(((s64)det * (s32)(((s64)src->_00 * src->_11 - (s64)src->_10 * src->_01) >> 12)) >> 12);

    m->_30 = -(s32)(((s64)m->_00 * src->_30 + (s64)m->_10 * src->_31 + (s64)m->_20 * src->_32) >> 12);
    m->_31 = -(s32)(((s64)m->_01 * src->_30 + (s64)m->_11 * src->_31 + (s64)m->_21 * src->_32) >> 12);
    m->_32 = -(s32)(((s64)m->_02 * src->_30 + (s64)m->_12 * src->_31 + (s64)m->_22 * src->_32) >> 12);

    if (m == &tmp) {
        MI_CpuCopy48(&tmp, dst);
    }
    return 0;
}

void func_020623cc(const Mtx43* a, const Mtx43* b, Mtx43* ab) {
    Mtx43  tmp;
    Mtx43* m;
    s32    xa, ya, za;
    s32    xb, yb, zb;

    if (ab == b) {
        m = &tmp;
    } else {
        m = ab;
    }

    xa = a->_00;
    ya = a->_01;
    za = a->_02;

    m->_00 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20) >> 12);
    m->_01 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);

    xb = b->_02;
    yb = b->_12;
    zb = b->_22;

    m->_02 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_10;
    ya = a->_11;
    za = a->_12;

    m->_12 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);
    m->_11 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);

    xb = b->_00;
    yb = b->_10;
    zb = b->_20;

    m->_10 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_20;
    ya = a->_21;
    za = a->_22;

    m->_20 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);
    m->_21 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);

    xb = b->_02;
    yb = b->_12;
    zb = b->_22;

    m->_22 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_30;
    ya = a->_31;
    za = a->_32;

    m->_32 = (s32)((((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12) + b->_32);
    m->_31 = (s32)((((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12) + b->_31);
    m->_30 = (s32)((((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20) >> 12) + b->_30);

    if (m == &tmp) {
        *ab = tmp;
    }
}

/* dst = vec * m (with translation) */
void func_020626a0(const Vec* vec, const Mtx43* m, Vec* dst) {
    s32 x = vec->x;
    s32 y = vec->y;
    s32 z = vec->z;

    dst->x = (s32)(((s64)x * m->_00 + (s64)y * m->_10 + (s64)z * m->_20) >> 12);
    dst->x += m->_30;
    dst->y = (s32)(((s64)x * m->_01 + (s64)y * m->_11 + (s64)z * m->_21) >> 12);
    dst->y += m->_31;
    dst->z = (s32)(((s64)x * m->_02 + (s64)y * m->_12 + (s64)z * m->_22) >> 12);
    dst->z += m->_32;
}
