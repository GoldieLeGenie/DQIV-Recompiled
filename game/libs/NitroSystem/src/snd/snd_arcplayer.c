// Sound archive player: player setup from the archive and sequence start helpers.
#include "snd_internal.h"
#ifdef DQIV_NATIVE
void pcSndRegisterSequence(const void* file, u32 offset, u32 size);
#endif

typedef struct UnkSndPlayerInfo {
    /* 0x00 */ u8 seqMax;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 allocChBitFlag;
    /* 0x04 */ u32 heapSize;
} UnkSndPlayerInfo;

typedef struct UnkSndSeqParam {
    /* 0x00 */ u32 fileId; // offset inside the archive for a sequence archive entry
    /* 0x04 */ u16 bankNo;
    /* 0x06 */ u8 volume;
    /* 0x07 */ u8 channelPrio;
    /* 0x08 */ u8 playerPrio;
    /* 0x09 */ u8 playerNo;
} UnkSndSeqParam;

typedef struct UnkSndSeqData {
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u32 dataOffset;
} UnkSndSeqData;

void* func_020747bc(void);
void* func_020747cc(int index);
void* func_02074830(int index);
void* func_020749c0(int index);
void* func_02074b70(u32 fileId);
int   func_02074ea8(int seqNo, u32 flags, void* heap, BOOL addToArc, void** pData);
int   func_02074f8c(int bankNo, u32 flags, void* heap, BOOL addToArc, void** pData);
UnkSndSeqParam* func_02077204(void* seqArc, int index);
void* func_020737b0(void* handle, int playerNo, int prio);
void  func_02073848(void* seqPlayer);
void  func_02073854(void* seqPlayer, const void* seq, u32 offset, const void* bank);
void* func_020738f0(int playerNo, void* seqPlayer);
void  func_02073370(int playerNo, int seqMax);
void  func_02073390(int playerNo, u32 allocChBitFlag);
BOOL  func_020733a8(int playerNo, void* heap, u32 size);
void  func_020734e0(void* handle, int volume);
void  func_020734f4(void* handle, int prio);
void  func_02073514(void* handle, int seqNo);
void  func_02073538(void* handle, int seqArcNo, int index);

BOOL func_020758e4(void* handle, int playerNo, int bankNo, int playerPrio, UnkSndSeqParam* param, int seqNo);
BOOL func_020759e0(void* handle, int playerNo, int bankNo, int playerPrio, UnkSndSeqParam* param, UnkSndSeqData* seqArc, int seqArcNo, int index);

BOOL func_02075780(void* heap) {
    int i;

    func_020747bc();

    for (i = 0; i < 32; i++) {
        UnkSndPlayerInfo* info = func_020749c0(i);
        if (info == NULL) {
            continue;
        }

        func_02073370(i, info->seqMax);
        func_02073390(i, info->allocChBitFlag);

        if (info->heapSize != 0 && heap != NULL) {
            int j;
            for (j = 0; j < info->seqMax; j++) {
                if (!func_020733a8(i, heap, info->heapSize)) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

BOOL func_0207581c(void* handle, int seqNo) {
    UnkSndSeqParam* info = func_020747cc(seqNo);

    if (info == NULL) {
        return FALSE;
    }
    return func_020758e4(handle, info->playerNo, info->bankNo, info->playerPrio, info, seqNo);
}

BOOL func_02075864(void* handle, int seqArcNo, int index) {
    u32* info = func_02074830(seqArcNo);
    UnkSndSeqData* seqArc;
    UnkSndSeqParam* param;

    if (info == NULL) {
        return FALSE;
    }

    seqArc = func_02074b70(*info);
    if (seqArc == NULL) {
        return FALSE;
    }

    param = func_02077204(seqArc, index);
    if (param == NULL) {
        return FALSE;
    }

    return func_020759e0(handle, param->playerNo, param->bankNo, param->playerPrio, param, seqArc, seqArcNo, index);
}

BOOL func_020758e4(void* handle, int playerNo, int bankNo, int playerPrio, UnkSndSeqParam* param, int seqNo) {
    void* seqPlayer;
    void* heap;
    UnkSndSeqData* seq;
    void* bank;

    seqPlayer = func_020737b0(handle, playerNo, playerPrio);
    if (seqPlayer == NULL) {
        return FALSE;
    }

    heap = func_020738f0(playerNo, seqPlayer);

    if (func_02074f8c(bankNo, 6, heap, FALSE, &bank) != 0) {
        func_02073848(seqPlayer);
        return FALSE;
    }

    if (func_02074ea8(seqNo, 1, heap, FALSE, (void**)&seq) != 0) {
        func_02073848(seqPlayer);
        return FALSE;
    }

#ifdef DQIV_NATIVE
    pcSndRegisterSequence(seq, seq->dataOffset, *(const u32*)((const u8*)seq + 8));
#endif
    func_02073854(seqPlayer, (u8*)seq + seq->dataOffset, 0, bank);
    func_020734e0(handle, param->volume);
    func_020734f4(handle, param->channelPrio);
    func_02073514(handle, seqNo);
    return TRUE;
}

BOOL func_020759e0(void* handle, int playerNo, int bankNo, int playerPrio, UnkSndSeqParam* param, UnkSndSeqData* seqArc, int seqArcNo, int index) {
    void* seqPlayer;
    void* heap;
    void* bank;

    seqPlayer = func_020737b0(handle, playerNo, playerPrio);
    if (seqPlayer == NULL) {
        return FALSE;
    }

    heap = func_020738f0(playerNo, seqPlayer);

    if (func_02074f8c(bankNo, 6, heap, FALSE, &bank) != 0) {
        func_02073848(seqPlayer);
        return FALSE;
    }

#ifdef DQIV_NATIVE
    pcSndRegisterSequence(seqArc, seqArc->dataOffset, *(const u32*)((const u8*)seqArc + 8));
#endif
    func_02073854(seqPlayer, (u8*)seqArc + seqArc->dataOffset, param->fileId, bank);
    func_020734e0(handle, param->volume);
    func_020734f4(handle, param->channelPrio);
    func_02073538(handle, seqArcNo, index);
    return TRUE;
}
