// Low level PCM stream channels driven by an alarm.
#include "snd_internal.h"

typedef struct UnkSleepCallbackInfo {
    /* 0x00 */ void (*func)(void*);
    /* 0x04 */ void* arg;
    /* 0x08 */ struct UnkSleepCallbackInfo* next;
} UnkSleepCallbackInfo;

typedef void (*UnkStrmCallback)(int status, int chNum, void** buffer, u32 len, int format, void* arg);

typedef struct UnkSndStrm {
    /* 0x00 */ UnkLink link;
    /* 0x08 */ UnkSleepCallbackInfo preSleepInfo;
    /* 0x14 */ UnkSleepCallbackInfo postSleepInfo;
    /* 0x20 */ int format;
    /* 0x24 */ BOOL active : 1;
    /* 0x24 */ BOOL started : 1;
    /* 0x28 */ u32 bufSize;
    /* 0x2C */ int interval;
    /* 0x30 */ UnkStrmCallback callback;
    /* 0x34 */ void* arg;
    /* 0x38 */ int curBlock;
    /* 0x3C */ int volume;
    /* 0x40 */ int alarmNo;
    /* 0x44 */ u32 chBitMask;
    /* 0x48 */ int chNum;
    /* 0x4C */ u8 chNo[16];
} UnkSndStrm;

typedef struct UnkSndStrmChannel {
    /* 0x00 */ u8* bufAddr;
    /* 0x04 */ int volume;
} UnkSndStrmChannel;

BOOL data_02112bc4;                  // list initialized
UnkList data_02112bc8;               // active streams
void* data_02112bd4[16];             // buffer pointers passed to callbacks
UnkSndStrmChannel data_02112c14[16]; // per hardware channel

void func_0207c1dc(UnkSleepCallbackInfo* info);
void func_0207c1f4(UnkSleepCallbackInfo* info);
void func_0207c20c(UnkSleepCallbackInfo* info);
void func_0207c224(UnkSleepCallbackInfo* info);

void func_02073ef8(UnkSndStrm* strm);
void func_02073fb0(UnkSndStrm* strm);
void func_02074024(UnkSndStrm* strm);
void func_02074054(void* arg);
void func_02074064(UnkSndStrm* strm, int status);
void func_02074114(void* arg);
void func_02074160(void* arg);

void func_02073c04(UnkSndStrm* strm) {
    if (!data_02112bc4) {
        func_02067dec(&data_02112bc8, 0);
        data_02112bc4 = TRUE;
    }

    strm->preSleepInfo.func = func_02074114;
    strm->preSleepInfo.arg = strm;
    strm->postSleepInfo.func = func_02074160;
    strm->postSleepInfo.arg = strm;
    strm->chBitMask = 0;
    strm->chNum = 0;
    strm->active = FALSE;
    strm->started = FALSE;
}

BOOL func_02073c7c(UnkSndStrm* strm, int chNum, const u8* chNoList) {
    u32 chBitMask = 0;
    int i;

    for (i = 0; i < chNum; i++) {
        strm->chNo[i] = chNoList[i];
        chBitMask |= 1 << chNoList[i];
    }

    if (!func_02073258(chBitMask)) {
        return FALSE;
    }

    strm->chNum = chNum;
    strm->chBitMask = chBitMask;
    return TRUE;
}

void func_02073cdc(UnkSndStrm* strm) {
    if (strm->chBitMask == 0) {
        return;
    }

    func_020732a0(strm->chBitMask);
    strm->chBitMask = 0;
    strm->chNum = 0;
}

BOOL func_02073d04(UnkSndStrm* strm, int format, u8* buffer, u32 bufSize, int timer, int interval, UnkStrmCallback callback, void* arg) {
    u32 len;
    u32 period;
    int i;
    int irq;

    if (strm->active) {
        func_02073ef8(strm);
    }

    bufSize = bufSize / (strm->chNum * (interval * 32));
    strm->bufSize = bufSize * interval * 32;

    len = strm->bufSize;
    if (format == 1) {
        len >>= 1;
    }
    period = timer * len / interval;

    strm->alarmNo = func_020732ec();
    if (strm->alarmNo < 0) {
        return FALSE;
    }

    for (i = 0; i < strm->chNum; i++) {
        int chNo = strm->chNo[i];
        UnkSndStrmChannel* ch = &data_02112c14[chNo];

        ch->bufAddr = buffer + strm->bufSize * i;
        ch->volume = 0;
        SND_SetupChannelPcm(chNo, format, ch->bufAddr, 1, 0, strm->bufSize >> 2, 0x7F, 0, timer * 32, 0x40);
    }

    SND_SetupAlarm(strm->alarmNo, period, period, func_02074054, strm);
    func_02067e30(&data_02112bc8, strm);

    strm->format = format;
    strm->interval = interval;
    strm->callback = callback;
    strm->arg = arg;
    strm->curBlock = 0;
    strm->volume = 0;
    strm->active = TRUE;

    irq = OS_DisableIRQ();
    strm->interval = 1;
    func_02074064(strm, 0);
    strm->interval = interval;
    OS_RestoreIRQ(irq);

    return TRUE;
}

void func_02073ea4(UnkSndStrm* strm) {
    func_0207a450(strm->chBitMask, 0, 1 << strm->alarmNo, 0);

    if (strm->started) {
        return;
    }

    func_0207c1dc(&strm->preSleepInfo);
    func_0207c1f4(&strm->postSleepInfo);
    strm->started = TRUE;
}

void func_02073ef8(UnkSndStrm* strm) {
    if (!strm->active) {
        return;
    }
    func_02073fb0(strm);
}

void func_02073f14(UnkSndStrm* strm, int volume) {
    int chNo;
    int i;

    strm->volume = volume;

    for (i = 0; i < strm->chNum; i++) {
        s32 vol;
        chNo = strm->chNo[i];
        vol = func_0207b024(strm->volume + data_02112c14[chNo].volume);

        SND_SetChannelVolume(1 << chNo, vol & 0xFF, vol >> 8);
    }
}

void func_02073f80(UnkSndStrm* strm, int index, int pan) {
    if (index > strm->chNum - 1) {
        return;
    }
    SND_SetChannelPan(1 << strm->chNo[index], pan);
}

void func_02073fb0(UnkSndStrm* strm) {
    if (strm->started) {
        u32 tag;

        func_0207a478(strm->chBitMask, 0, 1 << strm->alarmNo, 0);
        func_0207c20c(&strm->preSleepInfo);
        func_0207c224(&strm->postSleepInfo);
        strm->started = FALSE;

        tag = func_0207ac10();
        func_0207a9e8(1);
        func_0207aba4(tag);
    }

    func_02074024(strm);
}

void func_02074024(UnkSndStrm* strm) {
    func_02073334(strm->alarmNo);
    func_02067f38(&data_02112bc8, strm);
    strm->active = FALSE;
}

void func_02074054(void* arg) {
    func_02074064(arg, 1);
}

void func_02074064(UnkSndStrm* strm, int status) {
    u32 len = strm->bufSize / strm->interval;
    unsigned long offset = len * strm->curBlock;
    int i;

    for (i = 0; i < strm->chNum; i++) {
        data_02112bd4[i] = data_02112c14[strm->chNo[i]].bufAddr + offset;
    }

    strm->callback(status, strm->chNum, data_02112bd4, len, strm->format, strm->arg);

    if (++strm->curBlock >= strm->interval) {
        strm->curBlock = 0;
    }
}

void func_02074114(void* arg) {
    UnkSndStrm* strm = arg;
    u32 tag;

    if (!strm->started) {
        return;
    }

    func_0207a478(strm->chBitMask, 0, 1 << strm->alarmNo, 0);

    tag = func_0207ac10();
    func_0207a9e8(1);
    func_0207aba4(tag);
}

void func_02074160(void* arg) {
    UnkSndStrm* strm = arg;

    if (!strm->started) {
        return;
    }

    while (strm->curBlock != 0) {
        int irq = OS_DisableIRQ();
        func_02074064(strm, 1);
        OS_RestoreIRQ(irq);
    }

    func_0207a450(strm->chBitMask, 0, 1 << strm->alarmNo, 0);
}
