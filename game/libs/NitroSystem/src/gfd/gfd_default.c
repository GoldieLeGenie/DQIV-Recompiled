#include "../nns_internal.h"

/* default texture VRAM allocator: always fails */
u32 func_02072414(u32 szByte, BOOL is4x4comp, u32 opt)
{
    return 0;
}

/* default texture VRAM free: always fails */
int func_0207241c(u32 key)
{
    return -1;
}

/* default palette VRAM allocator: always fails */
u32 func_02072424(u32 szByte, BOOL is4pltt, u32 opt)
{
    return 0;
}

/* default palette VRAM free: always fails */
int func_0207242c(u32 key)
{
    return -1;
}

/* Current VRAM allocator functions (replaced by the linked VRAM managers, gfd_lnktex.c / gfd_lnkpltt.c).
   The definition order gives the ROM data layout (texture alloc, texture free, palette alloc, palette free). */
u32 (*data_020c4044)(u32 szByte, BOOL is4pltt, u32 opt) = func_02072424;
int (*data_020c4040)(u32 key) = func_0207241c;
u32 (*data_020c403c)(u32 szByte, BOOL is4x4comp, u32 opt) = func_02072414;
int (*data_020c4048)(u32 key) = func_0207242c;
