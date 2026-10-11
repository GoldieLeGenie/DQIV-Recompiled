#include <nitro/native_video.h>
#pragma once

// GX functions without a name in the libs headers (the named ones are in nitro/gx.h)
extern "C" {
    int func_020648b8(void);                    // reset the BG VRAM bank (GXState.bg)
    int func_020648cc(void);                    // reset the OBJ VRAM bank (GXState.obj)
}

inline void GX_SetOBJVRamModeBmp(int mode)
{
    *VIDEO_REG(volatile unsigned int, 0x04000000) = (*VIDEO_REG(volatile unsigned int, 0x04000000) & ~0x400060) | mode;
}

inline void GX_SetVisibleWnd(int window)
{
    *VIDEO_REG(volatile unsigned int, 0x04000000) = (*VIDEO_REG(volatile unsigned int, 0x04000000) & ~0xe000) | (window << 13);
}
