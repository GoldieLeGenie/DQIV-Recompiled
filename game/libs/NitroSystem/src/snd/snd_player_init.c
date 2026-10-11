#include "../nns_internal.h"

typedef struct {
    void* prevObject;                       /* 0x00 */
    void* nextObject;                       /* 0x04 */
} UnkFndLink;

typedef struct {
    void* headObject;                       /* 0x00 */
    void* tailObject;                       /* 0x04 */
    u16 numObjects;                         /* 0x08 */
    u16 offset;                             /* 0x0A */
} UnkFndList;

typedef struct {
    u8 unk_00[0x10];                        /* 0x00 */
} UnkSndFader;

typedef struct {
    u8 unk_00[0x20];
    u8 volume;                              /* 0x20 */
} UnkSndPlayerGroup;

struct UnkSndHandle;

typedef struct UnkSndSeqPlayer {
    struct UnkSndHandle* handle;            /* 0x00 */
    UnkSndPlayerGroup* player;              /* 0x04 */
    u8 unk_08[0x14 - 0x08];                 /* 0x08 */
    UnkFndLink prioLink;                    /* 0x14 */
    UnkSndFader fader;                      /* 0x1C */
    u8 status;                              /* 0x2C */
    u8 startedFlag;                         /* 0x2D */
    u8 unk_2e;                              /* 0x2E */
    u8 updateFlag;                          /* 0x2F */
    u32 commandTag;                         /* 0x30 */
    u16 seqType;                            /* 0x34 */
    u16 unk_36;                             /* 0x36 */
    u16 seqNo;                              /* 0x38 */
    u16 seqArcIndex;                        /* 0x3A */
    u8 playerNo;                            /* 0x3C */
    u8 unk_3d;                              /* 0x3D */
    s16 extVolume;                          /* 0x3E */
    u8 initVolume;                          /* 0x40 */
    u8 volume;                              /* 0x41 */
    u8 unk_42[2];                           /* 0x42 */
} UnkSndSeqPlayer;

typedef struct UnkSndHandle {
    UnkSndSeqPlayer* volatile player;       /* 0x00 */
} UnkSndHandle;

typedef struct {
    UnkFndList playerList;                  /* 0x00 */
    UnkFndList heapList;                    /* 0x0C */
    u32 playableSeqCount;                   /* 0x18 */
    u32 allocSize;                          /* 0x1C */
    u8 volume;                              /* 0x20 */
    u8 unk_21[3];                           /* 0x21 */
} UnkSndPlayerGroupState;

typedef struct {
    UnkFndLink link;                        /* 0x00 */
    void* heap;                             /* 0x08 */
    u32 unk_0c;                             /* 0x0C */
    int playerNo;                           /* 0x10 */
} UnkSndPlayerHeap;

UnkFndList data_021122ec;               /* free player list */
UnkFndList data_021122f8;               /* priority player list */
UnkSndSeqPlayer data_02112304[16];
UnkSndPlayerGroupState data_02112744[32];
extern const s16 data_020ba8ac[];

void func_02067dec(UnkFndList* list, u16 offset);
void func_02067e30(UnkFndList* list, void* object);
void* func_02067f98(UnkFndList* list, void* object);
void* func_02074d1c(void* heap, u32 size, void (*callback)(void*, u32, u32, u32), u32 data1, u32 data2);
void* func_02074bd8(void* buffer, u32 size);
void func_02073b54(void* mem, u32 size, u32 data1, u32 data2);
void func_020738a4(UnkSndSeqPlayer* player);
void func_0207a410(u32 playerNo, int fadeFrame);
BOOL func_0207af44(u32 playerNo);
u32 func_0207af18(void);
BOOL func_0207ac3c(u32 tag);
void func_02073ad0(UnkSndSeqPlayer* player);
void func_020772b8(UnkSndFader* fader);
int func_02077284(UnkSndFader* fader);
BOOL func_020772d0(UnkSndFader* fader);
void func_0207a3f8(u32 playerNo, int volume);
void func_02073a30(UnkSndSeqPlayer* player);
void func_0207a3d8(u32 playerNo);

void func_02073370(int playerNo, int seqCount)
{
    data_02112744[playerNo].playableSeqCount = (u16)seqCount;
}

void func_02073390(int playerNo, u32 size)
{
    data_02112744[playerNo].allocSize = size;
}

BOOL func_020733a8(int playerNo, void* heap, u32 size)
{
    UnkSndPlayerHeap* playerHeap;
    void* buffer;

    playerHeap = func_02074d1c(heap, size + sizeof(UnkSndPlayerHeap), func_02073b54, 0, 0);
    if (playerHeap == NULL) {
        return FALSE;
    }
    playerHeap->unk_0c = 0;
    playerHeap->playerNo = playerNo;
    playerHeap->heap = NULL;

    buffer = func_02074bd8(playerHeap + 1, size);
    if (buffer == NULL) {
        return FALSE;
    }
    playerHeap->heap = buffer;
    func_02067e30(&data_02112744[playerNo].heapList, playerHeap);
    return TRUE;
}

void func_0207343c(UnkSndHandle* handle)
{
    func_020738a4(handle->player);
}

void func_0207344c(UnkSndHandle* handle)
{
    handle->player = NULL;
}

void func_02073458(UnkSndHandle* handle)
{
    if (handle->player == NULL) {
        return;
    }
    handle->player->handle = NULL;
    handle->player = NULL;
}

int func_02073478(u16 seqNo)
{
    int count = 0;
    UnkSndSeqPlayer* player = func_02067f98(&data_021122f8, NULL);

    while (player != NULL) {
        if (player->seqType == 1 && player->seqNo == seqNo) {
            count++;
        }
        player = func_02067f98(&data_021122f8, player);
    }
    return count;
}

void func_020734cc(UnkSndHandle* handle, u8 volume)
{
    if (handle->player != NULL) {
        handle->player->volume = volume;
    }
}

void func_020734e0(UnkSndHandle* handle, u8 volume)
{
    if (handle->player != NULL) {
        handle->player->initVolume = volume;
    }
}

void func_020734f4(UnkSndHandle* handle, int fadeFrame)
{
    if (handle->player == NULL) {
        return;
    }
    func_0207a410(handle->player->playerNo, fadeFrame);
}

void func_02073514(UnkSndHandle* handle, u16 seqNo)
{
    if (handle->player == NULL) {
        return;
    }
    handle->player->seqType = 1;
    handle->player->seqNo = seqNo;
}

void func_02073538(UnkSndHandle* handle, u16 seqArcNo, u16 index)
{
    if (handle->player == NULL) {
        return;
    }
    handle->player->seqType = 2;
    handle->player->seqNo = seqArcNo;
    handle->player->seqArcIndex = index;
}

BOOL func_02073564(UnkSndHandle* handle)
{
    UnkSndSeqPlayer* player;

    if (handle->player == NULL) {
        return FALSE;
    }
    player = handle->player;
    if (!player->startedFlag) {
        return FALSE;
    }
    return func_0207af44(player->playerNo);
}

void func_02073598(void)
{
    int i;
    UnkSndSeqPlayer* player;
    UnkSndPlayerGroupState* group;

    func_02067dec(&data_021122f8, 0x14);
    func_02067dec(&data_021122ec, 0x14);

    player = data_02112304;
    for (i = 0; i < 16; i++, player++) {
        player->status = 0;
        player->playerNo = i;
        func_02067e30(&data_021122ec, player);
    }

    group = data_02112744;
    for (i = 0; i < 32; i++, group++) {
        func_02067dec(&group->playerList, 0xc);
        func_02067dec(&group->heapList, 0);
        group->volume = 0x7f;
        group->playableSeqCount = 1;
        group->allocSize = 0;
    }
}

void func_0207364c(void)
{
    u32 status = func_0207af18();
    UnkSndSeqPlayer* player = func_02067f98(&data_021122f8, NULL);
    UnkSndSeqPlayer* next;
    int volume;
    int seqVol, initVol, groupVol;

    while (player != NULL) {
        next = func_02067f98(&data_021122f8, player);

        if (!player->startedFlag) {
            if (func_0207ac3c(player->commandTag)) {
                player->startedFlag = TRUE;
            }
        }

        if (player->startedFlag && !(status & (1 << player->playerNo))) {
            func_02073ad0(player);
        } else {
            func_020772b8(&player->fader);
            seqVol = data_020ba8ac[player->volume];
            initVol = data_020ba8ac[player->initVolume];
            groupVol = data_020ba8ac[player->player->volume];
            volume = data_020ba8ac[func_02077284(&player->fader) >> 8];
            volume += initVol + seqVol + groupVol;
            if (volume < -0x8000) {
                volume = -0x8000;
            } else if (volume > 0x7fff) {
                volume = 0x7fff;
            }
            if (volume != player->extVolume) {
                func_0207a3f8(player->playerNo, volume);
                player->extVolume = volume;
            }
            if (player->status == 2 && func_020772d0(&player->fader)) {
                func_02073a30(player);
            }
            if (player->updateFlag) {
                func_0207a3d8(player->playerNo);
                player->updateFlag = FALSE;
            }
        }
        player = next;
    }
}
