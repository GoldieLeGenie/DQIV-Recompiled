#ifndef NITRO_NATIVE_G3_H
#define NITRO_NATIVE_G3_H
#include <nitro/types.h>
#ifdef DQIV_NATIVE
#ifdef __cplusplus
extern "C" {
#endif
void pcG3Write(unsigned int command, u32 value);
void pcG3Send(const void* words, u32 size);
#ifdef __cplusplus
}
#endif
#define G3_WRITE(command, value) pcG3Write((command), (u32)(value))
#define G3_SEND(words, size) pcG3Send((words), (size))
#else
#define G3_WRITE(command, value) (*(volatile u32*)(0x04000400 + (command)*4) = (value))
#define G3_SEND(words, size) MI_CpuFillFromSrc((words), (volatile void*)0x04000400, (size))
#endif
#endif
