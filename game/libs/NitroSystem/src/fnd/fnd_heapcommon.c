#include "fnd_types.h"

BOOL       data_0210ced4; // root heap list initialized
UnkFndList data_0210ced8; // root heap list

/* Recursively finds the heap (in list or its children) whose memory range contains memBlock. */
UnkFndHeapHead* func_02067fc4(UnkFndList* list, const void* memBlock) {
    UnkFndHeapHead* heap = NULL;

    while ((heap = func_02067f98(list, heap)) != NULL) {
        if (heap->unk_18 <= memBlock && memBlock < heap->unk_1c) {
            UnkFndHeapHead* child = func_02067fc4(&heap->unk_0c, memBlock);

            if (child != NULL) {
                return child;
            }
            return heap;
        }
    }
    return NULL;
}

/* Returns the list a heap at this address must be registered in. */
UnkFndList* func_0206802c(void* heap) {
    UnkFndList* list = &data_0210ced8;
    UnkFndHeapHead* containHeap = func_02067fc4(list, heap);

    if (containHeap != NULL) {
        list = &containHeap->unk_0c;
    }
    return list;
}

void func_02068054(UnkFndHeapHead* heap, u32 signature, void* start, void* end, u16 option) {
    heap->unk_00 = signature;
    heap->unk_18 = start;
    heap->unk_1c = end;
    heap->unk_20 = 0;
    heap->unk_20 = (heap->unk_20 & ~0xff) | (option & 0xff);
    func_02067dec(&heap->unk_0c, 4);
    if (!data_0210ced4) {
        func_02067dec(&data_0210ced8, 4);
        data_0210ced4 = TRUE;
    }
    func_02067e30(func_0206802c(heap), heap);
}

void func_020680d0(UnkFndHeapHead* heap) {
    func_02067f38(func_0206802c(heap), heap);
}
