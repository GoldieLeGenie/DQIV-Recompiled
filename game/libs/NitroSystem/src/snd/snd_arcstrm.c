// Sound archive stream player: stream players, request thread and data loading/ADPCM decoding.
#include "snd_internal.h"

typedef struct UnkSndFader {
    /* 0x00 */ s32 origin;
    /* 0x04 */ s32 target;
    /* 0x08 */ s32 counter;
    /* 0x0C */ s32 frames;
} UnkSndFader;

typedef struct UnkSndStrm {
    /* 0x00 */ u8 unk_00[0x5C];
} UnkSndStrm;

typedef struct UnkSndAdpcmState {
    /* 0x00 */ s16 pred;
    /* 0x02 */ u8 index;
    /* 0x03 */ u8 unk_03;
} UnkSndAdpcmState;

struct UnkSndArcStrm;
struct UnkSndArcStrmHandle;

typedef BOOL (*UnkSndArcStrmOpenFunc)(struct UnkSndArcStrm* strm, u32 fileId);
typedef void (*UnkSndArcStrmCloseFunc)(struct UnkSndArcStrm* strm);
typedef s32 (*UnkSndArcStrmReadFunc)(struct UnkSndArcStrm* strm, void* dst, s32 size, u32 offset);

typedef struct UnkSndArcStrmPos {
    /* 0x00 */ int no;
    /* 0x04 */ u32 offset;
} UnkSndArcStrmPos;

typedef void (*UnkSndArcStrmDataCallback)(int status, int chNum, void** buffer, u32 len, int format, void* arg);
typedef BOOL (*UnkSndArcStrmNextCallback)(int status, UnkSndArcStrmPos* cur, UnkSndArcStrmPos* next, void* arg);

typedef struct UnkSndArcStrm {
    /* 0x000 */ UnkSndStrm strm;
    /* 0x05C */ FS_File file;
    /* 0x0A4 */ u32 fileOffset;          // file offset, or memory address of the file image
    /* 0x0A8 */ u8 header[0x18];
    /* 0x0C0 */ u8 format;
    /* 0x0C1 */ u8 loop;
    /* 0x0C2 */ u8 channels;
    /* 0x0C3 */ u8 unk_c3;
    /* 0x0C4 */ u16 sampleRate;
    /* 0x0C6 */ u16 timer;
    /* 0x0C8 */ u32 loopStart;
    /* 0x0CC */ u32 sampleCount;
    /* 0x0D0 */ u32 dataOffset;
    /* 0x0D4 */ u32 blockCount;
    /* 0x0D8 */ u32 blockSize;
    /* 0x0DC */ u32 blockSamples;
    /* 0x0E0 */ u32 lastBlockSize;
    /* 0x0E4 */ u32 lastBlockSamples;
    /* 0x0E8 */ UnkSndFader fader;
    /* 0x0F8 */ UnkSndAdpcmState adpcmState[6];
    /* 0x110 */ BOOL active : 1;
    /* 0x110 */ BOOL started : 1;
    /* 0x110 */ BOOL startRequest : 1;
    /* 0x110 */ BOOL fadeStop : 1;
    /* 0x110 */ BOOL seekDecode : 1;
    /* 0x110 */ BOOL dataEnd : 1;
    /* 0x110 */ BOOL mono : 1;
    /* 0x114 */ volatile int stopWait;
    /* 0x118 */ BOOL prepared;
    /* 0x11C */ int requestCount;
    /* 0x120 */ int channelLockCount;
    /* 0x124 */ u8 chNum;
    /* 0x125 */ u8 unk_125;
    /* 0x126 */ u8 chNo[6];
    /* 0x12C */ void* buffer;
    /* 0x130 */ u32 bufSize;
    /* 0x134 */ UnkSndArcStrmDataCallback dataCallback;
    /* 0x138 */ void* dataCallbackArg;
    /* 0x13C */ UnkSndArcStrmNextCallback nextCallback;
    /* 0x140 */ void* nextCallbackArg;
    /* 0x144 */ int strmNo;
    /* 0x148 */ int playerNo;
    /* 0x14C */ struct UnkSndArcStrmHandle* handle;
    /* 0x150 */ int prio;
    /* 0x154 */ int playerVolume;
    /* 0x158 */ int volume;
    /* 0x15C */ int curVolume;
    /* 0x160 */ u32 curPos;
    /* 0x164 */ UnkSndArcStrmOpenFunc openFunc;
    /* 0x168 */ UnkSndArcStrmCloseFunc closeFunc;
    /* 0x16C */ UnkSndArcStrmReadFunc readFunc;
    /* 0x170 */ UnkSndArcStrmCloseFunc stopFunc;
} UnkSndArcStrm; // size 0x174

typedef struct UnkSndArcStrmHandle {
    /* 0x00 */ UnkSndArcStrm* volatile strm;
} UnkSndArcStrmHandle;

// sample format passed to the data callback
typedef enum UnkSndStrmFormat {
    UNK_SND_STRM_FORMAT_PCM8,
    UNK_SND_STRM_FORMAT_PCM16
} UnkSndStrmFormat;

typedef struct UnkSndArcStrmRequest {
    /* 0x00 */ UnkLink link;
    /* 0x08 */ UnkSndArcStrm* strm;
    /* 0x0C */ int status;
    /* 0x10 */ int chNum;
    /* 0x14 */ void* buffer[6];
    /* 0x2C */ u32 len;
} UnkSndArcStrmRequest; // size 0x30

typedef struct UnkSndArcStrmThread {
    /* 0x000 */ OSThread thread;
    /* 0x0C0 */ u8 stack[0x400];
    /* 0x4C0 */ OSThreadQueue queue;
    /* 0x4C8 */ OSMutex mutex;
    /* 0x4E0 */ UnkList requestList;
} UnkSndArcStrmThread; // size 0x4EC

typedef struct UnkSndArcStrmPlayerInfo {
    /* 0x00 */ u8 chNum;
    /* 0x01 */ u8 chNo[16];
} UnkSndArcStrmPlayerInfo;

typedef struct UnkSndArcStrmInfo {
    /* 0x00 */ u32 fileId;
    /* 0x04 */ u8 volume;
    /* 0x05 */ u8 prio;
    /* 0x06 */ u8 playerNo;
    /* 0x07 */ u8 flags;
} UnkSndArcStrmInfo;

typedef struct UnkSndArcStrmWork {
    /* 0x00 */ BOOL initialized;
    /* 0x04 */ UnkSndArcStrmThread* thread;  // optional second thread
    /* 0x08 */ u8* adpcmBuffer;
} UnkSndArcStrmWork;

/* definition order chosen for the .bss layout (the compiler sorts the objects by size) */
UnkSndArcStrmWork data_021132d0;
UnkList data_021132dc;                          // free request list
OSMutex data_021132e8;                          // ADPCM buffer mutex
UnkSndArcStrmThread data_02113480;              // stream thread
UnkSndArcStrmRequest data_02113300[8];          // requests
UnkSndArcStrm data_0211396c[4];                 // stream players
ATTRIBUTE_ALIGN(32) u8 data_02113f40[0x200];    // ADPCM read buffer
extern const s16 data_020ba8ac[];               // decibel to volume table
// IMA-ADPCM index adjustment per 4-bit sample
const s8 data_020ba6e8[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};
// IMA-ADPCM step sizes
const s16 data_020ba6f8[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21,
    23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66,
    73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209,
    230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
    724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
    7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350,
    22385, 24623, 27086, 29794, 32767,
};

void OS_WakeupThreadDirect(void* thread);
void* func_020747bc(void);
void* func_02074a24(int index);
void* func_0207495c(int index);
u32   func_02074a88(u32 fileId);
s32   func_02074ad8(u32 fileId, void* dst, u32 size, u32 offset);
void  func_02074b50(FS_FileIdentifier* fileId);
void* func_02074b70(u32 fileId);
void* func_02074d1c(void* heap, u32 size, void (*callback)(void*, u32, void*, u32), void* data1, u32 data2);
void  func_02073c04(UnkSndStrm* strm);
BOOL  func_02073c7c(UnkSndStrm* strm, int chNum, const u8* chNoList);
void  func_02073cdc(UnkSndStrm* strm);
BOOL  func_02073d04(UnkSndStrm* strm, int format, void* buffer, u32 bufSize, int timer, int interval, UnkSndArcStrmDataCallback callback, void* arg);
void  func_02073ea4(UnkSndStrm* strm);
void  func_02073ef8(UnkSndStrm* strm);
void  func_02073f14(UnkSndStrm* strm, int volume);
void  func_02073f80(UnkSndStrm* strm, int index, int pan);
void  func_02077244(UnkSndFader* fader);
void  func_0207725c(UnkSndFader* fader, s32 target, s32 frames);
s32   func_02077284(UnkSndFader* fader);
void  func_020772b8(UnkSndFader* fader);
BOOL  func_020772d0(UnkSndFader* fader);

BOOL func_02075ba8(void* heap);

BOOL func_02075c70(UnkSndArcStrmHandle* handle, int strmNo, u32 offset);
void func_02075ccc(UnkSndArcStrmHandle* handle);
void func_02075d50(UnkSndArcStrmHandle* handle);
UnkSndArcStrm* func_02075e78(UnkSndArcStrmHandle* handle, int playerNo, int prio);
void func_02075f04(UnkSndArcStrm* strm);
BOOL func_02075f30(UnkSndArcStrmHandle* handle, UnkSndArcStrmInfo* info, int playerNo, int prio, int strmNo, u32 offset,
                   UnkSndArcStrmDataCallback dataCallback, void* dataCallbackArg, UnkSndArcStrmNextCallback nextCallback, void* nextCallbackArg);
void func_020761a0(UnkSndArcStrm* strm, int fadeFrame);
void func_020761f8(UnkSndArcStrm* strm);
void func_02076290(UnkSndArcStrm* strm);
BOOL func_020762f4(UnkSndArcStrm* strm, int chNum, const u8* chNo);
void func_0207632c(UnkSndArcStrm* strm);
void func_02076350(UnkSndArcStrmThread* thread, u32 prio);
void func_020763b0(UnkList* list, UnkSndArcStrm* strm);
UnkSndArcStrmRequest* func_02076420(UnkList* list);
UnkSndArcStrmRequest* func_02076470(void);
void func_020764b0(UnkSndArcStrmRequest* request);
void func_020764dc(void* mem, u32 size, void* data1, u32 data2);
void func_02076578(int status, int chNum, void** buffer, u32 len, int format, void* arg);
void func_020766b0(UnkSndArcStrm* strm);
void func_020767f4(UnkSndArcStrmRequest* request);
void func_0207701c(UnkSndArcStrm* strm, u32 fileId);
BOOL func_0207709c(UnkSndArcStrm* strm, u32 fileId);
void func_02077110(UnkSndArcStrm* strm);
s32  func_02077120(UnkSndArcStrm* strm, void* dst, s32 size, u32 offset);
void func_02077158(UnkSndArcStrm* strm);
BOOL func_02077168(UnkSndArcStrm* strm, u32 fileId);
void func_02077190(UnkSndArcStrm* strm);
s32  func_02077194(UnkSndArcStrm* strm, void* dst, s32 size, u32 offset);
void func_020771b0(UnkSndArcStrm* strm);
void func_020771b4(void* arg);

// The loops of this function are not rotated/strength-reduced in the ROM, which mwcc only
// reproduces with global optimization level 2 for this function.
#pragma push
#pragma optimization_level 2
void func_02075aa4(u32 threadPrio, void* heap) {
    int i;
    int j;

    if (data_021132d0.initialized) {
        func_02075ba8(heap);
        return;
    }

    data_021132d0.initialized = TRUE;

    func_02067dec(&data_021132dc, 0);
    for (j = 0; j < 8; j++) {
        func_02067e30(&data_021132dc, &data_02113300[j]);
    }

    OS_InitMutex(&data_021132e8);
    data_021132d0.adpcmBuffer = data_02113f40;

    for (i = 0; i < 4; i++) {
        UnkSndArcStrm* strm = &data_0211396c[i];

        strm->active = FALSE;
        func_02060e04(&strm->file);
        func_02073c04(&strm->strm);
        strm->playerNo = i;
        strm->chNum = 0;
        strm->buffer = NULL;
        strm->bufSize = 0;
        strm->channelLockCount = 0;
    }

    func_02075ba8(heap);
    func_02076350(&data_02113480, threadPrio);
}

#pragma pop

BOOL func_02075ba8(void* heap) {
    int i;

    for (i = 0; i < 4; i++) {
        UnkSndArcStrm* strm = &data_0211396c[i];
        UnkSndArcStrmPlayerInfo* info = func_02074a24(i);
        int j;

        if (info == NULL) {
            continue;
        }

        strm->chNum = info->chNum;
        for (j = 0; j < info->chNum; j++) {
            strm->chNo[j] = info->chNo[j];
        }

        if (heap != NULL) {
            u32 size = strm->chNum * 0x800;
            void* buffer = func_02074d1c(heap, size, func_020764dc, strm, 0);

            if (buffer == NULL) {
                return FALSE;
            }

            func_020761f8(strm);
            strm->buffer = buffer;
            strm->bufSize = size;
        }
    }

    return TRUE;
}

BOOL func_02075c70(UnkSndArcStrmHandle* handle, int strmNo, u32 offset) {
    UnkSndArcStrmInfo* info = func_0207495c(strmNo);

    if (info == NULL) {
        return FALSE;
    }
    return func_02075f30(handle, info, info->playerNo, info->prio, strmNo, offset, NULL, NULL, NULL, NULL);
}

void func_02075ccc(UnkSndArcStrmHandle* handle) {
    UnkSndArcStrm* strm;

    if (handle->strm == NULL) {
        return;
    }
    strm = handle->strm;
    strm->startRequest = TRUE;
}

BOOL func_02075cec(UnkSndArcStrmHandle* handle, int strmNo, u32 offset) {
    if (!func_02075c70(handle, strmNo, offset)) {
        return FALSE;
    }
    func_02075ccc(handle);
    return TRUE;
}

void func_02075d14(UnkSndArcStrmHandle* handle, int fadeFrame) {
    if (handle->strm == NULL) {
        return;
    }
    func_020761a0(handle->strm, fadeFrame);
}

void func_02075d30(UnkSndArcStrmHandle* handle, int volume) {
    if (handle->strm != NULL) {
        handle->strm->volume = volume;
    }
}

void func_02075d44(UnkSndArcStrmHandle* handle) {
    handle->strm = NULL;
}

void func_02075d50(UnkSndArcStrmHandle* handle) {
    UnkSndArcStrm* strm = handle->strm;

    if (strm != NULL) {
        strm->handle = NULL;
        handle->strm = NULL;
    }
}

void func_02075d68(void) {
    int i;

    for (i = 0; i < 4; i++) {
        UnkSndArcStrm* strm = &data_0211396c[i];
        int volume;

        if (!strm->active) {
            continue;
        }

        if (strm->stopWait == 0) {
            func_020761f8(strm);
            continue;
        }

        if (strm->startRequest && strm->prepared) {
            func_02073ea4(&strm->strm);
            strm->started = TRUE;
            strm->startRequest = FALSE;
        }

        if (!strm->started) {
            continue;
        }

        func_020772b8(&strm->fader);

        {
            int playerVol = data_020ba8ac[strm->playerVolume];
            int fade = func_02077284(&strm->fader);
            int fadeVol;

            fade >>= 8;
            fadeVol = data_020ba8ac[fade];
            volume = data_020ba8ac[strm->volume] + (fadeVol + playerVol);
        }
        if (volume != strm->curVolume) {
            func_02073f14(&strm->strm, volume);
            strm->curVolume = volume;
        }

        if (strm->fadeStop && func_020772d0(&strm->fader)) {
            func_020761f8(strm);
        }
    }
}

UnkSndArcStrm* func_02075e78(UnkSndArcStrmHandle* handle, int playerNo, int prio) {
    UnkSndArcStrm* strm;

    if (handle->strm != NULL) {
        func_02075d50(handle);
    }

    strm = &data_0211396c[playerNo];
    if (strm->buffer == NULL) {
        return NULL;
    }

    if (strm->active) {
        if (prio < strm->prio) {
            return NULL;
        }
        func_020761f8(strm);
    }

    strm->prio = prio;
    strm->active = TRUE;
    strm->handle = handle;
    handle->strm = strm;
    return strm;
}

void func_02075f04(UnkSndArcStrm* strm) {
    if (strm->handle != NULL) {
        strm->handle->strm = NULL;
        strm->handle = NULL;
    }

    strm->active = FALSE;
    strm->startRequest = FALSE;
    strm->started = FALSE;
}

BOOL func_02075f30(UnkSndArcStrmHandle* handle, UnkSndArcStrmInfo* info, int playerNo, int prio, int strmNo, u32 offset,
                   UnkSndArcStrmDataCallback dataCallback, void* dataCallbackArg, UnkSndArcStrmNextCallback nextCallback, void* nextCallbackArg) {
    UnkSndArcStrm* strm = func_02075e78(handle, playerNo, prio);
    int format;
    int chNum;

    if (strm == NULL) {
        return FALSE;
    }

    func_0207701c(strm, info->fileId);
    if (!strm->openFunc(strm, info->fileId)) {
        func_02075f04(strm);
        return FALSE;
    }

    strm->curPos = (u64)strm->sampleRate * offset / 1000;
    if (strm->curPos != 0 && strm->format == 2) {
        strm->seekDecode = TRUE;
    } else {
        strm->seekDecode = FALSE;
    }

    strm->stopWait = 4;
    strm->dataEnd = FALSE;
    strm->started = FALSE;
    strm->prepared = FALSE;
    strm->startRequest = FALSE;
    strm->fadeStop = FALSE;
    strm->requestCount = 0;
    strm->dataCallback = dataCallback;
    strm->dataCallbackArg = dataCallbackArg;
    strm->nextCallback = nextCallback;
    strm->nextCallbackArg = nextCallbackArg;
    strm->strmNo = strmNo;
    strm->curVolume = 0;
    strm->playerVolume = info->volume;
    strm->volume = 0x7F;
    func_02077244(&strm->fader);
    func_0207725c(&strm->fader, 0x7F00, 1);

    switch (strm->format) {
    case 0:
        format = 0;
        break;
    case 1:
    case 2:
        format = 1;
        break;
    }

    chNum = strm->channels;
    if (info->flags & 1) {
        chNum = 2;
    }
    if (chNum > strm->chNum) {
        chNum = strm->chNum;
    }
    strm->mono = chNum == 1;

    if (!func_020762f4(strm, chNum, strm->chNo)) {
        strm->closeFunc(strm);
        func_02075f04(strm);
        return FALSE;
    }

    if (!func_02073d04(&strm->strm, format, strm->buffer, strm->bufSize * chNum / strm->chNum, strm->timer, 4, func_02076578, strm)) {
        func_0207632c(strm);
        strm->closeFunc(strm);
        func_02075f04(strm);
        return FALSE;
    }

    if (chNum == 2) {
        func_02073f80(&strm->strm, 0, 0);
        func_02073f80(&strm->strm, 1, 0x7F);
    }

    return TRUE;
}

void func_020761a0(UnkSndArcStrm* strm, int fadeFrame) {
    if (!strm->started) {
        func_020761f8(strm);
        return;
    }

    if (fadeFrame == 0) {
        func_020761f8(strm);
        return;
    }

    func_0207725c(&strm->fader, 0, fadeFrame);
    strm->fadeStop = TRUE;
    strm->prio = 0;
}

void func_020761f8(UnkSndArcStrm* strm) {
    OS_LockMutex(&data_02113480.mutex);
    if (data_021132d0.thread != NULL) {
        OS_LockMutex(&data_021132d0.thread->mutex);
    }

    if (strm->started) {
        func_02073ef8(&strm->strm);
    }
    if (strm->active) {
        strm->stopFunc(strm);
    }
    func_02076290(strm);

    OS_UnlockMutex(&data_02113480.mutex);
    if (data_021132d0.thread != NULL) {
        OS_UnlockMutex(&data_021132d0.thread->mutex);
    }
}

void func_02076290(UnkSndArcStrm* strm) {
    if (!strm->active) {
        return;
    }

    func_0207632c(strm);
    strm->closeFunc(strm);

    func_020763b0(&data_02113480.requestList, strm);
    if (data_021132d0.thread != NULL) {
        func_020763b0(&data_021132d0.thread->requestList, strm);
    }

    func_02075f04(strm);
}

BOOL func_020762f4(UnkSndArcStrm* strm, int chNum, const u8* chNo) {
    if (strm->channelLockCount == 0) {
        if (!func_02073c7c(&strm->strm, chNum, chNo)) {
            return FALSE;
        }
    }
    strm->channelLockCount++;
    return TRUE;
}

void func_0207632c(UnkSndArcStrm* strm) {
    if (strm->channelLockCount == 0) {
        return;
    }
    if (--strm->channelLockCount != 0) {
        return;
    }
    func_02073cdc(&strm->strm);
}

void func_02076350(UnkSndArcStrmThread* thread, u32 prio) {
    func_020787c4(&thread->thread, func_020771b4, thread, thread->stack + sizeof(thread->stack), sizeof(thread->stack), prio);
    func_02067dec(&thread->requestList, 0);
    OS_InitMutex(&thread->mutex);
    OS_InitThreadQueue(&thread->queue);
    OS_WakeupThreadDirect(&thread->thread);
}

void func_020763b0(UnkList* list, UnkSndArcStrm* strm) {
    int irq = OS_DisableIRQ();
    UnkSndArcStrmRequest* request = func_02067f98(list, NULL);

    while (request != NULL) {
        UnkSndArcStrmRequest* next = func_02067f98(list, request);

        if (request->strm == strm) {
            func_02067f38(list, request);
            func_020764b0(request);
        }
        request = next;
    }

    OS_RestoreIRQ(irq);
}

UnkSndArcStrmRequest* func_02076420(UnkList* list) {
    int irq = OS_DisableIRQ();
    UnkSndArcStrmRequest* request = func_02067f98(list, NULL);

    if (request != NULL) {
        func_02067f38(list, request);
        request->strm->requestCount--;
    }

    OS_RestoreIRQ(irq);
    return request;
}

UnkSndArcStrmRequest* func_02076470(void) {
    int irq = OS_DisableIRQ();
    UnkSndArcStrmRequest* request = func_02067f98(&data_021132dc, NULL);

    if (request != NULL) {
        func_02067f38(&data_021132dc, request);
    }

    OS_RestoreIRQ(irq);
    return request;
}

void func_020764b0(UnkSndArcStrmRequest* request) {
    int irq = OS_DisableIRQ();
    func_02067e30(&data_021132dc, request);
    OS_RestoreIRQ(irq);
}

void func_020764dc(void* mem, u32 size, void* data1, u32 data2) {
    UnkSndArcStrm* strm = data1;

    if (mem != strm->buffer) {
        return;
    }

    OS_LockMutex(&data_02113480.mutex);
    if (data_021132d0.thread != NULL) {
        OS_LockMutex(&data_021132d0.thread->mutex);
    }

    func_020761f8(strm);
    strm->buffer = NULL;
    strm->bufSize = 0;
    strm->chNum = 0;

    if (strm->channelLockCount > 0) {
        func_02073cdc(&strm->strm);
        strm->channelLockCount = 0;
    }

    OS_UnlockMutex(&data_02113480.mutex);
    if (data_021132d0.thread != NULL) {
        OS_UnlockMutex(&data_021132d0.thread->mutex);
    }
}

void func_02076578(int status, int chNum, void** buffer, u32 len, int format, void* arg) {
    UnkSndArcStrm* strm = arg;
    UnkSndArcStrmRequest* request;
    UnkSndArcStrmThread* thread;
    int i;

    if (strm->requestCount >= 2) {
        request = func_02067f98(&data_02113480.requestList, NULL);
        while (request != NULL) {
            if (request->strm == strm) {
                break;
            }
            request = func_02067f98(&data_02113480.requestList, request);
        }

        for (i = 0; i < request->chNum; i++) {
            MI_CpuSet(request->buffer[i], 0, request->len);
        }

        func_02067f38(&data_02113480.requestList, request);
        strm->requestCount--;
        func_020764b0(request);
    }

    request = func_02076470();
    request->strm = strm;
    request->status = status;
    request->chNum = chNum;
    for (i = 0; i < chNum; i++) {
        request->buffer[i] = buffer[i];
    }
    request->len = len;

    thread = &data_02113480;
    if (status == 0) {
        if (data_021132d0.thread != NULL) {
            thread = data_021132d0.thread;
        }
    }

    strm->requestCount++;
    func_02067e30(&thread->requestList, request);
    OS_UnpauseThread(&thread->queue);
}

void func_020766b0(UnkSndArcStrm* strm) {
    UnkSndArcStrmPos cur;
    UnkSndArcStrmPos next;
    UnkSndArcStrmInfo* info;
    int format;
    int sampleRate;

    cur.no = strm->playerNo;
    cur.offset = strm->strmNo;
    next.no = strm->strmNo;
    next.offset = 0;

    if (!strm->nextCallback(0, &cur, &next, strm->nextCallbackArg)) {
        return;
    }

    info = func_0207495c(next.no);
    if (info == NULL) {
        return;
    }

    format = strm->format;
    sampleRate = strm->sampleRate;
    strm->closeFunc(strm);
    func_0207701c(strm, info->fileId);

    if (!strm->openFunc(strm, info->fileId)) {
        return;
    }
    if (sampleRate != strm->sampleRate) {
        return;
    }
    if (format == 0 && strm->format != 0) {
        return;
    }
    if (format != 0 && strm->format == 0) {
        return;
    }

    strm->strmNo = next.no;
    strm->curPos = (u64)strm->sampleRate * next.offset / 1000;
    if (strm->curPos != 0 && strm->format == 2) {
        strm->seekDecode = TRUE;
    } else {
        strm->seekDecode = FALSE;
    }
    strm->dataEnd = FALSE;
}

static inline s16 UnkSndArcStrm_DecodeAdpcm(UnkSndAdpcmState* state, int data) {
    int step;
    int pred;
    int index;
    int diff;

    index = state->index;
    pred = state->pred;
    step = data_020ba6f8[index];

    data &= 0xF;
    diff = step >> 3;
    if (data & 4) {
        diff += step;
    }
    if (data & 2) {
        diff += step >> 1;
    }
    if (data & 1) {
        diff += step >> 2;
    }

    if (data & 8) {
        pred -= diff;
        if (pred < -0x8000) {
            pred = -0x8000;
        }
    } else {
        pred += diff;
        if (pred > 0x7FFF) {
            pred = 0x7FFF;
        }
    }

    index += data_020ba6e8[data];
    if (index < 0) {
        index = 0;
    } else if (index > 88) {
        index = 88;
    }

    state->pred = pred;
    state->index = index;
    return pred;
}

void func_020767f4(UnkSndArcStrmRequest* request) {
    UnkSndArcStrm* strm = request->strm;
    BOOL loopEnd;
    unsigned long bufOffset;
    u32 remain;
    u32 blockSize;
    u32 posInBlock;
    u32 fileOffset;
    u32 copySamples;
    u32 copyBytes;
    u32 readSize;
    u32 blockSamples;
    u32 curPos;
    u32 block;
    u32 samplesInBlock;
    u32 readOffset;
    int i;

    if (strm->dataEnd && strm->stopWait > 0) {
        strm->stopWait--;
    }

    remain = request->len;
    bufOffset = 0;

    while (remain != 0) {

        if (strm->dataEnd) {
            for (i = 0; i < request->chNum; i++) {
                MI_CpuSet((u8*)request->buffer[i] + bufOffset, 0, remain);
            }
            break;
        }

        blockSamples = strm->blockSamples;
        curPos = strm->curPos;
        block = curPos / blockSamples;
        if (block < strm->blockCount - 1) {
            samplesInBlock = blockSamples;
            blockSize = strm->blockSize;
        } else {
            blockSize = strm->lastBlockSize;
            samplesInBlock = strm->lastBlockSamples;
        }

        posInBlock = curPos - block * blockSamples;
        copySamples = remain;
        if (strm->format != 0) {
            copySamples = remain >> 1;
        }

        if (strm->seekDecode) {
            if (posInBlock == 0) {
                strm->seekDecode = FALSE;
            } else {
                copySamples = posInBlock;
                posInBlock = 0;
            }
        }

        loopEnd = FALSE;
        if (posInBlock + copySamples >= samplesInBlock) {
            copySamples = samplesInBlock - posInBlock;
            if (block >= strm->blockCount - 1) {
                if (strm->loop) {
                    loopEnd = TRUE;
                } else {
                    strm->dataEnd = TRUE;
                }
            }
        }

        readOffset = posInBlock;
        copyBytes = copySamples;
        switch (strm->format) {
        case 0:
            readSize = copySamples;
            break;
        case 1:
            copyBytes = copySamples * 2;
            readSize = copyBytes;
            readOffset = posInBlock * 2;
            break;
        case 2:
            readSize = ((posInBlock + copySamples + 1) >> 1) - (posInBlock >> 1);
            readOffset = posInBlock >> 1;
            if (posInBlock == 0) {
                readSize += 4;
            } else {
                readOffset += 4;
            }
            copyBytes *= 2;
            break;
        }

        fileOffset = strm->channels * (block * strm->blockSize) + readOffset;
        fileOffset += strm->dataOffset;

        for (i = 0; i < request->chNum; i++) {
            UnkSndAdpcmState* state = &strm->adpcmState[i];
            void* buf = (u8*)request->buffer[i] + bufOffset;
            s16* dst = buf;

            if (i < strm->channels) {
                s32 readResult;

                if (strm->format == 2) {
                    OS_LockMutex(&data_021132e8);
                    buf = data_021132d0.adpcmBuffer;
                }

                readResult = strm->readFunc(strm, buf, readSize, fileOffset + i * blockSize);
                if (readResult != readSize) {
                    strm->dataEnd = TRUE;
                    copyBytes = 0;
                    copySamples = 0;
                    loopEnd = FALSE;
                    if (strm->format == 2) {
                        OS_UnlockMutex(&data_021132e8);
                    }
                    break;
                }

                if (strm->format == 2) {
                    const u8* src = data_021132d0.adpcmBuffer;
                    u32 pos;
                    u32 end;

                    if (posInBlock == 0) {
                        const UnkSndAdpcmState* header = (const UnkSndAdpcmState*)src;
                        src = (const u8*)(header + 1);
                        *state = *header;
                    }

                    end = posInBlock + copySamples;
                    pos = posInBlock;
                    if (pos & 1) {
                        *dst++ = UnkSndArcStrm_DecodeAdpcm(state, *src >> 4);
                        pos++;
                        src++;
                    }

                    while (pos < (end & ~1)) {
                        *dst++ = UnkSndArcStrm_DecodeAdpcm(state, *src);
                        *dst++ = UnkSndArcStrm_DecodeAdpcm(state, *src >> 4);
                        pos += 2;
                        src++;
                    }

                    if (pos < end) {
                        *dst = UnkSndArcStrm_DecodeAdpcm(state, *src);
                    }

                    OS_UnlockMutex(&data_021132e8);
                }
            } else if (strm->mono) {
                MI_CpuSet(buf, 0, copyBytes);
            } else {
                MI_CpuCopyU8((u8*)request->buffer[0] + bufOffset, buf, copyBytes);
            }
        }

        if (strm->seekDecode) {
            strm->seekDecode = FALSE;
        } else {
            if (loopEnd) {
                strm->curPos = strm->loopStart;
            } else {
                strm->curPos += copySamples;
            }
            bufOffset += copyBytes;
            remain -= copyBytes;
            if (strm->dataEnd && strm->nextCallback != NULL) {
                func_020766b0(strm);
            }
        }
    }

    if (strm->dataCallback != NULL) {
        strm->dataCallback(request->status, request->chNum, request->buffer, request->len, strm->format == 0 ? UNK_SND_STRM_FORMAT_PCM8 : UNK_SND_STRM_FORMAT_PCM16, strm->dataCallbackArg);
    }

    for (i = 0; i < request->chNum; i++) {
        DC_PurgeRange(request->buffer[i], request->len);
    }

    if (request->status == 0) {
        strm->prepared = TRUE;
    }
}

void func_0207701c(UnkSndArcStrm* strm, u32 fileId) {
    if (func_02074b70(fileId) == NULL) {
        strm->openFunc = func_0207709c;
        strm->closeFunc = func_02077110;
        strm->readFunc = func_02077120;
        strm->stopFunc = func_02077158;
    } else {
        strm->openFunc = func_02077168;
        strm->closeFunc = func_02077190;
        strm->readFunc = func_02077194;
        strm->stopFunc = func_020771b0;
    }
}

BOOL func_0207709c(UnkSndArcStrm* strm, u32 fileId) {
    FS_FileIdentifier id;

    if (func_02074ad8(fileId, strm->header, 0x40, 0) != 0x40) {
        return FALSE;
    }

    func_02074b50(&id);
    if (!func_02061074(&strm->file, id)) {
        return FALSE;
    }

    strm->fileOffset = func_02074a88(fileId);
    return TRUE;
}

void func_02077110(UnkSndArcStrm* strm) {
    func_0206112c(&strm->file);
}

s32 func_02077120(UnkSndArcStrm* strm, void* dst, s32 size, u32 offset) {
    func_02061280(&strm->file, strm->fileOffset + offset, 0);
    return func_02061270(&strm->file, dst, size);
}

void func_02077158(UnkSndArcStrm* strm) {
    func_02061228(&strm->file);
}

BOOL func_02077168(UnkSndArcStrm* strm, u32 fileId) {
    strm->fileOffset = (u32)func_02074b70(fileId);
    MI_CpuCopyU8((void*)strm->fileOffset, strm->header, 0x40);
    return TRUE;
}

void func_02077190(UnkSndArcStrm* strm) {
}

s32 func_02077194(UnkSndArcStrm* strm, void* dst, s32 size, u32 offset) {
    MI_CpuCopyU8((void*)(strm->fileOffset + offset), dst, size);
    return size;
}

void func_020771b0(UnkSndArcStrm* strm) {
}

void func_020771b4(void* arg) {
    UnkSndArcStrmThread* thread = arg;

    while (TRUE) {
        OS_PauseThread(&thread->queue);

        while (TRUE) {
            UnkSndArcStrmRequest* request;

            OS_LockMutex(&thread->mutex);
            request = func_02076420(&thread->requestList);
            if (request == NULL) {
                OS_UnlockMutex(&thread->mutex);
                break;
            }

            func_020767f4(request);
            func_020764b0(request);
            OS_UnlockMutex(&thread->mutex);
        }
    }
}
