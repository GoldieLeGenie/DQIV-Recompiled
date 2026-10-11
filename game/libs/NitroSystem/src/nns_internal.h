#ifndef CHUNK_F_H
#define CHUNK_F_H

#include <nitro/types.h>

typedef long fx32;
typedef s16 fx16;

typedef struct {
    fx32 x;
    fx32 y;
    fx32 z;
} UnkVecFx32;

/* runtime / SDK functions used by this chunk */
u32 _u32_div_f(u32 a, u32 b);
void MI_CpuFillU16(u16 value, void* dest, u32 size);
void MI_CpuCopyU32(const void* src, void* dest, u32 size);
void DC_PurgeRange(const void* addr, u32 size);
fx32 FX_Divide(fx32 numer, fx32 denom);
void FX_DivAsync(fx32 numer, fx32 denom);
fx32 FX_GetDivResult(void);
void func_020630ec(const UnkVecFx32* src, UnkVecFx32* dst); /* vector normalize */

#endif
