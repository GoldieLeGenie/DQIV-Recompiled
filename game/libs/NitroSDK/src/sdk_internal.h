#ifndef SDK_INTERNAL_H
#define SDK_INTERNAL_H

#include <nitro/types.h>

/* ---- external functions (names from config/eur/arm9/symbols.txt) ---- */
u32 OS_DisableIRQ(void);
u32 OS_RestoreIRQ(u32 state);
u32 OS_GetIRQFlag(void);
void OS_Delay(u32 count);
void OS_Terminate(void);
void func_0207a068(void);
u32 func_02079c90(void);

typedef struct UnkMutex {
    u32 unk_00[6];
} UnkMutex;
void OS_InitMutex(UnkMutex* mutex);
void OS_LockMutex(UnkMutex* mutex);
void OS_UnlockMutex(UnkMutex* mutex);

typedef struct UnkMessageQueue {
    u32 unk_00[8];
} UnkMessageQueue;
void func_02078d60(UnkMessageQueue* queue, void* buf, s32 count);
BOOL func_02078d88(UnkMessageQueue* queue, void* msg, s32 flags);
BOOL func_02078e1c(UnkMessageQueue* queue, void* msg, s32 flags);
BOOL func_02078ec0(UnkMessageQueue* queue, void* msg, s32 flags);
BOOL func_02078d40(void);

void func_020776b0(u32 mask);
void func_02077480(u32 mask, void* handler);
u32 func_02077650(u32 mask);
u32 func_02077680(u32 mask);
u32 func_02077624(u32 mask);

void DC_InvalidateRange(void* ptr, u32 size);
void DC_CleanRange(void* ptr, u32 size);
void DC_PurgeRange(void* ptr, u32 size);

void MI_CpuCopyU8(const void* src, void* dest, u32 size);
void MI_CpuCopyU16(const void* src, void* dest, u32 size);
void MI_CpuCopyU32(const void* src, void* dest, u32 size);
void MI_CpuSet(void* dest, u8 value, u32 size);
void MI_CpuFillU16(u16 value, void* dest, u32 size);
void func_0206785c(u32 value, void* dest, u32 size);
void MI_CpuFill(u32 value, void* dest, u32 size);
void func_020670cc(u32 dmaNo, void* dest, u32 value, u32 size);
u8 func_02066df0(u32 value);
BOOL func_0205e9b4(void);

/* ---- PXI ---- */
typedef void (*PxiCallback)(u32 tag, u32 data, BOOL err);
void func_0207a074(void);
void func_0207a080(void);
void func_0207a180(u32 tag, PxiCallback callback);
BOOL func_0207a1cc(u32 tag, u32 proc);
s32 PXI_SendWordByFifo(u32 tag, u32 data, BOOL err);
void PXIi_HandlerRecvFifoNotEmpty(void);

#endif
