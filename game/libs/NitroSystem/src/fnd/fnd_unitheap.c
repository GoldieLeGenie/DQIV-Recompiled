#include "fnd_types.h"

/* Unit heap: fixed-size blocks kept in a singly linked free list. */

typedef struct UnkUnitMBlock {
    struct UnkUnitMBlock* unk_00;   // 0x00 next free block
} UnkUnitMBlock;

typedef struct UnkUnitMBlockList {
    UnkUnitMBlock* unk_00;          // 0x00 first free block
} UnkUnitMBlockList;

typedef struct UnkUnitHeap {
    UnkFndHeapHead unk_00;          // 0x00
    UnkUnitMBlockList unk_24;       // 0x24 free list
    u32 unk_28;                     // 0x28 block size
} UnkUnitHeap;

static inline void FillAllocMemory(UnkFndHeapHead* heap, void* address, u32 size) {
    if ((u16)(heap->unk_20 & 0xff) & 1) {
        func_0206785c(0, address, size);
    }
}

/* Pops the first free block. */
UnkUnitMBlock* func_02068910(UnkUnitMBlockList* list) {
    UnkUnitMBlock* block = list->unk_00;

    if (block != NULL) {
        list->unk_00 = block->unk_00;
    }
    return block;
}

void* func_02068928(UnkUnitHeap* heap) {
    UnkUnitMBlock* block = func_02068910(&heap->unk_24);

    if (block != NULL) {
        FillAllocMemory(&heap->unk_00, block, heap->unk_28);
    }
    return block;
}

void func_02068968(UnkUnitHeap* heap, void* memBlock) {
    UnkUnitMBlock* block = (UnkUnitMBlock*)memBlock;

    block->unk_00 = heap->unk_24.unk_00;
    heap->unk_24.unk_00 = block;
}
