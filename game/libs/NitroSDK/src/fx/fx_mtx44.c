#include "fx_internal.h"

/* Sets a 4x4 matrix to identity. */
#ifndef DQIV_NATIVE
asm void func_02062740(register Mtx44* m) {
    mov     r2, #0x1000
    mov     r3, #0
    stmia   r0!, {r2, r3}
    mov     r1, #0
    stmia   r0!, {r1, r3}
    stmia   r0!, {r1, r2, r3}
    stmia   r0!, {r1, r3}
    stmia   r0!, {r1, r2, r3}
    stmia   r0!, {r1, r3}
    stmia   r0!, {r1, r2}
    bx      lr
}
#endif

/* Copies a 4x4 matrix into a 4x3 matrix (drops the last column). */
#ifndef DQIV_NATIVE
asm void func_0206276c(register const Mtx44* src, register Mtx43* dst) {
    ldmia   r0!, {r2-r3, r12}
    add     r0, r0, #4
    stmia   r1!, {r2-r3, r12}
    ldmia   r0!, {r2-r3, r12}
    add     r0, r0, #4
    stmia   r1!, {r2-r3, r12}
    ldmia   r0!, {r2-r3, r12}
    add     r0, r0, #4
    stmia   r1!, {r2-r3, r12}
    ldmia   r0!, {r2-r3, r12}
    add     r0, r0, #4
    stmia   r1!, {r2-r3, r12}
    bx      lr
}
#endif

void func_020627a0(const Mtx44* a, const Mtx44* b, Mtx44* ab) {
    Mtx44  tmp;
    Mtx44* m;
    s32    xa, ya, za, wa;
    s32    xb, yb, zb, wb;

    if (ab == b) {
        m = &tmp;
    } else {
        m = ab;
    }

    xa = a->_00;
    ya = a->_01;
    za = a->_02;
    wa = a->_03;

    m->_00 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20 + (s64)wa * b->_30) >> 12);
    m->_01 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    m->_03 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    xb = b->_02;
    yb = b->_12;
    zb = b->_22;
    wb = b->_32;

    m->_02 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_10;
    ya = a->_11;
    za = a->_12;
    wa = a->_13;

    m->_12 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    m->_11 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    m->_13 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    xb = b->_00;
    yb = b->_10;
    zb = b->_20;
    wb = b->_30;

    m->_10 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_20;
    ya = a->_21;
    za = a->_22;
    wa = a->_23;

    m->_20 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    m->_21 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    m->_23 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    xb = b->_02;
    yb = b->_12;
    zb = b->_22;
    wb = b->_32;

    m->_22 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_30;
    ya = a->_31;
    za = a->_32;
    wa = a->_33;

    m->_32 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    m->_31 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    m->_30 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20 + (s64)wa * b->_30) >> 12);
    m->_33 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    if (m == &tmp) {
        *ab = tmp;
    }
}
