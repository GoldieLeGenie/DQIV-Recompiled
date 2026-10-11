#ifndef CTRDG_INTERNAL_H
#define CTRDG_INTERNAL_H

#include <nitro/types.h>
#include <nitro/os/thread.h>
#include <nitro/os/cpustat.h>
#include <nitro/os/cache.h>
#include <nitro/mi/cpumem.h>

/* Cartridge information kept in the shared work area (0x027FFC30). */
typedef struct UnkCtrdgInfo {
    /* 0x00 */ u16 unk_00; /* game ID (0xFFFF = none) */
    /* 0x02 */ u8  unk_02[3];
    /* 0x05 */ u8  unk_05_0 : 1; /* AGB cartridge */
    /* 0x05 */ u8  unk_05_1 : 1; /* cartridge was pulled out */
    /* 0x05 */ u8  unk_05_2 : 6;
    /* 0x06 */ u16 unk_06; /* maker code */
    /* 0x08 */ u32 unk_08; /* game code */
} UnkCtrdgInfo;

#define CTRDG_INFO ((UnkCtrdgInfo*)0x027FFC30)

/* data_0210c7cc */
typedef struct UnkCtrdgCommon2 {
    /* 0x00 */ vu16 unk_00; /* ARM7 finished reading the header */
    /* 0x02 */ u16  unk_02; /* cartridge lock ID */
} UnkCtrdgCommon2;

/* data_0210c7e4 */
typedef BOOL (*UnkCtrdgPulledOutCallback)(void);
typedef struct UnkCtrdgState {
    /* 0x00 */ u32                       unk_00;
    /* 0x04 */ BOOL                      unk_04; /* CTRDG initialized */
    /* 0x08 */ BOOL                      unk_08; /* pull-out handled */
    /* 0x0C */ u32                       unk_0c[2];
    /* 0x14 */ UnkCtrdgPulledOutCallback unk_14;
} UnkCtrdgState;

/* Saved cartridge lock state. */
typedef struct UnkCtrdgLock {
    /* 0x00 */ u32 unk_00; /* locked by the ARM7 */
    /* 0x04 */ u32 unk_04; /* saved IRQ state */
} UnkCtrdgLock;

/* Saved cartridge access cycles. */
typedef struct UnkCtrdgCycles {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} UnkCtrdgCycles;

/* Cartridge task (0x24 bytes). */
struct UnkCtrdgTask;
typedef u32 (*UnkCtrdgTaskFunc)(struct UnkCtrdgTask* task);
typedef void (*UnkCtrdgTaskCallback)(struct UnkCtrdgTask* task);
typedef struct UnkCtrdgTask {
    /* 0x00 */ UnkCtrdgTaskFunc     unk_00;
    /* 0x04 */ UnkCtrdgTaskCallback unk_04;
    /* 0x08 */ u32                  unk_08; /* result */
    /* 0x0C */ u32                  unk_0c[5];
    /* 0x20 */ u16                  unk_20;
    /* 0x22 */ u8                   unk_22; /* busy */
    /* 0x23 */ u8                   unk_23;
} UnkCtrdgTask;

/* Cartridge task thread (data_0210c8c0, 0xE8 bytes). */
typedef struct UnkCtrdgThread {
    /* 0x00 */ OSThread      unk_00;
    /* 0xC0 */ UnkCtrdgTask* volatile unk_c0; /* current task */
    /* 0xC4 */ UnkCtrdgTask  unk_c4;
} UnkCtrdgThread;

extern BOOL              data_0210c7c8; /* cartridge access enabled */
extern UnkCtrdgCommon2   data_0210c7cc;
extern UnkCtrdgState     data_0210c7e4;
/* Copy of the AGB cartridge header (data_0210c800, 0xC0 bytes). */
typedef struct UnkCtrdgAgbHeader {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u8  unk_04[0x9C]; /* logo */
    /* 0xA0 */ u8  unk_a0[0x0C]; /* title */
    /* 0xAC */ u32 unk_ac;       /* game code */
    /* 0xB0 */ u16 unk_b0;       /* maker code */
    /* 0xB2 */ u8  unk_b2;       /* fixed 0x96 */
    /* 0xB3 */ u8  unk_b3[2];
    /* 0xB5 */ u8  unk_b5[3];
    /* 0xB8 */ u8  unk_b8[6];
    /* 0xBE */ u16 unk_be;
} UnkCtrdgAgbHeader;

extern UnkCtrdgAgbHeader data_0210c800;
extern UnkCtrdgThread    data_0210c8c0;
extern UnkCtrdgThread*   data_0210c9a8; /* task thread */
extern UnkCtrdgTask      data_0210c9ac;

/* external */
void OS_Terminate(void);
u32  OS_GetLockID(void);
void CpuSet(const void* src, void* dst, u32 control);
void WaitByLoop(s32 count);
s32  PXI_SendWordByFifo(u32 tag, u32 data, BOOL err);
void func_0207a074(void);
void func_0207a180(u32 tag, void (*callback)(u32 tag, u32 data, BOOL err));
BOOL func_0207a1cc(u32 tag, BOOL arm7);
u32  func_02077a0c(void* lockWord);
s32  func_02077954(u16 lockId);
void func_02077948(u16 lockId);
void func_020798f4(u32 mask, u32 flags);
u32  func_02077624(u32 mask);
void func_020671bc(u32 dma, const void* src, void* dst, u32 size);
void func_020787c4(OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 prio);
void func_020788c0(void);

/* ctrdg_common.c */
void func_0205e93c(void);
BOOL func_0205e974(void);
BOOL func_0205e99c(void);
BOOL func_0205e9b4(void);
void func_0205eac0(UnkCtrdgCycles* cycles);
void func_0205eb08(const UnkCtrdgCycles* cycles);
void func_0205eb3c(u16 lockId, UnkCtrdgLock* lock);
void func_0205eb98(u16 lockId, UnkCtrdgLock* lock);
void func_0205ebbc(u32 data);
void func_0205ec0c(BOOL enable);
void func_0205ec54(void);
void func_0205ed08(void);
void func_0205eefc(u32 tag, u32 data, BOOL err);
void func_0205ef28(u32 tag, u32 data, BOOL err);
void func_0205ef84(void);
void func_0205ef98(u32 tag, u32 data, BOOL err);

/* ctrdg_task.c */
void func_0205efac(UnkCtrdgThread* thread);
void func_0205f038(UnkCtrdgTask* task);
void func_0205f04c(void* arg);

#endif
