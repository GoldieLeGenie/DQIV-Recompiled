// Sound heap: frame heap with sections of blocks that carry a free callback.
#include "snd_internal.h"

typedef void (*UnkSndHeapCallback)(void* mem, u32 size, void* data1, u32 data2);

typedef struct UnkSndHeap {
    /* 0x00 */ void* frmHeap;
    /* 0x04 */ UnkList sectionList;
} UnkSndHeap; // size 0x10

typedef struct UnkSndHeapSection {
    /* 0x00 */ UnkList blockList;
    /* 0x0C */ UnkLink link;
} UnkSndHeapSection; // size 0x14

typedef struct UnkSndHeapBlock {
    /* 0x00 */ UnkLink link;
    /* 0x08 */ u32 size;
    /* 0x0C */ UnkSndHeapCallback callback;
    /* 0x10 */ void* data1;
    /* 0x14 */ u32 data2;
    /* 0x18 */ u8 unk_18[8];
    /* 0x20 */ u8 buffer[0];
} UnkSndHeapBlock; // size 0x20

void func_02074c60(UnkSndHeap* heap);
void func_02074d84(UnkList* list);
BOOL func_02074d94(UnkSndHeap* heap, void* frmHeap);
BOOL func_02074dc8(UnkSndHeap* heap);
void func_02074e04(void);

UnkSndHeap* func_02074bd8(void* startAddress, u32 size) {
    u32 endAddress = (u32)startAddress + size;
    UnkSndHeap* heap = (UnkSndHeap*)(((u32)startAddress + 3) & ~3);
    void* frmHeap;

    if ((u32)heap > endAddress) {
        return NULL;
    }
    if (endAddress - (u32)heap < sizeof(UnkSndHeap)) {
        return NULL;
    }

    frmHeap = func_0206886c(heap + 1, endAddress - (u32)heap - sizeof(UnkSndHeap), 0);
    if (frmHeap == NULL) {
        return NULL;
    }

    if (!func_02074d94(heap, frmHeap)) {
        func_020688a4(frmHeap);
        return NULL;
    }

    return heap;
}

void func_02074c48(UnkSndHeap* heap) {
    func_02074c60(heap);
    func_020688a4(heap->frmHeap);
}

void func_02074c60(UnkSndHeap* heap) {
    UnkSndHeapSection* section;
    BOOL doCallback = FALSE;
    UnkSndHeapBlock* block;

    while ((section = func_02067fb0(&heap->sectionList, NULL)) != NULL) {
        for (block = func_02067fb0(&section->blockList, NULL); block != NULL; block = func_02067fb0(&section->blockList, block)) {
            if (block->callback != NULL) {
                block->callback(block->buffer, block->size, block->data1, block->data2);
                doCallback = TRUE;
            }
        }
        func_02067f38(&heap->sectionList, section);
    }

    func_020688e4(heap->frmHeap, 3);

    if (doCallback) {
        func_02074e04();
    }

    func_02074dc8(heap);
}

void* func_02074d1c(UnkSndHeap* heap, u32 size, UnkSndHeapCallback callback, void* data1, u32 data2) {
    UnkSndHeapSection* section;
    UnkSndHeapBlock* block = func_020688b0(heap->frmHeap, ((size + 0x1F) & ~0x1F) + sizeof(UnkSndHeapBlock), 32);

    if (block == NULL) {
        return NULL;
    }

    section = func_02067fb0(&heap->sectionList, NULL);
    block->size = size;
    block->callback = callback;
    block->data1 = data1;
    block->data2 = data2;
    func_02067e30(&section->blockList, block);
    return block->buffer;
}

void func_02074d84(UnkList* list) {
    func_02067dec(list, 0);
}

BOOL func_02074d94(UnkSndHeap* heap, void* frmHeap) {
    func_02067dec(&heap->sectionList, 0xC);
    heap->frmHeap = frmHeap;

    if (!func_02074dc8(heap)) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_02074dc8(UnkSndHeap* heap) {
    UnkSndHeapSection* section = func_020688b0(heap->frmHeap, sizeof(UnkSndHeapSection), 4);

    if (section == NULL) {
        return FALSE;
    }

    func_02074d84(&section->blockList);
    func_02067e30(&heap->sectionList, section);
    return TRUE;
}

void func_02074e04(void) {
    u32 tag = func_0207ac10();
    func_0207a9e8(1);
    func_0207aba4(tag);
}
