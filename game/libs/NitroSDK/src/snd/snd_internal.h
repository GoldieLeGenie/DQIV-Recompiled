#ifndef SND_INTERNAL_H
#define SND_INTERNAL_H

#include "../sdk_internal.h"

typedef struct UnkSndCommand {
    struct UnkSndCommand* next;
    s32 id;
    u32 arg[4];
} UnkSndCommand;

typedef struct UnkSndPlayerWork {
    s16 variable[16];
    u32 tickCounter;
} UnkSndPlayerWork;

typedef struct UnkSndSharedWork {
    vu32 finishCommandTag;          // 0x00
    vu32 playerStatus;              // 0x04
    vu16 channelStatus;             // 0x08
    vu16 captureStatus;             // 0x0a
    vu32 unk_0c[5];                 // 0x0c
    UnkSndPlayerWork player[16];    // 0x20
    vs16 globalVariable[16];        // 0x260
} UnkSndSharedWork;

typedef void (*UnkSndAlarmHandler)(void* arg);

typedef struct UnkSndAlarm {
    UnkSndAlarmHandler func;
    void* arg;
    u8 id;
} UnkSndAlarm;

UnkSndCommand* func_0207a928(u32 flags);
void func_0207a9b0(UnkSndCommand* cmd);
BOOL func_0207a9e8(u32 flags);
UnkSndCommand* func_0207a818(u32 flags);
BOOL func_0207ac3c(u32 tag);
s32 func_0207ac8c(void);
s32 func_0207acc8(void);
s32 func_0207ad04(void);
void func_0207ad20(u32 tag, u32 data, BOOL err);
void func_0207ad44(void);
void func_0207ada4(void);
UnkSndCommand* func_0207adcc(void);
BOOL func_0207ae14(void);
void func_0207a744(void);
void func_0207a71c(void);
void func_0207a730(void);
void func_0207a6a0(s32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3);
void func_0207a678(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

void func_0207ae54(void);
void func_0207ae84(s32 no);
u8 func_0207aea4(s32 no, UnkSndAlarmHandler func, void* arg);
void func_0207aed4(s32 data);

u32 func_0207af80(void);
void func_0207afa8(UnkSndSharedWork* work);

#endif
