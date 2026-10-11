#include "os_internal.h"
#include <nitro/os/thread.h>

#define MESSAGE_BLOCK 1

typedef void* UnkMessage;

typedef struct UnkMessageQueue {
    /* 0x00 */ OSThreadQueue queueSend;
    /* 0x08 */ OSThreadQueue queueReceive;
    /* 0x10 */ UnkMessage*   msgArray;
    /* 0x14 */ s32           msgCount;
    /* 0x18 */ s32           firstIndex;
    /* 0x1c */ s32           usedCount;
} UnkMessageQueue; // size 0x20

void func_02078d60(UnkMessageQueue* mq, UnkMessage* msgArray, s32 msgCount) {
    OS_InitThreadQueue(&mq->queueSend);
    OS_InitThreadQueue(&mq->queueReceive);
    mq->msgArray   = msgArray;
    mq->msgCount   = msgCount;
    mq->firstIndex = 0;
    mq->usedCount  = 0;
}

// Sends a message (appends at the end)
BOOL func_02078d88(UnkMessageQueue* mq, UnkMessage msg, s32 flags) {
    u32 prev = OS_DisableIRQ();
    s32 lastIndex;

    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & MESSAGE_BLOCK)) {
            OS_RestoreIRQ(prev);
            return FALSE;
        }
        OS_PauseThread(&mq->queueSend);
    }

    lastIndex                = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    mq->msgArray[lastIndex]  = msg;
    mq->usedCount++;

    OS_UnpauseThread(&mq->queueReceive);
    OS_RestoreIRQ(prev);
    return TRUE;
}

// Receives a message
BOOL func_02078e1c(UnkMessageQueue* mq, UnkMessage* msg, s32 flags) {
    u32 prev = OS_DisableIRQ();

    while (mq->usedCount == 0) {
        if (!(flags & MESSAGE_BLOCK)) {
            OS_RestoreIRQ(prev);
            return FALSE;
        }
        OS_PauseThread(&mq->queueReceive);
    }

    if (msg != NULL) {
        *msg = mq->msgArray[mq->firstIndex];
    }
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;

    OS_UnpauseThread(&mq->queueSend);
    OS_RestoreIRQ(prev);
    return TRUE;
}

// Sends a message (inserts at the front)
BOOL func_02078ec0(UnkMessageQueue* mq, UnkMessage msg, s32 flags) {
    u32 prev = OS_DisableIRQ();

    while (mq->msgCount <= mq->usedCount) {
        if (!(flags & MESSAGE_BLOCK)) {
            OS_RestoreIRQ(prev);
            return FALSE;
        }
        OS_PauseThread(&mq->queueSend);
    }

    mq->firstIndex                 = (mq->firstIndex + mq->msgCount - 1) % mq->msgCount;
    mq->msgArray[mq->firstIndex]   = msg;
    mq->usedCount++;

    OS_UnpauseThread(&mq->queueReceive);
    OS_RestoreIRQ(prev);
    return TRUE;
}
