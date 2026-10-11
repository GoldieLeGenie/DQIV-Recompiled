#include "fx_internal.h"

/* Sets a 2x2 matrix to identity. */
#ifndef DQIV_NATIVE
asm void func_02061b70(register UnkMtx22* m) {
    mov     r1, #0
    mov     r2, #0x1000
    mov     r3, #0
    stmia   r0!, {r2, r3}
    stmia   r0!, {r1, r2}
    bx      lr
}
#endif
