// Sound archive loader: loads sequences, banks and wave archives into a sound heap.
#include "snd_internal.h"
#include <nitro/snd/wavearc.h>

typedef void (*UnkSndHeapCallback)(void* mem, u32 size, void* data1, u32 data2);

typedef struct UnkSndSeqInfo {
    /* 0x00 */ u32 fileId;
    /* 0x04 */ u16 bankNo;
} UnkSndSeqInfo;

typedef struct UnkSndSeqArcInfo {
    /* 0x00 */ u32 fileId;
} UnkSndSeqArcInfo;

typedef struct UnkSndBankInfo {
    /* 0x00 */ u32 fileId;
    /* 0x04 */ u16 waveArcNo[4];
} UnkSndBankInfo;

typedef struct UnkSndWaveArcInfo {
    /* 0x00 */ u32 fileId : 24;
    /* 0x00 */ u32 flags : 8;
} UnkSndWaveArcInfo;

typedef struct UnkSndInstPos {
    /* 0x00 */ u32 prgNo;
    /* 0x04 */ u32 index;
} UnkSndInstPos;

typedef struct UnkSndInstData {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 waveNo;
    /* 0x04 */ u16 waveArcNo;
    /* 0x06 */ u8 unk_06[6];
} UnkSndInstData;


void* func_020747a4(void* arc);
void* func_020747bc(void);
void* func_020747cc(int index);
void* func_02074830(int index);
void* func_02074894(int index);
void* func_020748f8(int index);
u32   func_02074ab0(u32 fileId);
s32   func_02074ad8(u32 fileId, void* dst, u32 size, u32 offset);
void* func_02074b70(u32 fileId);
void  func_02074b98(u32 fileId, void* mem);
void* func_02074d1c(void* heap, u32 size, UnkSndHeapCallback callback, void* data1, u32 data2);
UnkSndInstPos func_0207b240(void);
BOOL  func_0207b260(void* bank, UnkSndInstData* inst, UnkSndInstPos* pos);

int   func_02074ea8(int seqNo, u32 flags, void* heap, BOOL addToArc, void** pData);
int   func_02074f2c(int seqArcNo, u32 flags, void* heap, BOOL addToArc, void** pData);
int   func_02074f8c(int bankNo, u32 flags, void* heap, BOOL addToArc, void** pData);
int   func_020750cc(int waveArcNo, u32 flags, void* heap, BOOL addToArc, void** pData);
void* func_02075158(u32 fileId, UnkSndHeapCallback callback, void* data1, u32 data2, void* heap);
void* func_020751f8(u32 fileId, void* heap, BOOL addToArc);
void* func_02075264(u32 fileId, void* heap, BOOL addToArc);
void* func_020752d0(u32 fileId, void* heap, BOOL addToArc);
void* func_0207533c(u32 fileId, void* heap, BOOL addToArc);
void* func_020753a8(u32 fileId, void* heap, BOOL addToArc);
void  func_020754b0(void* mem, void* arc, u32 fileId);
void  func_02075508(void* mem, u32 size, void* data1, u32 data2);
void  func_02075530(void* mem, u32 size, void* data1, u32 data2);
void  func_02075560(void* mem, u32 size, void* data1, u32 data2);
void  func_02075590(void* mem, u32 size, void* data1, u32 data2);
void  func_020755b0(void* mem, u32 size, void* data1, u32 data2);
BOOL  func_020755f8(UnkSndWaveArc* waveArc, u32 index, u32 fileId, void* heap);
BOOL  func_020756c0(UnkSndWaveArc* waveArc, void* bank, int index, u32 fileId, void* heap);

BOOL func_02074e24(int seqNo, void* heap) {
    return func_02074ea8(seqNo, 0xFF, heap, TRUE, NULL) == 0;
}

BOOL func_02074e50(int seqArcNo, void* heap) {
    return func_02074f2c(seqArcNo, 0xFF, heap, TRUE, NULL) == 0;
}

BOOL func_02074e7c(int bankNo, void* heap) {
    return func_02074f8c(bankNo, 0xFF, heap, TRUE, NULL) == 0;
}

int func_02074ea8(int seqNo, u32 flags, void* heap, BOOL addToArc, void** pData) {
    UnkSndSeqInfo* info = func_020747cc(seqNo);
    void* data;
    int result;

    if (info == NULL) {
        return 2;
    }

    result = func_02074f8c(info->bankNo, flags, heap, addToArc, NULL);
    if (result != 0) {
        return result;
    }

    if (flags & 1) {
        data = func_020751f8(info->fileId, heap, addToArc);
        if (data == NULL) {
            return 6;
        }
    } else {
        data = func_02074b70(info->fileId);
    }

    if (pData != NULL) {
        *pData = data;
    }
    return 0;
}

int func_02074f2c(int seqArcNo, u32 flags, void* heap, BOOL addToArc, void** pData) {
    UnkSndSeqArcInfo* info = func_02074830(seqArcNo);
    void* data;

    if (info == NULL) {
        return 3;
    }

    if (flags & 8) {
        data = func_02075264(info->fileId, heap, addToArc);
        if (data == NULL) {
            return 7;
        }
    } else {
        data = func_02074b70(info->fileId);
    }

    if (pData != NULL) {
        *pData = data;
    }
    return 0;
}

int func_02074f8c(int bankNo, u32 flags, void* heap, BOOL addToArc, void** pData) {
    UnkSndBankInfo* info = func_02074894(bankNo);
    void* bank;
    int i;

    if (info == NULL) {
        return 4;
    }

    if (flags & 2) {
        bank = func_020752d0(info->fileId, heap, addToArc);
        if (bank == NULL) {
            return 8;
        }
    } else {
        bank = func_02074b70(info->fileId);
    }

    for (i = 0; i < 4; i++) {
        UnkSndWaveArcInfo* waveArcInfo;
        void* waveArc;
        int result;

        if (info->waveArcNo[i] == 0xFFFF) {
            continue;
        }

        waveArcInfo = func_020748f8(info->waveArcNo[i]);
        if (waveArcInfo == NULL) {
            return 5;
        }

        result = func_020750cc(info->waveArcNo[i], flags, heap, addToArc, &waveArc);
        if (result != 0) {
            return result;
        }

        if ((waveArcInfo->flags & 1) && (flags & 4)) {
            if (!func_020756c0(waveArc, bank, i, waveArcInfo->fileId, heap)) {
                return 9;
            }
        }

        if (bank != NULL && waveArc != NULL) {
            func_0207b094(bank, i, waveArc);
        }
    }

    if (pData != NULL) {
        *pData = bank;
    }
    return 0;
}

int func_020750cc(int waveArcNo, u32 flags, void* heap, BOOL addToArc, void** pData) {
    UnkSndWaveArcInfo* info = func_020748f8(waveArcNo);
    void* data;

    if (info == NULL) {
        return 5;
    }

    if (flags & 4) {
        if (info->flags & 1) {
            data = func_020753a8(info->fileId, heap, addToArc);
        } else {
            data = func_0207533c(info->fileId, heap, addToArc);
        }
        if (data == NULL) {
            return 9;
        }
    } else {
        data = func_02074b70(info->fileId);
    }

    if (pData != NULL) {
        *pData = data;
    }
    return 0;
}

void* func_02075158(u32 fileId, UnkSndHeapCallback callback, void* data1, u32 data2, void* heap) {
    u32 size = func_02074ab0(fileId);
    void* mem;

    if (size == 0) {
        return NULL;
    }
    if (heap == NULL) {
        return NULL;
    }

    mem = func_02074d1c(heap, size + 0x20, callback, data1, data2);
    if (mem == NULL) {
        return NULL;
    }

    if (size != func_02074ad8(fileId, mem, size, 0)) {
        return NULL;
    }

    DC_CleanRange(mem, size);
    return mem;
}

void* func_020751f8(u32 fileId, void* heap, BOOL addToArc) {
    void* data = func_02074b70(fileId);

    if (data == NULL) {
        data = func_02075158(fileId, func_02075508, addToArc ? func_020747bc() : NULL, fileId, heap);
        if (addToArc && data != NULL) {
            func_02074b98(fileId, data);
        }
    }
    return data;
}

void* func_02075264(u32 fileId, void* heap, BOOL addToArc) {
    void* data = func_02074b70(fileId);

    if (data == NULL) {
        data = func_02075158(fileId, func_02075508, addToArc ? func_020747bc() : NULL, fileId, heap);
        if (addToArc && data != NULL) {
            func_02074b98(fileId, data);
        }
    }
    return data;
}

void* func_020752d0(u32 fileId, void* heap, BOOL addToArc) {
    void* data = func_02074b70(fileId);

    if (data == NULL) {
        data = func_02075158(fileId, func_02075530, addToArc ? func_020747bc() : NULL, fileId, heap);
        if (addToArc && data != NULL) {
            func_02074b98(fileId, data);
        }
    }
    return data;
}

void* func_0207533c(u32 fileId, void* heap, BOOL addToArc) {
    void* data = func_02074b70(fileId);

    if (data == NULL) {
        data = func_02075158(fileId, func_02075560, addToArc ? func_020747bc() : NULL, fileId, heap);
        if (addToArc && data != NULL) {
            func_02074b98(fileId, data);
        }
    }
    return data;
}

void* func_020753a8(u32 fileId, void* heap, BOOL addToArc) {
    static UnkSndWaveArc data_02113294;
    u32 size;
    UnkSndWaveArc* waveArc = func_02074b70(fileId);
    u32 tableSize;
    s32 readSize;

    if (waveArc == NULL) {

        if (func_02074ad8(fileId, &data_02113294, 0x3C, 0) != 0x3C) {
            return NULL;
        }

        tableSize = data_02113294.waveCount * 4;
        size = tableSize * 2;

        if (heap == NULL) {
            return NULL;
        }

        waveArc = func_02074d1c(heap, size + 0x5C, func_02075590, addToArc ? func_020747bc() : NULL, fileId);
        if (waveArc == NULL) {
            return NULL;
        }

        readSize = func_02074ad8(fileId, waveArc, tableSize + 0x3C, 0);
        if (readSize != tableSize + 0x3C) {
            return NULL;
        }

        MI_CpuCopyU8(waveArc->waveOffset, &waveArc->waveOffset[waveArc->waveCount], tableSize);
        MI_CpuSet(waveArc->waveOffset, 0, tableSize);
        DC_CleanRange(waveArc, size + 0x3C);

        if (addToArc) {
            func_02074b98(fileId, waveArc);
        }
    }
    return waveArc;
}

void func_020754b0(void* mem, void* arc, u32 fileId) {
    void* prev;
    int irq;

    if (arc == NULL) {
        return;
    }

    irq = OS_DisableIRQ();
    prev = func_020747a4(arc);
    if (mem == func_02074b70(fileId)) {
        func_02074b98(fileId, NULL);
    }
    func_020747a4(prev);
    OS_RestoreIRQ(irq);
}

void func_02075508(void* mem, u32 size, void* data1, u32 data2) {
    func_020754b0(mem, data1, data2);
    func_0207a5f0(mem, (u8*)mem + size);
}

void func_02075530(void* mem, u32 size, void* data1, u32 data2) {
    func_020754b0(mem, data1, data2);
    func_0207a610(mem, (u8*)mem + size);
    func_0207b160(mem);
}

void func_02075560(void* mem, u32 size, void* data1, u32 data2) {
    func_020754b0(mem, data1, data2);
    func_0207a630(mem, (u8*)mem + size);
    func_0207b1f8(mem);
}

void func_02075590(void* mem, u32 size, void* data1, u32 data2) {
    func_020754b0(mem, data1, data2);
    func_0207b1f8(mem);
}

void func_020755b0(void* mem, u32 size, void* data1, u32 data2) {
    if (mem == func_0207b44c(data1, data2)) {
        func_0207b418(data1, data2, NULL);
    }
    func_0207a630(mem, (u8*)mem + size);
}

BOOL func_020755f8(UnkSndWaveArc* waveArc, u32 index, u32 fileId, void* heap) {
    u32 size;
    u32 offset;
    u32 end;

    if (func_0207b44c(waveArc, index) != NULL) {
        return TRUE;
    }

    {
        u32 count = func_0207b410(waveArc);
        u32 i = waveArc->waveCount + index;
        offset = waveArc->waveOffset[i];
        end = index < count - 1 ? *(waveArc->waveOffset + i + 1) : waveArc->fileSize;
    }
    size = end - offset;

    if (heap == NULL) {
        return FALSE;
    }

    heap = func_02074d1c(heap, size + 0x20, func_020755b0, waveArc, index);
    if (heap == NULL) {
        return FALSE;
    }

    if (size != func_02074ad8(fileId, heap, size, offset)) {
        return FALSE;
    }

    DC_CleanRange(heap, size);
    func_0207b418(waveArc, index, heap);
    return TRUE;
}

BOOL func_020756c0(UnkSndWaveArc* waveArc, void* bank, int index, u32 fileId, void* heap) {
    UnkSndInstPos pos = func_0207b240();
    UnkSndInstData inst;

    if (bank == NULL) {
        return FALSE;
    }

    while (func_0207b260(bank, &inst, &pos)) {
        if (inst.type == 1 && index == inst.waveArcNo) {
            if (!func_020755f8(waveArc, inst.waveNo, fileId, heap)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
