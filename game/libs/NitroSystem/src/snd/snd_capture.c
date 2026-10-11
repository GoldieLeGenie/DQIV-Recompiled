// Capture effect: worker thread, volume fade and stop/sleep handling.
#include "snd_internal.h"

typedef struct UnkSndFader {
    /* 0x00 */ s32 origin;
    /* 0x04 */ s32 target;
    /* 0x08 */ s32 counter;
    /* 0x0C */ s32 frames;
} UnkSndFader;

typedef void (*UnkCaptureCallback)(void* buf0, void* buf1, u32 size, int format, void* arg);

typedef struct UnkSndCapture {
    /* 0x00 */ BOOL active;
    /* 0x04 */ int type;
    /* 0x08 */ int format;
    /* 0x0C */ void* buf0;
    /* 0x10 */ void* buf1;
    /* 0x14 */ u32 bufSize;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 unk_1c;
    /* 0x20 */ u32 lockedChBitMask;
    /* 0x24 */ u32 chBitMask;
    /* 0x28 */ u32 capBitMask;
    /* 0x2C */ int alarmNo;
    /* 0x30 */ u32 unk_30;
    /* 0x34 */ UnkCaptureCallback callback;
    /* 0x38 */ void* arg;
    /* 0x3C */ UnkSndFader fader;
    /* 0x4C */ BOOL stopAfterFade;
    /* 0x50 */ int volume;
} UnkSndCapture;

typedef struct UnkSndCaptureMsg {
    /* 0x00 */ UnkSndCapture* capture;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ void* buf0;
    /* 0x10 */ void* buf1;
} UnkSndCaptureMsg;

typedef struct UnkMessageQueue {
    /* 0x00 */ u8 unk_00[0x20];
} UnkMessageQueue;

typedef struct UnkSndCaptureUnk {
    /* 0x00 */ u32 unk_00[0x28];
} UnkSndCaptureUnk;

BOOL             data_02112c94;        // capture thread created
u32              data_02112c98;
UnkMessageQueue  data_02112c9c;        // capture message queue
void*            data_02112cbc[8];     // capture messages
UnkSndCapture    data_02112cdc;        // capture state
UnkSndCaptureUnk data_02112d30;        // used only by code that is not linked into the ROM
OSThread         data_02112dd0;        // capture thread
u8               data_02112e90[0x400]; // capture thread stack

// Uses data_02112d30 like the library code that is not linked into the ROM (keeps it in the pooled .bss,
// between the capture state and the capture thread).
static void unkfunc_unused_25(void) {
    data_02112d30.unk_00[0] = 0;
}

void OS_WakeupThreadDirect(void* thread);
s32  func_02077284(UnkSndFader* fader);
void func_020772b8(UnkSndFader* fader);
BOOL func_020772d0(UnkSndFader* fader);

void func_020742fc(void);
void func_020744d0(void* arg);

void func_020741d0(u32 prio) {
    if (data_02112c94) {
        return;
    }

    data_02112c98 = 0;
    func_02078d60(&data_02112c9c, data_02112cbc, 8);
    func_020787c4(&data_02112dd0, func_020744d0, NULL, data_02112e90 + sizeof(data_02112e90), sizeof(data_02112e90), prio);
    data_02112c94 = TRUE;
    OS_WakeupThreadDirect(&data_02112dd0);
}

void func_0207425c(void) {
    data_02112c94 = FALSE;
    data_02112cdc.active = FALSE;
}

void func_02074274(void) {
    UnkSndCapture* capture = &data_02112cdc;
    int volume;

    if (!data_02112cdc.active) {
        return;
    }
    if (capture->type != 0) {
        return;
    }

    func_020772b8(&capture->fader);

    if (capture->stopAfterFade && func_020772d0(&capture->fader)) {
        func_020742fc();
        return;
    }

    volume = func_02077284(&capture->fader) >> 8;
    if (volume == capture->volume) {
        return;
    }

    SND_SetChannelVolume(capture->chBitMask, volume, 0);
    capture->volume = volume;
}

void func_020742fc(void) {
    UnkSndCapture* capture = &data_02112cdc;
    BOOL hasAlarm;

    if (!data_02112cdc.active) {
        return;
    }

    hasAlarm = capture->alarmNo >= 0;
    func_0207a478(capture->chBitMask, capture->capBitMask, hasAlarm ? 1 << capture->alarmNo : 0, 0);

    if (hasAlarm) {
        u32 tag = func_0207ac10();
        func_0207a9e8(1);
        func_0207aba4(tag);

        while (func_02078e1c(&data_02112c9c, NULL, 0)) {
        }
    }

    if (capture->capBitMask != 0) {
        func_020732d0(capture->capBitMask);
    }
    if (capture->lockedChBitMask != 0) {
        func_020732a0(capture->lockedChBitMask);
    }
    if (hasAlarm) {
        func_02073334(capture->alarmNo);
    }
    if (capture->type == 1) {
        func_0207a650(0, 0, 0, 0);
    }

    capture->active = FALSE;
}

void func_020743ec(void) {
    UnkSndCapture* capture = &data_02112cdc;
    u32 tag;

    if (!data_02112cdc.active) {
        return;
    }

    func_0207a478(capture->chBitMask, capture->capBitMask, capture->alarmNo >= 0 ? 1 << capture->alarmNo : 0, 0);

    tag = func_0207ac10();
    func_0207a9e8(1);
    func_0207aba4(tag);
}

void func_0207444c(void) {
    UnkSndCapture* capture = &data_02112cdc;

    if (!data_02112cdc.active) {
        return;
    }

    capture->unk_1c = 0;
    func_0206785c(0, capture->buf0, capture->bufSize);
    func_0206785c(0, capture->buf1, capture->bufSize);
    DC_PurgeRange(capture->buf0, capture->bufSize);
    DC_PurgeRange(capture->buf1, capture->bufSize);

    func_0207a450(capture->chBitMask, capture->capBitMask, capture->alarmNo >= 0 ? 1 << capture->alarmNo : 0, 0);
}

void func_020744d0(void* arg) {
    void* msg;

    while (TRUE) {
        UnkSndCaptureMsg* info;
        UnkSndCapture* capture;

        func_02078e1c(&data_02112c9c, &msg, 1);
        info = msg;
        if (info == NULL) {
            break;
        }

        DC_InvalidateRange(info->buf0, info->size);
        DC_InvalidateRange(info->buf1, info->size);

        capture = info->capture;
        capture->callback(info->buf0, info->buf1, info->size, capture->format, capture->arg);
    }
}
