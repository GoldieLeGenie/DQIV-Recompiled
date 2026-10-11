#include "fx_internal.h"

void func_02062f64(const Vec* a, const Vec* b, Vec* ab) {
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

void func_02062f98(const Vec* a, const Vec* b, Vec* ab) {
    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}

/* Dot product. */
s32 func_02062fcc(const Vec* a, const Vec* b) {
    return (s32)(((s64)a->x * b->x + (s64)a->y * b->y + (s64)a->z * b->z + (1 << 11)) >> 12);
}

/* Cross product. */
void func_02063008(const Vec* a, const Vec* b, Vec* axb) {
    s32 x = (s32)(((s64)a->y * b->z - (s64)a->z * b->y + (1 << 11)) >> 12);
    s32 y = (s32)(((s64)a->z * b->x - (s64)a->x * b->z + (1 << 11)) >> 12);
    s32 z = (s32)(((s64)a->x * b->y - (s64)a->y * b->x + (1 << 11)) >> 12);

    axb->x = x;
    axb->y = y;
    axb->z = z;
}

/* Length of a vector. */
#ifndef DQIV_NATIVE
s32 func_0206308c(const Vec* v) {
    s64 t = (s64)v->x * v->x;
    t += (s64)v->y * v->y;
    t += (s64)v->z * v->z;
    t <<= 2;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = t;

    while (REG_SQRT_CNT & 0x8000) {
    }
    return ((s32)REG_SQRT_RESULT + 1) >> 1;
}
#endif

/* Normalizes a vector. */
#ifndef DQIV_NATIVE
void func_020630ec(const Vec* src, Vec* dst) {
    s64 t = (s64)src->x * src->x;
    s32 sqrt;
    s64 div;

    t += (s64)src->y * src->y;
    t += (s64)src->z * src->z;

    REG_DIV_CNT   = 2;
    REG_DIV_NUMER = 1LL << 56;
    REG_DIV_DENOM = t;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = t << 2;

    while (REG_SQRT_CNT & 0x8000) {
    }
    sqrt = (s32)REG_SQRT_RESULT;

    while (REG_DIV_CNT & 0x8000) {
    }
    div = (s64)REG_DIV_RESULT;

    div *= sqrt;

    dst->x = (s32)((div * src->x + (1LL << 44)) >> 45);
    dst->y = (s32)((div * src->y + (1LL << 44)) >> 45);
    dst->z = (s32)((div * src->z + (1LL << 44)) >> 45);
}
#endif

/* Normalizes a 16-bit vector. */
#ifndef DQIV_NATIVE
void func_02063204(const UnkVecFx16* src, UnkVecFx16* dst) {
    s64 t = src->x * src->x;
    s32 sqrt;
    s64 div;

    t += src->y * src->y;
    t += src->z * src->z;

    REG_DIV_CNT   = 2;
    REG_DIV_NUMER = 1LL << 56;
    REG_DIV_DENOM = t;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = t << 2;

    while (REG_SQRT_CNT & 0x8000) {
    }
    sqrt = (s32)REG_SQRT_RESULT;

    while (REG_DIV_CNT & 0x8000) {
    }
    div = (s64)REG_DIV_RESULT;

    div *= sqrt;

    dst->x = (s16)((div * src->x + (1LL << 44)) >> 45);
    dst->y = (s16)((div * src->y + (1LL << 44)) >> 45);
    dst->z = (s16)((div * src->z + (1LL << 44)) >> 45);
}
#endif

/* dst = a * v1 + v2 */
void func_02063330(s32 a, const Vec* v1, const Vec* v2, Vec* dst) {
    dst->x = v2->x + (s32)(((s64)a * v1->x) >> 12);
    dst->y = v2->y + (s32)(((s64)a * v1->y) >> 12);
    dst->z = v2->z + (s32)(((s64)a * v1->z) >> 12);
}

/* Distance between two points. */
#ifndef DQIV_NATIVE
s32 func_0206338c(const Vec* a, const Vec* b) {
    s64 t;
    s32 d;

    d = a->x - b->x;
    t = (s64)d * d;
    d = a->y - b->y;
    t += (s64)d * d;
    d = a->z - b->z;
    t += (s64)d * d;
    t <<= 2;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = t;

    while (REG_SQRT_CNT & 0x8000) {
    }
    return ((s32)REG_SQRT_RESULT + 1) >> 1;
}
#endif
