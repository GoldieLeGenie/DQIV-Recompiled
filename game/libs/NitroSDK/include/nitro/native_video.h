#ifndef NITRO_NATIVE_VIDEO_H
#define NITRO_NATIVE_VIDEO_H
#include <nitro/types.h>
#ifdef DQIV_NATIVE
#ifdef __cplusplus
extern "C" {
#endif
void* pcVideoRegister(u32 address, u32 width);
void* pcVideoLcdcSource(u32 address, u32 size);
void pcVideoCopy(u32 destination, const void* source, u32 size);
void pcVideoFill(u32 destination, u32 value, u32 size);
#ifdef __cplusplus
}
#endif
#define VIDEO_REG(type, address) ((type*)pcVideoRegister((u32)(address), sizeof(type)))
#define VIDEO_LCDC_SOURCE(address, size) pcVideoLcdcSource((u32)(address), (u32)(size))
#define VIDEO_FILL32(value, address, size) pcVideoFill((u32)(address), (u32)(value), (u32)(size))
#else
#define VIDEO_REG(type, address) ((type*)(address))
#define VIDEO_LCDC_SOURCE(address, size) ((void*)(address))
#define VIDEO_FILL32(value, address, size) MI_CpuFill((value), (void*)(address), (size))
#endif
#endif
