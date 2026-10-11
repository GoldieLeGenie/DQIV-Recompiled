#include "snd_internal.h"

UnkSndSharedWork* data_02116100;

u32 func_0207af18(void) {
    DC_InvalidateRange((void*)&data_02116100->playerStatus, 4);
    return data_02116100->playerStatus;
}

u32 func_0207af44(s32 playerNo) {
    DC_InvalidateRange((void*)&data_02116100->player[playerNo].tickCounter, 4);
    return data_02116100->player[playerNo].tickCounter;
}

u32 func_0207af80(void) {
    DC_InvalidateRange((void*)&data_02116100->finishCommandTag, 4);
    return data_02116100->finishCommandTag;
}

void func_0207afa8(UnkSndSharedWork* work) {
    s32 i;
    s32 j;

    work->playerStatus = 0;
    work->channelStatus = 0;
    work->captureStatus = 0;
    work->finishCommandTag = 0;
    for (i = 0; i < 16; i++) {
        work->player[i].tickCounter = 0;
        for (j = 0; j < 16; j++) {
            work->player[i].variable[j] = -1;
        }
    }
    for (i = 0; i < 16; i++) {
        work->globalVariable[i] = -1;
    }
    DC_PurgeRange(work, sizeof(UnkSndSharedWork));
}
