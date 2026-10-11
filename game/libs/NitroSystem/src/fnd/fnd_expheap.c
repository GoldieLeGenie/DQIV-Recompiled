#include "fnd_types.h"

/* Expanded heap: best/first-fit allocator with 0x10-byte block headers. */

typedef struct UnkExpMBlock {
    u16 unk_00;                  // 0x00 signature (free / used)
    u16 unk_02;                  // 0x02 attributes: 0-7 group, 8-14 alignment padding, 15 direction
    u32 unk_04;                  // 0x04 payload size
    struct UnkExpMBlock* unk_08; // 0x08 previous block
    struct UnkExpMBlock* unk_0c; // 0x0C next block
} UnkExpMBlock;

typedef struct UnkExpMBlockList {
    UnkExpMBlock* unk_00;       // 0x00 first block
    UnkExpMBlock* unk_04;       // 0x04 last block
} UnkExpMBlockList;

typedef struct UnkExpHeapHead {
    UnkExpMBlockList unk_00;    // 0x00 free blocks
    UnkExpMBlockList unk_08;    // 0x08 used blocks
    u16 unk_10;                 // 0x10 group id
    u16 unk_12;                 // 0x12 features (bit 0: allocation mode)
} UnkExpHeapHead;

typedef struct UnkExpHeap {
    UnkFndHeapHead unk_00;      // 0x00
    UnkExpHeapHead unk_24;      // 0x24
} UnkExpHeap;                   // 0x38

typedef struct UnkMemRegion {
    void* unk_00;               // 0x00 start
    void* unk_04;               // 0x04 end
} UnkMemRegion;

#define SIG_EXPH 0x45585048
#define SIG_FR   0x4652
#define SIG_UD   0x5544

#define GET_BITS(data, st, bits) (((data) >> (st)) & ((1 << (bits)) - 1))
#define SET_BITS(data, st, bits, val)                    \
    do {                                                 \
        (data) &= ~(((1 << (bits)) - 1) << (st));        \
        (data) |= ((val) & ((1 << (bits)) - 1)) << (st); \
    } while (0)

#define HEAD_FROM_EXP(exp) ((UnkFndHeapHead*)((u32)(exp) - sizeof(UnkFndHeapHead)))
#define ROUND_UP(v, a)     ((((a) - 1) + (u32)(v)) & ~((a) - 1))
#define ROUND_DOWN(v, a)   ((u32)(v) & ~((a) - 1))

static inline u16 GetAlignmentForMBlock(UnkExpMBlock* block) {
    return (u16)GET_BITS(block->unk_02, 8, 7);
}

static inline u16 GetAllocMode(UnkExpHeapHead* exp) {
    return (u16)GET_BITS(exp->unk_12, 0, 1);
}

static inline void* GetMemPtrForMBlock(UnkExpMBlock* block) {
    return (void*)((u32)block + sizeof(UnkExpMBlock));
}

static inline void* GetMBlockEndAddr(UnkExpMBlock* block) {
    return (void*)((u32)GetMemPtrForMBlock(block) + block->unk_04);
}

static inline void SetAllocDirForMBlock(UnkExpMBlock* block, u16 dir) {
    SET_BITS(block->unk_02, 15, 1, dir);
}

static inline void SetAlignmentForMBlock(UnkExpMBlock* block, u16 alignment) {
    SET_BITS(block->unk_02, 8, 7, alignment);
}

static inline void SetGroupIDForMBlock(UnkExpMBlock* block, u8 id) {
    SET_BITS(block->unk_02, 0, 8, id);
}

static inline void FillAllocMemory(UnkFndHeapHead* heap, void* address, u32 size) {
    if ((u16)GET_BITS(heap->unk_20, 0, 8) & 1) {
        func_0206785c(0, address, size);
    }
}

void func_020680e8(UnkMemRegion* region, UnkExpMBlock* block) {
    region->unk_00 = (void*)((u32)block - GetAlignmentForMBlock(block));
    region->unk_04 = GetMBlockEndAddr(block);
}

/* Unlinks block, returns the previous block. */
UnkExpMBlock* func_02068114(UnkExpMBlockList* list, UnkExpMBlock* block) {
    UnkExpMBlock* prev = block->unk_08;
    UnkExpMBlock* next = block->unk_0c;

    if (prev != NULL) {
        prev->unk_0c = next;
    } else {
        list->unk_00 = next;
    }
    if (next != NULL) {
        next->unk_08 = prev;
    } else {
        list->unk_04 = prev;
    }
    return prev;
}

/* Links block after prev (at the head when prev is NULL). */
UnkExpMBlock* func_0206813c(UnkExpMBlockList* list, UnkExpMBlock* block, UnkExpMBlock* prev) {
    UnkExpMBlock* next;

    block->unk_08 = prev;
    if (prev != NULL) {
        next = prev->unk_0c;
        prev->unk_0c = block;
    } else {
        next = list->unk_00;
        list->unk_00 = block;
    }
    block->unk_0c = next;
    if (next != NULL) {
        next->unk_08 = block;
    } else {
        list->unk_04 = block;
    }
    return block;
}

/* Turns a memory region into a single block with the given signature. */
UnkExpMBlock* func_0206816c(UnkMemRegion* region, u16 signature) {
    UnkExpMBlock* block = (UnkExpMBlock*)region->unk_00;

    block->unk_00 = signature;
    block->unk_02 = 0;
    block->unk_04 = (u32)region->unk_04 - (u32)GetMemPtrForMBlock(block);
    block->unk_08 = NULL;
    block->unk_0c = NULL;
    return block;
}

UnkExpHeap* func_02068198(void* start, void* end, u16 option) {
    UnkExpHeap* heap = (UnkExpHeap*)start;
    UnkExpHeapHead* exp = &heap->unk_24;

    func_02068054(&heap->unk_00, SIG_EXPH, (void*)((u32)heap + sizeof(UnkExpHeap)), end, option);
    exp->unk_10 = 0;
    exp->unk_12 = 0;
    SET_BITS(exp->unk_12, 0, 1, 0);
    {
        UnkMemRegion region;
        UnkExpMBlock* block;

        region.unk_00 = heap->unk_00.unk_18;
        region.unk_04 = heap->unk_00.unk_1c;
        block = func_0206816c(&region, SIG_FR);
        exp->unk_00.unk_00 = block;
        exp->unk_00.unk_04 = block;
        exp->unk_08.unk_00 = NULL;
        exp->unk_08.unk_04 = NULL;
    }
    return heap;
}

/* Carves a used block of size bytes at mem out of a free block. */
void* func_0206820c(UnkExpHeapHead* exp, UnkExpMBlock* block, void* mem, u32 size, u16 direction) {
    UnkMemRegion freeRgnT;
    UnkMemRegion freeRgnB;
    UnkExpMBlock* prev;

    func_020680e8(&freeRgnT, block);
    freeRgnB.unk_04 = freeRgnT.unk_04;
    freeRgnB.unk_00 = (void*)(size + (u32)mem);
    freeRgnT.unk_04 = (void*)((u32)mem - sizeof(UnkExpMBlock));

    prev = func_02068114(&exp->unk_00, block);

    if ((u32)freeRgnT.unk_04 - (u32)freeRgnT.unk_00 < sizeof(UnkExpMBlock) + 4) {
        freeRgnT.unk_04 = freeRgnT.unk_00;
    } else {
        prev = func_0206813c(&exp->unk_00, func_0206816c(&freeRgnT, SIG_FR), prev);
    }

    if ((u32)freeRgnB.unk_04 - (u32)freeRgnB.unk_00 < sizeof(UnkExpMBlock) + 4) {
        freeRgnB.unk_00 = freeRgnB.unk_04;
    } else {
        func_0206813c(&exp->unk_00, func_0206816c(&freeRgnB, SIG_FR), prev);
    }

    FillAllocMemory(HEAD_FROM_EXP(exp), freeRgnT.unk_04, (u32)freeRgnB.unk_00 - (u32)freeRgnT.unk_04);

    {
        UnkMemRegion region;
        UnkExpMBlock* newBlock;

        region.unk_00 = (void*)((u32)mem - sizeof(UnkExpMBlock));
        region.unk_04 = freeRgnB.unk_00;
        newBlock = func_0206816c(&region, SIG_UD);
        SetAllocDirForMBlock(newBlock, direction);
        SetAlignmentForMBlock(newBlock, (u16)((u32)newBlock - (u32)freeRgnT.unk_04));
        SetGroupIDForMBlock(newBlock, exp->unk_10);
        func_0206813c(&exp->unk_08, newBlock, exp->unk_08.unk_04);
    }
    return mem;
}

/* Allocates from the start of the heap. */
void* func_0206838c(UnkExpHeap* heap, u32 size, u32 alignment) {
    UnkExpHeapHead* exp = &heap->unk_24;
    BOOL first = GetAllocMode(exp) == 0;
    UnkExpMBlock* found = NULL;
    UnkExpMBlock* block;
    u32 foundSize = 0xffffffff;
    void* foundMem = NULL;

    for (block = exp->unk_00.unk_00; block != NULL; block = block->unk_0c) {
        void* mem = GetMemPtrForMBlock(block);
        void* reqMem = (void*)ROUND_UP(mem, alignment);
        u32 offset = (u32)reqMem - (u32)mem;

        if (block->unk_04 >= size + offset && foundSize > block->unk_04) {
            found = block;
            foundSize = block->unk_04;
            foundMem = reqMem;
            if (first || foundSize == size) {
                break;
            }
        }
    }

    if (found == NULL) {
        return NULL;
    }
    return func_0206820c(exp, found, foundMem, size, 0);
}

/* Allocates from the end of the heap. */
void* func_02068440(UnkExpHeap* heap, u32 size, u32 alignment) {
    UnkExpHeapHead* exp = &heap->unk_24;
    BOOL first = GetAllocMode(exp) == 0;
    UnkExpMBlock* found = NULL;
    UnkExpMBlock* block;
    u32 foundSize = 0xffffffff;
    void* foundMem = NULL;

    for (block = exp->unk_00.unk_04; block != NULL; block = block->unk_08) {
        void* mem = GetMemPtrForMBlock(block);
        void* memEnd = (void*)(block->unk_04 + (u32)mem);
        void* reqMem = (void*)ROUND_DOWN((u32)memEnd - size, alignment);
        s32 offset = (s32)((u32)reqMem - (u32)mem);

        if (offset >= 0 && foundSize > block->unk_04) {
            found = block;
            foundSize = block->unk_04;
            foundMem = reqMem;
            if (first || foundSize == size) {
                break;
            }
        }
    }

    if (found == NULL) {
        return NULL;
    }
    return func_0206820c(exp, found, foundMem, size, 1);
}

/* Returns a freed region to the free list, merging with neighbouring free blocks. */
BOOL func_020684f0(UnkExpHeapHead* exp, UnkMemRegion* region) {
    UnkExpMBlock* blockPrev = NULL;
    UnkMemRegion freeRgn = *region;
    UnkExpMBlock* block;

    for (block = exp->unk_00.unk_00; block != NULL; block = block->unk_0c) {
        if (block < (UnkExpMBlock*)region->unk_00) {
            blockPrev = block;
            continue;
        }
        if (block == (UnkExpMBlock*)region->unk_04) {
            freeRgn.unk_04 = GetMBlockEndAddr(block);
            func_02068114(&exp->unk_00, block);
        }
        break;
    }

    if (blockPrev != NULL && GetMBlockEndAddr(blockPrev) == region->unk_00) {
        freeRgn.unk_00 = blockPrev;
        blockPrev = func_02068114(&exp->unk_00, blockPrev);
    }

    if ((u32)freeRgn.unk_04 - (u32)freeRgn.unk_00 < sizeof(UnkExpMBlock)) {
        return FALSE;
    }
    func_0206813c(&exp->unk_00, func_0206816c(&freeRgn, SIG_FR), blockPrev);
    return TRUE;
}

UnkExpHeap* func_020685e0(void* start, u32 size, u16 option) {
    void* end = (void*)ROUND_DOWN(size + (u32)start, 4);

    start = (void*)ROUND_UP(start, 4);
    if ((u32)start > (u32)end || (u32)end - (u32)start < sizeof(UnkExpHeap) + sizeof(UnkExpMBlock) + 4) {
        return NULL;
    }
    return func_02068198(start, end, option);
}

void* func_02068618(UnkExpHeap* heap, u32 size, int alignment) {
    if (size == 0) {
        size = 1;
    }
    size = ROUND_UP(size, 4);
    if (alignment >= 0) {
        return func_0206838c(heap, size, alignment);
    } else {
        return func_02068440(heap, size, -alignment);
    }
}

void func_02068648(UnkExpHeap* heap, void* memBlock) {
    UnkMemRegion region;
    UnkExpMBlock* block = (UnkExpMBlock*)((u32)memBlock - sizeof(UnkExpMBlock));

    func_020680e8(&region, block);
    func_02068114(&heap->unk_24.unk_08, block);
    func_020684f0(&heap->unk_24, &region);
}

u32 func_02068684(UnkExpHeap* heap) {
    u32 sum = 0;
    UnkExpMBlock* block;

    for (block = heap->unk_24.unk_00.unk_00; block != NULL; block = block->unk_0c) {
        sum += block->unk_04;
    }
    return sum;
}

extern int abs(int);

u32 func_020686ac(UnkExpHeap* heap, int alignment) {
    u32 maxSize;
    u32 offsetMin;
    UnkExpMBlock* block;

    alignment = abs(alignment);
    maxSize = 0;
    offsetMin = 0xffffffff;
    for (block = heap->unk_24.unk_00.unk_00; block != NULL; block = block->unk_0c) {
        void* mem = GetMemPtrForMBlock(block);
        void* baseAddress = (void*)ROUND_UP(mem, alignment);
        void* memEnd = (void*)(block->unk_04 + (u32)mem);

        if ((u32)baseAddress < (u32)memEnd) {
            u32 blockSize = (u32)memEnd - (u32)baseAddress;
            u32 offset = (u32)baseAddress - (u32)mem;

            if (maxSize < blockSize || (maxSize == blockSize && offsetMin > offset)) {
                maxSize = blockSize;
                offsetMin = offset;
            }
        }
    }
    return maxSize;
}
