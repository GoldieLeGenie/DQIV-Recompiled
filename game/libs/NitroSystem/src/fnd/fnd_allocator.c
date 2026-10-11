#include "fnd_types.h"

/* Generic allocator interface over the expanded / frame / unit / OS heaps. */

struct UnkFndAllocator;

typedef void* (*UnkFndAllocFunc)(struct UnkFndAllocator* allocator, u32 size);
typedef void (*UnkFndFreeFunc)(struct UnkFndAllocator* allocator, void* memBlock);

typedef struct UnkFndAllocatorFunc {
    UnkFndAllocFunc unk_00;         // 0x00 alloc
    UnkFndFreeFunc unk_04;          // 0x04 free
} UnkFndAllocatorFunc;

typedef struct UnkFndAllocator {
    const UnkFndAllocatorFunc* unk_00; // 0x00 function table
    void* unk_04;                   // 0x04 heap
    u32 unk_08;                     // 0x08 heap parameter 1 (alignment / OS arena id)
    u32 unk_0c;                     // 0x0C heap parameter 2
} UnkFndAllocator;

extern void* func_02068618(void* heap, u32 size, int alignment);
extern void func_02068648(void* heap, void* memBlock);
extern void* func_020688b0(void* heap, u32 size, int alignment);
extern void* func_02068928(void* heap);
extern void func_02068968(void* heap, void* memBlock);
extern void* OS_AllocFromHeap(int arena, int heap, u32 size);
extern void OS_FreeFromHeap(int arena, int heap, void* ptr);

typedef struct UnkUnitHeapView {
    UnkFndHeapHead unk_00;          // 0x00
    void* unk_24;                   // 0x24 free list
    u32 unk_28;                     // 0x28 block size
} UnkUnitHeapView;

void* func_02068978(UnkFndAllocator* allocator, u32 size) {
    return func_02068618(allocator->unk_04, size, (int)allocator->unk_08);
}

void func_0206898c(UnkFndAllocator* allocator, void* memBlock) {
    func_02068648(allocator->unk_04, memBlock);
}

void* func_0206899c(UnkFndAllocator* allocator, u32 size) {
    return func_020688b0(allocator->unk_04, size, (int)allocator->unk_08);
}

void func_020689b0(UnkFndAllocator* allocator, void* memBlock) {
}

void* func_020689b4(UnkFndAllocator* allocator, u32 size) {
    UnkUnitHeapView* heap = (UnkUnitHeapView*)allocator->unk_04;

    if (size > heap->unk_28) {
        return NULL;
    }
    return func_02068928(heap);
}

void func_020689d4(UnkFndAllocator* allocator, void* memBlock) {
    func_02068968(allocator->unk_04, memBlock);
}

#ifndef DQIV_NATIVE
void* func_020689e4(UnkFndAllocator* allocator, u32 size) {
    return OS_AllocFromHeap((int)allocator->unk_08, (int)allocator->unk_04, size);
}

void func_02068a00(UnkFndAllocator* allocator, void* memBlock) {
    OS_FreeFromHeap((int)allocator->unk_08, (int)allocator->unk_04, memBlock);
}
#endif

/* allocator function tables: expanded heap, frame heap, unit heap, OS heap */
static const UnkFndAllocatorFunc data_020ba510 = {func_02068978, func_0206898c};
static const UnkFndAllocatorFunc data_020ba508 = {func_0206899c, func_020689b0};
static const UnkFndAllocatorFunc data_020ba500 = {func_020689b4, func_020689d4};
#ifndef DQIV_NATIVE
static const UnkFndAllocatorFunc data_020ba518 = {func_020689e4, func_02068a00};
#endif

void* func_02068a1c(UnkFndAllocator* allocator, u32 size) {
    return allocator->unk_00->unk_00(allocator, size);
}

void func_02068a30(UnkFndAllocator* allocator, void* memBlock) {
    allocator->unk_00->unk_04(allocator, memBlock);
}

void func_02068a44(UnkFndAllocator* allocator, void* heap, int alignment) {
#ifdef DQIV_NATIVE
    extern int pcTryInitAllocator(void*, void*, int);
    if (pcTryInitAllocator(allocator, heap, alignment)) return;
#endif
    allocator->unk_00 = &data_020ba510;
    allocator->unk_04 = heap;
    allocator->unk_08 = (u32)alignment;
    allocator->unk_0c = 0;
}

/* The frame heap, unit heap and OS heap initializers are not linked in the ROM (no caller);
   their function tables stay in this unit's .rodata. */
void unkfunc_unused_18(UnkFndAllocator* allocator, void* heap, int alignment) {
    allocator->unk_00 = &data_020ba508;
    allocator->unk_04 = heap;
    allocator->unk_08 = (u32)alignment;
    allocator->unk_0c = 0;
}

void unkfunc_unused_17(UnkFndAllocator* allocator, void* heap) {
    allocator->unk_00 = &data_020ba500;
    allocator->unk_04 = heap;
    allocator->unk_08 = 0;
    allocator->unk_0c = 0;
}

#ifndef DQIV_NATIVE
void unkfunc_unused_19(UnkFndAllocator* allocator, int arena, int heap) {
    allocator->unk_00 = &data_020ba518;
    allocator->unk_04 = (void*)heap;
    allocator->unk_08 = (u32)arena;
    allocator->unk_0c = 0;
}
#endif
