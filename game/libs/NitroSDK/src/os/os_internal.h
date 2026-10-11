#ifndef H_OS_H_H
#define H_OS_H_H

#include <nitro/types.h>
#include <nitro/reg.h>
#include <nitro/os/cpustat.h>
#include <stdarg.h>

// ---------------------------------------------------------------- externals
void WaitByLoop(s32 count);                                  // 0x02000074 (thumb, svc)
void func_0206785c(u32 data, void* dst, u32 size);           // 32-bit fill
s32  func_02067b80(u32 value, void* dst);                    // atomic word swap
void CP_SaveContext(void* context);                          // 0x0205e8c0
void CPi_RestoreContext(void* context);                      // 0x0205e900
void OS_Terminate(void);                                     // 0x0207a058
void func_0207a068(void);                                    // halt

// ---------------------------------------------------------------- interrupts
typedef void (*UnkIrqFunction)(void);

typedef struct UnkIrqCallbackInfo {
    /* 0x00 */ void (*func)(void*);
    /* 0x04 */ u32   enable;
    /* 0x08 */ void* arg;
} UnkIrqCallbackInfo; // size 0xc

typedef struct UnkThreadQueue UnkThreadQueue;
struct UnkThreadQueue {
    void* head;
    void* tail;
};

extern UnkIrqFunction     data_027e0000[22];  // DTCM IRQ handler table
extern UnkThreadQueue     data_027e0060;      // IRQ thread queue (DTCM)
extern UnkIrqCallbackInfo data_02114140[8];   // DMA (0..3) + timer (4..7) callbacks

void OS_InitIRQQueue(void);
void func_02077480(u32 mask, UnkIrqFunction func);
UnkIrqFunction func_02077508(u32 mask);
void func_02077594(u32 dmaNo, void (*func)(void*), void* arg);
void func_020775dc(u32 timerNo, void (*func)(void*), void* arg);
u32  func_02077624(u32 mask);
u32  func_02077650(u32 mask);
u32  func_02077680(u32 mask);
u32  func_020776b0(u32 mask);
void func_020776dc(void);

// ---------------------------------------------------------------- spin locks
typedef struct UnkLockWord {
    /* 0x00 */ u32 lockFlag;
    /* 0x04 */ u16 ownerID;
    /* 0x06 */ u16 extension;
} UnkLockWord;

void func_02077710(void);
s32  func_020777dc(u16 lockID, UnkLockWord* lockp, void (*ctrlFunc)(void), BOOL disableFiq);
s32  func_02077828(u16 lockID, UnkLockWord* lockp, void (*ctrlFunc)(void));
s32  func_02077838(u16 lockID, UnkLockWord* lockp, void (*ctrlFunc)(void), BOOL disableFiq);
s32  func_020778ac(u16 lockID, UnkLockWord* lockp, void (*ctrlFunc)(void));
s32  func_020778bc(u16 lockID, UnkLockWord* lockp, void (*ctrlFunc)(void), BOOL disableFiq);
s32  func_02077928(u16 lockID);
s32  func_02077948(u16 lockID);
s32  func_02077954(u16 lockID);
void func_02077974(void);
void func_0207798c(void);
s32  func_020779a4(u16 lockID);
s32  func_020779c0(u16 lockID);
void func_020779dc(void);
void func_020779f4(void);
u16  func_02077a0c(UnkLockWord* lockp);
s32  OS_GetLockID(void);

#endif
