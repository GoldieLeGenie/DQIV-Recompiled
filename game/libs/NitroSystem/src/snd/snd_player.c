// Sequence player management (second half of the player module).
#include "snd_internal.h"

typedef struct UnkSndFader {
    /* 0x00 */ s32 origin;
    /* 0x04 */ s32 target;
    /* 0x08 */ s32 counter;
    /* 0x0C */ s32 frames;
} UnkSndFader;

struct UnkSndHandle;
struct UnkSndPlayer;
struct UnkSndPlayerHeap;

typedef struct UnkSndSeqPlayer {
    /* 0x00 */ struct UnkSndHandle* handle;
    /* 0x04 */ struct UnkSndPlayer* player;
    /* 0x08 */ struct UnkSndPlayerHeap* heap;
    /* 0x0C */ UnkLink prioLink;
    /* 0x14 */ UnkLink playerLink;
    /* 0x1C */ UnkSndFader fader;
    /* 0x2C */ u8 status;
    /* 0x2D */ u8 unk_2d;
    /* 0x2E */ u8 unk_2e;
    /* 0x2F */ u8 unk_2f;
    /* 0x30 */ u32 commandTag;
    /* 0x34 */ u16 unk_34;
    /* 0x36 */ u8 unk_36[6];
    /* 0x3C */ u8 playerNo;
    /* 0x3D */ u8 prio;
    /* 0x3E */ u16 unk_3e;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
} UnkSndSeqPlayer;

typedef struct UnkSndHandle {
    /* 0x00 */ UnkSndSeqPlayer* player;
} UnkSndHandle;

typedef struct UnkSndPlayer {
    /* 0x00 */ UnkList playerList;
    /* 0x0C */ UnkList heapList;
    /* 0x18 */ u32 playableSeqCount;
    /* 0x1C */ u32 allocChBitFlag;
    /* 0x20 */ u32 unk_20;
} UnkSndPlayer; // size 0x24

typedef struct UnkSndPlayerHeap {
    /* 0x00 */ UnkLink link;
    /* 0x08 */ void* heap;
    /* 0x0C */ UnkSndSeqPlayer* seqPlayer;
    /* 0x10 */ int playerNo;
} UnkSndPlayerHeap;

extern UnkList data_021122ec;           // free seq player list
extern UnkList data_021122f8;           // priority ordered seq player list
extern UnkSndPlayer data_02112744[];    // players

void func_02073458(UnkSndHandle* handle);
void func_02074c48(void* heap);
void func_02074c60(void* heap);
void func_0207725c(UnkSndFader* fader, s32 target, s32 frames);
void func_02077244(UnkSndFader* fader);

void func_02073854(UnkSndSeqPlayer* seqPlayer, const void* seq, u32 offset, const void* bank);
void func_020738a4(UnkSndSeqPlayer* seqPlayer, int fadeFrame);
void* func_020738f0(int playerNo, UnkSndSeqPlayer* seqPlayer);
void func_02073944(UnkSndSeqPlayer* seqPlayer);
void func_0207398c(UnkSndPlayer* player, UnkSndSeqPlayer* seqPlayer);
void func_020739dc(UnkSndSeqPlayer* seqPlayer);
void func_02073a30(UnkSndSeqPlayer* seqPlayer);
UnkSndSeqPlayer* func_02073a68(int prio);
void func_02073ad0(UnkSndSeqPlayer* seqPlayer);
void func_02073b54(UnkSndPlayerHeap* heap);
void func_02073ba4(UnkSndSeqPlayer* seqPlayer, int prio);

UnkSndSeqPlayer* func_020737b0(UnkSndHandle* handle, int playerNo, int prio) {
    UnkSndPlayer* player = &data_02112744[playerNo];
    UnkSndSeqPlayer* seqPlayer;

    if (handle->player != NULL) {
        func_02073458(handle);
    }

    if (player->playerList.numObjects >= player->playableSeqCount) {
        seqPlayer = func_02067f98(&player->playerList, NULL);
        if (seqPlayer == NULL) {
            return NULL;
        }
        if (prio < seqPlayer->prio) {
            return NULL;
        }
        func_02073a30(seqPlayer);
    }

    seqPlayer = func_02073a68(prio);
    if (seqPlayer == NULL) {
        return NULL;
    }

    func_0207398c(player, seqPlayer);
    seqPlayer->handle = handle;
    handle->player = seqPlayer;
    return seqPlayer;
}

void func_02073848(UnkSndSeqPlayer* seqPlayer) {
    func_02073ad0(seqPlayer);
}

void func_02073854(UnkSndSeqPlayer* seqPlayer, const void* seq, u32 offset, const void* bank) {
    UnkSndPlayer* player = seqPlayer->player;

    func_0207a3b0(seqPlayer->playerNo, seq, offset, bank);
    if (player->allocChBitFlag != 0) {
        func_0207a428(seqPlayer->playerNo, 0xFFFF, player->allocChBitFlag);
    }
    func_02073944(seqPlayer);
    seqPlayer->commandTag = func_0207ac10();
    seqPlayer->unk_2f = 1;
    seqPlayer->status = 1;
}

void func_020738a4(UnkSndSeqPlayer* seqPlayer, int fadeFrame) {
    if (seqPlayer == NULL || seqPlayer->status == 0) {
        return;
    }

    if (fadeFrame == 0) {
        func_02073a30(seqPlayer);
        return;
    }

    func_0207725c(&seqPlayer->fader, 0, fadeFrame);
    func_02073ba4(seqPlayer, 0);
    seqPlayer->status = 2;
}

void* func_020738f0(int playerNo, UnkSndSeqPlayer* seqPlayer) {
    UnkSndPlayer* player = &data_02112744[playerNo];
    UnkSndPlayerHeap* heap;

    heap = func_02067f98(&player->heapList, NULL);
    if (heap == NULL) {
        return NULL;
    }
    func_02067f38(&player->heapList, heap);
    heap->seqPlayer = seqPlayer;
    seqPlayer->heap = heap;
    func_02074c60(heap->heap);
    return heap->heap;
}

void func_02073944(UnkSndSeqPlayer* seqPlayer) {
    seqPlayer->unk_2e = 0;
    seqPlayer->unk_2d = 0;
    seqPlayer->unk_2f = 0;
    seqPlayer->unk_34 = 0;
    seqPlayer->unk_3e = 0;
    seqPlayer->unk_40 = 0x7F;
    seqPlayer->unk_41 = 0x7F;
    func_02077244(&seqPlayer->fader);
    func_0207725c(&seqPlayer->fader, 0x7F00, 1);
}

void func_0207398c(UnkSndPlayer* player, UnkSndSeqPlayer* seqPlayer) {
    UnkSndSeqPlayer* next = func_02067f98(&player->playerList, NULL);

    while (next != NULL) {
        if (seqPlayer->prio < next->prio) {
            break;
        }
        next = func_02067f98(&player->playerList, next);
    }

    func_02067ed4(&player->playerList, next, seqPlayer);
    seqPlayer->player = player;
}

void func_020739dc(UnkSndSeqPlayer* seqPlayer) {
    UnkSndSeqPlayer* next = func_02067f98(&data_021122f8, NULL);

    while (next != NULL) {
        if (seqPlayer->prio < next->prio) {
            break;
        }
        next = func_02067f98(&data_021122f8, next);
    }

    func_02067ed4(&data_021122f8, next, seqPlayer);
}

void func_02073a30(UnkSndSeqPlayer* seqPlayer) {
    if (seqPlayer->status == 2) {
        func_0207a3f8(seqPlayer->playerNo, -723);
    }
    func_0207a390(seqPlayer->playerNo);
    func_02073ad0(seqPlayer);
}

UnkSndSeqPlayer* func_02073a68(int prio) {
    UnkSndSeqPlayer* seqPlayer = func_02067f98(&data_021122ec, NULL);

    if (seqPlayer == NULL) {
        seqPlayer = func_02067f98(&data_021122f8, NULL);
        if (prio < seqPlayer->prio) {
            return NULL;
        }
        func_02073a30(seqPlayer);
    }

    func_02067f38(&data_021122ec, seqPlayer);
    seqPlayer->prio = prio;
    func_020739dc(seqPlayer);
    return seqPlayer;
}

void func_02073ad0(UnkSndSeqPlayer* seqPlayer) {
    UnkSndPlayer* player;

    if (seqPlayer->handle != NULL) {
        seqPlayer->handle->player = NULL;
        seqPlayer->handle = NULL;
    }

    player = seqPlayer->player;
    func_02067f38(&player->playerList, seqPlayer);
    seqPlayer->player = NULL;

    if (seqPlayer->heap != NULL) {
        func_02067e30(&player->heapList, seqPlayer->heap);
        seqPlayer->heap->seqPlayer = NULL;
        seqPlayer->heap = NULL;
    }

    func_02067f38(&data_021122f8, seqPlayer);
    func_02067e30(&data_021122ec, seqPlayer);
    seqPlayer->status = 0;
}

void func_02073b54(UnkSndPlayerHeap* heap) {
    if (heap->heap == NULL) {
        return;
    }

    func_02074c48(heap->heap);

    if (heap->seqPlayer != NULL) {
        heap->seqPlayer->heap = NULL;
    } else {
        func_02067f38(&data_02112744[heap->playerNo].heapList, heap);
    }
}

void func_02073ba4(UnkSndSeqPlayer* seqPlayer, int prio) {
    UnkSndPlayer* player = seqPlayer->player;

    if (player != NULL) {
        func_02067f38(&player->playerList, seqPlayer);
        seqPlayer->player = NULL;
    }

    func_02067f38(&data_021122f8, seqPlayer);
    seqPlayer->prio = prio;

    if (player != NULL) {
        func_0207398c(player, seqPlayer);
    }

    func_020739dc(seqPlayer);
}
