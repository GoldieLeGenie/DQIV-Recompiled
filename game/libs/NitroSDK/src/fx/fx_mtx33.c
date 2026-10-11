#include "fx_internal.h"

/* Sets a 3x3 matrix to identity. */
#ifndef DQIV_NATIVE
asm void func_02061b88(register Mtx33* m) {
    mov     r2, #0x1000
    str     r2, [r0, #0x20]
    mov     r3, #0
    stmia   r0!, {r2, r3}
    mov     r1, #0
    stmia   r0!, {r1, r3}
    stmia   r0!, {r2, r3}
    stmia   r0!, {r1, r3}
    bx      lr
}
#endif

/* Scales the rows of a 3x3 matrix by x, y and z. */
void func_02061bac(const Mtx33* src, Mtx33* dst, s32 x, s32 y, s32 z) {
    dst->_00 = (s32)(((s64)x * src->_00) >> 12);
    dst->_01 = (s32)(((s64)x * src->_01) >> 12);
    dst->_02 = (s32)(((s64)x * src->_02) >> 12);

    dst->_10 = (s32)(((s64)y * src->_10) >> 12);
    dst->_11 = (s32)(((s64)y * src->_11) >> 12);
    dst->_12 = (s32)(((s64)y * src->_12) >> 12);

    dst->_20 = (s32)(((s64)z * src->_20) >> 12);
    dst->_21 = (s32)(((s64)z * src->_21) >> 12);
    dst->_22 = (s32)(((s64)z * src->_22) >> 12);
}

#pragma thumb on

/* Rotation around the X axis. */
#ifndef DQIV_NATIVE
asm void func_02061c6c(register Mtx33* m, register s32 sinVal, register s32 cosVal) {
    mov     r3, #1
    lsl     r3, r3, #0xc
    str     r3, [r0, #0]
    mov     r3, #0
    str     r3, [r0, #0x4]
    str     r3, [r0, #0x8]
    str     r3, [r0, #0xc]
    str     r2, [r0, #0x10]
    str     r1, [r0, #0x14]
    str     r3, [r0, #0x18]
    neg     r1, r1
    str     r1, [r0, #0x1c]
    str     r2, [r0, #0x20]
    bx      lr
}
#endif

/* Rotation around the Y axis. */
#ifndef DQIV_NATIVE
asm void func_02061c88(register Mtx33* m, register s32 sinVal, register s32 cosVal) {
    str     r2, [r0, #0]
    str     r2, [r0, #0x20]
    mov     r3, #0
    str     r3, [r0, #0x4]
    str     r3, [r0, #0xc]
    str     r3, [r0, #0x14]
    str     r3, [r0, #0x1c]
    neg     r2, r1
    mov     r3, #1
    lsl     r3, r3, #0xc
    str     r1, [r0, #0x18]
    str     r2, [r0, #0x8]
    str     r3, [r0, #0x10]
    bx      lr
}
#endif

/* Rotation around the Z axis. */
#ifndef DQIV_NATIVE
asm void func_02061ca4(register Mtx33* m, register s32 sinVal, register s32 cosVal) {
    stmia   r0!, {r2}
    mov     r3, #0
    stmia   r0!, {r1, r3}
    neg     r1, r1
    stmia   r0!, {r1, r2}
    mov     r1, #1
    lsl     r1, r1, #0xc
    str     r3, [r0, #0]
    str     r3, [r0, #0x4]
    str     r3, [r0, #0x8]
    str     r1, [r0, #0xc]
    bx      lr
}
#endif

#pragma thumb off

void MTX_Concat33(const Mtx33* a, const Mtx33* b, Mtx33* ab) {
    Mtx33  tmp;
    Mtx33* m;
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
    m->_22 = (s32)(((s64)xa * b->_02 + (s64)ya * b->_12 + (s64)za * b->_22) >> 12);

    if (m == &tmp) {
        *ab = tmp;
    }
}

/* dst = vec * m */
void func_02061edc(const Vec* vec, const Mtx33* m, Vec* dst) {
    s32 x = vec->x;
    s32 y = vec->y;
    s32 z = vec->z;

    dst->x = (s32)(((s64)x * m->_00 + (s64)y * m->_10 + (s64)z * m->_20) >> 12);
    dst->y = (s32)(((s64)x * m->_01 + (s64)y * m->_11 + (s64)z * m->_21) >> 12);
    dst->z = (s32)(((s64)x * m->_02 + (s64)y * m->_12 + (s64)z * m->_22) >> 12);
}
