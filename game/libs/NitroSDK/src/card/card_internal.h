#ifndef CARD_INTERNAL_H
#define CARD_INTERNAL_H

#include <nitro/types.h>
#include <nitro/os/thread.h>
#include <nitro/os/cpustat.h>
#include <nitro/os/cache.h>
#include <nitro/mi/cpumem.h>

/* Command block shared with the ARM7 (data_0210bf00, 0x60 bytes). */
typedef struct UnkCardCmd {
    /* 0x00 */ u32 unk_00; /* result */
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0c; /* source */
    /* 0x10 */ u32 unk_10; /* destination */
    /* 0x14 */ u32 unk_14; /* length */
    /* 0x18 */ u32 unk_18; /* backup total size */
    /* 0x1C */ u32 unk_1c; /* backup sector size */
    /* 0x20 */ u32 unk_20; /* backup page size */
    /* 0x24 */ u32 unk_24; /* backup address width */
    /* 0x28 */ u32 unk_28;
    /* 0x2C */ u32 unk_2c;
    /* 0x30 */ u32 unk_30;
    /* 0x34 */ u32 unk_34;
    /* 0x38 */ u32 unk_38;
    /* 0x3C */ u32 unk_3c;
    /* 0x40 */ u32 unk_40;
    /* 0x44 */ u32 unk_44;
    /* 0x48 */ u8  unk_48; /* initial status */
    /* 0x49 */ u8  unk_49[3];
    /* 0x4C */ u32 unk_4c; /* supported command mask */
    /* 0x50 */ u8  unk_50[0x10];
} UnkCardCmd; /* 0x60 */

struct UnkCardCommon;
typedef void (*UnkCardTask)(struct UnkCardCommon* common);
typedef void (*UnkCardCallback)(void* arg);

/* Card library state (data_0210bf60). */
typedef struct UnkCardCommon {
    /* 0x000 */ UnkCardCmd*     unk_000; /* -> data_0210bf00 */
    /* 0x004 */ u32             unk_004; /* current request */
    /* 0x008 */ vs32            unk_008; /* lock owner (-3 = none) */
    /* 0x00C */ vs32            unk_00c; /* lock count */
    /* 0x010 */ OSThreadQueue   unk_010; /* lock wait queue */
    /* 0x018 */ u32             unk_018; /* locked target (1 = rom, 2 = backup) */
    /* 0x01C */ u32             unk_01c; /* source */
    /* 0x020 */ u32             unk_020; /* destination */
    /* 0x024 */ u32             unk_024; /* length */
    /* 0x028 */ u32             unk_028; /* dma channel */
    /* 0x02C */ u32             unk_02c; /* backup request */
    /* 0x030 */ u32             unk_030; /* retry count */
    /* 0x034 */ u32             unk_034; /* backup copy mode */
    /* 0x038 */ UnkCardCallback unk_038; /* completion callback */
    /* 0x03C */ void*           unk_03c; /* callback argument */
    /* 0x040 */ UnkCardTask     unk_040; /* task for the card thread */
    /* 0x044 */ OSThread        unk_044; /* card thread */
    /* 0x104 */ OSThread*       unk_104; /* thread waiting for the ARM7 */
    /* 0x108 */ u32             unk_108; /* card thread priority */
    /* 0x10C */ OSThreadQueue   unk_10c; /* busy wait queue */
    /* 0x114 */ vu32            unk_114; /* flags */
    /* 0x118 */ u32             unk_118[2];
    /* 0x120 */ u8              unk_120[0x100]; /* backup bounce buffer */
} UnkCardCommon; /* 0x220 */

/* ROM read state (data_0210c5a0). */
typedef struct UnkCardRom {
    /* 0x000 */ void (*unk_000)(struct UnkCardRom* rom); /* read routine */
    /* 0x004 */ u32 unk_004;                              /* card control value */
    /* 0x008 */ u32 unk_008;                              /* cached page */
    /* 0x00C */ u32 unk_00c[5];
    /* 0x020 */ u32 unk_020[0x80];                        /* page cache */
} UnkCardRom; /* 0x220 */

extern UnkCardCmd     data_0210bf00;
extern UnkCardCommon  data_0210bf60;
extern u32            data_0210c580; /* ROM base offset */
extern UnkCardRom     data_0210c5a0;
extern BOOL           data_0210bee0; /* card access enabled */

/* "[SDK+NINTENDO:BACKUP]" marker string inside BuildInfo (no relocation in the ROM) */
#define CARD_BACKUP_MARKER ((const char*)0x02000B88)

/* external (os / mi / pxi) */
void OS_Terminate(void);
u32  OS_GetLockID(void);
void func_02000b60(const char* marker);
void func_020787c4(OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 prio);
void func_02078af4(OSThread* thread, u32 prio);
void func_0207a180(u32 tag, void (*callback)(u32 tag, u32 data, BOOL err));
BOOL func_0207a1cc(u32 tag, BOOL arm7);
s32  PXI_SendWordByFifo(u32 tag, u32 data, BOOL err);
void func_0207a074(void);
s32  func_020779a4(u16 lockId);
s32  func_020779c0(u16 lockId);
void func_020673ec(u32 dma);
u32  func_020798c0(void);
void func_02067d60(u32 dma, const void* src, void* dst, u32 size);
void func_02077480(u32 mask, void (*handler)(void));
u32  func_02077650(u32 mask);
u32  func_02077680(u32 mask);
u32  func_020776b0(u32 mask);
void WaitByLoop(s32 count);
u32  func_0207bd04(void);

/* Waits until the card thread is idle and marks it busy. */
static inline void CARDi_WaitAndLock(UnkCardCommon* p, UnkCardCallback callback, void* arg) {
    ENTER_CRITICAL_SECTION();
    while (p->unk_114 & 4) {
        OS_PauseThread(&p->unk_10c);
    }
    p->unk_114 |= 4;
    p->unk_038 = callback;
    p->unk_03c = arg;
    LEAVE_CRITICAL_SECTION();
}

/* Marks the card thread idle, wakes waiters and runs the completion callback. */
static inline void CARDi_EndTask(UnkCardCommon* p) {
    const UnkCardCallback callback = p->unk_038;
    void* const           arg      = p->unk_03c;
    {
        ENTER_CRITICAL_SECTION();
        p->unk_114 &= ~0x4C;
        OS_UnpauseThread(&p->unk_10c);
        if (p->unk_114 & 0x10) {
            OS_WakeupThreadDirect(&p->unk_044);
        }
        LEAVE_CRITICAL_SECTION();
    }
    if (callback != NULL) {
        callback(arg);
    }
}

/* card_common.c */
void func_0205d3bc(UnkCardTask task);
void func_0205d3f8(u16 lockId, u32 target);
void func_0205d47c(u16 lockId, u32 target);
void func_0205d508(void);
BOOL func_0205d60c(void);
void func_0205d61c(void);
void func_0205d634(BOOL enable);
BOOL func_0205d644(void);
BOOL func_0205d690(void);
u32  func_0205d6ac(void);
void CARD_LockRom(u16 lockId);
void CARD_UnlockRom(u16 lockId);
void func_0205d6f8(u16 lockId);
void func_0205d708(u16 lockId);

/* card_backup.c */
void func_0205d718(u32 type);
void func_0205d9a0(UnkCardCommon* common);
BOOL func_0205db78(u32 src, u32 dst, u32 len, UnkCardCallback callback, void* arg, BOOL async, u32 req, u32 retry, u32 mode);
u32  func_0205dc60(void);
BOOL func_0205dc74(u32 type);
BOOL func_0205ddac(void);

/* card_rom.c */
BOOL func_0205ddb8(UnkCardRom* rom);
void func_0205de44(u32 hi, u32 lo);
void func_0205dea4(void);
void func_0205def8(void);
BOOL func_0205dfc8(UnkCardRom* rom);
void func_0205e12c(UnkCardRom* rom);
u32  func_0205e214(void);
void func_0205e270(UnkCardCommon* common);
void CARDi_ReadRom(u32 dma, const void* src, void* dst, u32 len, UnkCardCallback callback, void* arg, BOOL async);
void CARD_Init(void);
BOOL func_0205e464(void);
void* func_0205e470(void);

/* card_request.c */
void func_0205e47c(u32 tag, u32 data, BOOL err);
void func_0205e4b0(void* arg);
BOOL func_0205e508(UnkCardCommon* common, u32 req, s32 retry);

/* card_pullout.c */
void func_0205e65c(void);
void func_0205e688(u32 tag, u32 data, BOOL err);
BOOL CARD_IsPulledOut(void);
void func_0205e6ec(void);
void func_0205e778(u32 id);
void func_0205e7d8(u32 data, s32 wait);

#endif
