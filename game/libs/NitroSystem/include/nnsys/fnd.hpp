#pragma once

struct NNSFndAllocator {
    void* pFunc;                                // 0x00
    void* pHeap;                                // 0x04
    unsigned int heapParam1;                    // 0x08
    unsigned int heapParam2;                    // 0x0C
};

extern "C" {
    void  func_02068a44(NNSFndAllocator* allocator, int heap, int alignment);       // NNS_FndInitAllocatorForExpHeap
    void* func_02068a1c(NNSFndAllocator* allocator, unsigned int size);             // NNS_FndAllocFromAllocator
    void  func_02068a30(NNSFndAllocator* allocator, void* memBlock);                // NNS_FndFreeToAllocator
    void* func_020685e0(void* memory, unsigned int size, int option);               // NNS_FndCreateExpHeapEx
    void* func_02068618(void* heap, int size, int align);                           // NNS_FndAllocFromExpHeapEx
    void  func_02068648(void* heap, void* memBlock);                                // NNS_FndFreeToExpHeap
    unsigned int func_02068684(void* heap);                                         // NNS_FndGetTotalFreeSizeForExpHeap
    unsigned int func_020686ac(void* heap, int alignment);                          // NNS_FndGetAllocatableSizeForExpHeapEx
}
