#pragma once
#include <globaldefs.h>
#include "main/dss/DssCore.hpp"
#include "nnsys/fnd.hpp"

// touch panel state
struct UnkTouchPanel {
    char unk_00[0x18];                          // 0x00
    int touch_;                                 // 0x18
    int x_;                                     // 0x1C
    int y_;                                     // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C

    UnkTouchPanel();
};

// main heap: system, sub and application expanded heaps
struct UnkHeap {
    void* heap0_;                               // 0x00
    void* heap1_;                               // 0x04 system heap
    void* heap2_;                               // 0x08 application heap
    NNSFndAllocator allocator0_;                // 0x0C
    NNSFndAllocator allocator1_;                // 0x1C
    NNSFndAllocator allocator2_;                // 0x2C
    unsigned int size0_;                        // 0x3C
    unsigned int size1_;                        // 0x40
    unsigned int size2_;                        // 0x44
    unsigned int free0_;                        // 0x48
    unsigned int free1_;                        // 0x4C
    unsigned int free2_;                        // 0x50
    unsigned int unk_54;                        // 0x54 program size
};

extern UnkTouchPanel data_0211a5d4;
extern void* data_0211a604[2];                  // compressed data, uncompressed data
extern UnkHeap data_0211a60c;

void unkfunc_0207f4c4(void* src);               // set the compressed data
void unkfunc_0207f4d4(void* dst);               // uncompress
unsigned int unkfunc_0207f52c(void* src);       // uncompressed size
int unkfunc_0207f548(void* data);               // is compressed
void* unkfunc_0207f590(void* data);             // uncompress (allocates)
void unkfunc_0207f5c8(UnkHeap* heap, unsigned int size0, unsigned int size1, unsigned int size2, int flag);    // init
void unkfunc_0207f728(UnkHeap* heap, void* p);  // free (system heap)
void* unkfunc_0207f77c(UnkHeap* heap, int size, int align);     // alloc
void unkfunc_0207f7e0(UnkHeap* heap, void* p);  // free
void* unkfunc_0207f834(UnkHeap* heap, int size, int align);     // alloc
void unkfunc_0207f840(UnkHeap* heap, void* p);  // free
unsigned int unkfunc_0207f84c(UnkHeap* heap);   // free size of the heap 0
unsigned int unkfunc_0207f85c(UnkHeap* heap);   // free size of the system heap
unsigned int unkfunc_0207f86c(UnkHeap* heap);   // free size of the application heap
unsigned int unkfunc_0207f87c(UnkHeap* heap);   // free size of the application heap
void** unkfunc_0207f88c(UnkHeap* heap);         // heap 0
void** unkfunc_0207f890(UnkHeap* heap);         // system heap
void unkfunc_0207f898(UnkHeap* heap);           // mark the allocated blocks
void unkfunc_0207f8ac(void* p);                 // free (system heap)
int unkfunc_0207f8c4(void* archive);            // file count of an archive
int unkfunc_0207f8cc(void* archive, int index); // file size in an archive
void* unkfunc_0207f8dc(void* archive, int index);   // file of an archive

extern "C" {
    void func_02067b88(const void* src, void* dst);         // MI_UncompressLZ8
    void func_02067c1c(const void* src, void* dst);         // MI_UncompressHuffman
    void func_02067cf4(const void* src, void* dst);         // MI_UncompressRL8
}
