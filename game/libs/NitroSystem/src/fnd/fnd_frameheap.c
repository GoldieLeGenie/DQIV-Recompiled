#include "fnd_types.h"

/* Frame heap: stack-like allocator growing from both ends. */

typedef struct UnkFrmHeapState {
    u32 unk_00;                     // 0x00 tag
    void* unk_04;                   // 0x04 saved head
    void* unk_08;                   // 0x08 saved tail
    struct UnkFrmHeapState* unk_0c; // 0x0C previous state
} UnkFrmHeapState;

typedef struct UnkFrmHeapHead {
    void* unk_00;                   // 0x00 head allocation pointer
    void* unk_04;                   // 0x04 tail allocation pointer
    UnkFrmHeapState* unk_08;        // 0x08 saved states
} UnkFrmHeapHead;

typedef struct UnkFrmHeap {
    UnkFndHeapHead unk_00;          // 0x00
    UnkFrmHeapHead unk_24;          // 0x24
} UnkFrmHeap;                       // 0x30

#define SIG_FRMH 0x46524d48

#define HEAD_FROM_FRM(frm) ((UnkFndHeapHead*)((u32)(frm) - sizeof(UnkFndHeapHead)))
#define ROUND_UP(v, a)     ((((a) - 1) + (u32)(v)) & ~((a) - 1))
#define ROUND_DOWN(v, a)   ((u32)(v) & ~((a) - 1))

static inline void FillAllocMemory(UnkFndHeapHead* heap, void* address, u32 size) {
    if ((u16)(heap->unk_20 & 0xff) & 1) {
        func_0206785c(0, address, size);
    }
}

UnkFrmHeap* func_02068730(void* start, void* end, u16 option) {
    UnkFrmHeap* heap = (UnkFrmHeap*)start;

    func_02068054(&heap->unk_00, SIG_FRMH, (void*)((u32)heap + sizeof(UnkFrmHeap)), end, option);
    heap->unk_24.unk_00 = heap->unk_00.unk_18;
    heap->unk_24.unk_04 = heap->unk_00.unk_1c;
    heap->unk_24.unk_08 = NULL;
    return heap;
}

/* Allocates from the head pointer. */
void* func_02068778(UnkFrmHeapHead* frm, u32 size, u32 alignment) {
    void* newBlock = (void*)ROUND_UP(frm->unk_00, alignment);
    void* endAddress = (void*)(size + (u32)newBlock);

    if ((u32)endAddress > (u32)frm->unk_04) {
        return NULL;
    }
    FillAllocMemory(HEAD_FROM_FRM(frm), frm->unk_00, (u32)endAddress - (u32)frm->unk_00);
    frm->unk_00 = endAddress;
    return newBlock;
}

/* Allocates from the tail pointer. */
void* func_020687d4(UnkFrmHeapHead* frm, u32 size, u32 alignment) {
    void* newBlock = (void*)ROUND_DOWN((u32)frm->unk_04 - size, alignment);

    if ((u32)newBlock < (u32)frm->unk_00) {
        return NULL;
    }
    FillAllocMemory(HEAD_FROM_FRM(frm), newBlock, (u32)frm->unk_04 - (u32)newBlock);
    frm->unk_04 = newBlock;
    return newBlock;
}

/* Frees everything allocated from the head. */
void func_0206882c(UnkFrmHeap* heap) {
    heap->unk_24.unk_00 = heap->unk_00.unk_18;
    heap->unk_24.unk_08 = NULL;
}

/* Frees everything allocated from the tail. */
void func_02068840(UnkFrmHeap* heap) {
    UnkFrmHeapState* state;

    for (state = heap->unk_24.unk_08; state != NULL; state = state->unk_0c) {
        state->unk_08 = heap->unk_00.unk_1c;
    }
    heap->unk_24.unk_04 = heap->unk_00.unk_1c;
}

UnkFrmHeap* func_0206886c(void* start, u32 size, u16 option) {
    void* end = (void*)ROUND_DOWN(size + (u32)start, 4);

    start = (void*)ROUND_UP(start, 4);
    if ((u32)start > (u32)end || (u32)end - (u32)start < sizeof(UnkFrmHeap)) {
        return NULL;
    }
    return func_02068730(start, end, option);
}

void func_020688a4(UnkFrmHeap* heap) {
    func_020680d0(&heap->unk_00);
}

void* func_020688b0(UnkFrmHeap* heap, u32 size, int alignment) {
    if (size == 0) {
        size = 1;
    }
    size = ROUND_UP(size, 4);
    if (alignment >= 0) {
        return func_02068778(&heap->unk_24, size, alignment);
    } else {
        return func_020687d4(&heap->unk_24, size, -alignment);
    }
}

void func_020688e4(UnkFrmHeap* heap, int mode) {
    if (mode & 1) {
        func_0206882c(heap);
    }
    if (mode & 2) {
        func_02068840(heap);
    }
}
