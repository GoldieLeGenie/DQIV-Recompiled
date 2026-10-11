#include "snd_internal.h"
#include <nitro/snd/wavearc.h>
#ifdef DQIV_NATIVE
void pcSndMarkWaveAddress(const u32* slot);
const void* pcSndResolveWaveAddress(const void* arc, const u32* slot);
void pcSndForgetWaveAddresses(const u32* first, u32 count);
#endif

struct UnkSndBankData;

typedef struct UnkSndBankData {
    u8 fileHeader[0x10];
    u8 blockHeader[8];
    UnkSndWaveArcLink waveArcLink[4]; // 0x18
    u32 instCount;                    // 0x38
    u32 instOffset[1];                // 0x3c
} UnkSndBankData;

typedef struct UnkSndInstParam {
    u16 wave[2];
    u8 originalKey;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 pan;
} UnkSndInstParam;

typedef struct UnkSndInstData {
    u8 type;
    u8 padding;
    UnkSndInstParam param;
} UnkSndInstData;

typedef struct UnkSndDrumSet {
    u8 min;
    u8 max;
    UnkSndInstData instOffset[1];
} UnkSndDrumSet;

typedef struct UnkSndKeySplit {
    u8 key[8];
    UnkSndInstData instOffset[1];
} UnkSndKeySplit;

typedef struct UnkSndInstPos {
    u32 prgNo;
    u32 index;
} UnkSndInstPos;

#define SND_BANK_HEADER_SIZE 0x3c

void func_0207b094(UnkSndBankData* bank, s32 index, UnkSndWaveArc* waveArc) {
    func_0207a71c();

    if (bank->waveArcLink[index].waveArc != NULL) {
        if (waveArc == bank->waveArcLink[index].waveArc) {
            func_0207a730();
            return;
        }
        if (&bank->waveArcLink[index] == bank->waveArcLink[index].waveArc->topLink) {
            bank->waveArcLink[index].waveArc->topLink = bank->waveArcLink[index].next;
            DC_CleanRange(bank->waveArcLink[index].waveArc, SND_BANK_HEADER_SIZE);
        } else {
            UnkSndWaveArcLink* link;
            for (link = bank->waveArcLink[index].waveArc->topLink; link != NULL; link = link->next) {
                if (&bank->waveArcLink[index] == link->next) {
                    break;
                }
            }
            link->next = bank->waveArcLink[index].next;
            DC_CleanRange(link, sizeof(UnkSndWaveArcLink));
        }
    }

    {
        UnkSndWaveArcLink* next = waveArc->topLink;
        waveArc->topLink = &bank->waveArcLink[index];
        bank->waveArcLink[index].next = next;
        bank->waveArcLink[index].waveArc = waveArc;
    }

    func_0207a730();
    DC_CleanRange(bank, SND_BANK_HEADER_SIZE);
    DC_CleanRange(waveArc, SND_BANK_HEADER_SIZE);
}

void func_0207b160(UnkSndBankData* bank) {
    s32 i;

    func_0207a71c();
    for (i = 0; i < 4; i++) {
        UnkSndWaveArc* waveArc = bank->waveArcLink[i].waveArc;
        if (waveArc == NULL) {
            continue;
        }
        if (&bank->waveArcLink[i] == waveArc->topLink) {
            waveArc->topLink = bank->waveArcLink[i].next;
            DC_CleanRange(waveArc, SND_BANK_HEADER_SIZE);
        } else {
            UnkSndWaveArcLink* link;
            for (link = waveArc->topLink; link != NULL; link = link->next) {
                if (&bank->waveArcLink[i] == link->next) {
                    break;
                }
            }
            link->next = bank->waveArcLink[i].next;
            DC_CleanRange(link, sizeof(UnkSndWaveArcLink));
        }
    }
    func_0207a730();
}

void func_0207b1f8(UnkSndWaveArc* waveArc) {
    UnkSndWaveArcLink* link;

    func_0207a71c();
#ifdef DQIV_NATIVE
    pcSndForgetWaveAddresses(waveArc->waveOffset, waveArc->waveCount);
#endif
    link = waveArc->topLink;
    while (link != NULL) {
        UnkSndWaveArcLink* next = link->next;
        link->waveArc = NULL;
        link->next = NULL;
        DC_CleanRange(link, sizeof(UnkSndWaveArcLink));
        link = next;
    }
    func_0207a730();
}

UnkSndInstPos func_0207b240(const UnkSndBankData* bank) {
    UnkSndInstPos pos;
    pos.prgNo = 0;
    pos.index = 0;
    return pos;
}

BOOL func_0207b260(const UnkSndBankData* bank, UnkSndInstData* inst, UnkSndInstPos* pos) {
    while (pos->prgNo < bank->instCount) {
        u32 instOffset = bank->instOffset[pos->prgNo];
        u32 offset;

        inst->type = (u8)instOffset;
        offset = instOffset >> 8;
        switch (inst->type) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            inst->param = *(const UnkSndInstParam*)((u8*)bank + offset);
            pos->prgNo++;
            return TRUE;
        case 16: {
            const UnkSndDrumSet* drumSet = (const UnkSndDrumSet*)((u8*)bank + offset);
            while (pos->index < drumSet->max - drumSet->min + 1) {
                *inst = drumSet->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        case 17: {
            const UnkSndKeySplit* keySplit = (const UnkSndKeySplit*)((u8*)bank + offset);
            while (pos->index < 8) {
                if (keySplit->key[pos->index] == 0) {
                    break;
                }
                *inst = keySplit->instOffset[pos->index];
                pos->index++;
                return TRUE;
            }
            break;
        }
        }
        pos->prgNo++;
        pos->index = 0;
    }
    return FALSE;
}

u32 func_0207b410(const UnkSndWaveArc* waveArc) {
    return waveArc->waveCount;
}

void func_0207b418(UnkSndWaveArc* waveArc, s32 index, const void* address) {
    func_0207a71c();
    waveArc->waveOffset[index] = (u32)address;
#ifdef DQIV_NATIVE
    pcSndMarkWaveAddress(&waveArc->waveOffset[index]);
#endif
    DC_CleanRange(&waveArc->waveOffset[index], sizeof(u32));
    func_0207a730();
}

const void* func_0207b44c(const UnkSndWaveArc* waveArc, s32 index) {
    const void* wave;

    func_0207a71c();
#ifdef DQIV_NATIVE
    wave = pcSndResolveWaveAddress(waveArc, &waveArc->waveOffset[index]);
#else
    wave = (const void*)waveArc->waveOffset[index];
    if (wave != NULL) {
        if ((u32)wave < 0x02000000) {
            wave = (const void*)((u32)waveArc + (u32)wave);
        }
    } else {
        wave = NULL;
    }
#endif
    func_0207a730();
    return wave;
}
