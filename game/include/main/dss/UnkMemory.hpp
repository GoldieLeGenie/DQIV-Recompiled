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

extern UnkTouchPanel g_TouchPanel;
extern void* g_compressedDataPtrs[2];                  // compressed data, uncompressed data
extern UnkHeap g_appHeap;

void setCompressedData(void* src);               // set the compressed data
void uncompressData(void* dst);               // uncompress
unsigned int getUncompressedSize(void* src);       // uncompressed size
int isCompressed(void* data);               // is compressed
void* uncompressAlloc(void* data);             // uncompress (allocates)
void initHeap(UnkHeap* heap, unsigned int size0, unsigned int size1, unsigned int size2, int flag);    // init
void freeSystemHeap(UnkHeap* heap, void* p);  // free (system heap)
void* allocAppHeap(UnkHeap* heap, int size, int align);     // alloc
void freeAppHeap(UnkHeap* heap, void* p);  // free
void* dssAlloc(UnkHeap* heap, int size, int align);     // alloc
void dssFree(UnkHeap* heap, void* p);  // free
unsigned int getHeap0FreeSize(UnkHeap* heap);   // free size of the heap 0
unsigned int getSystemHeapFreeSize(UnkHeap* heap);   // free size of the system heap
unsigned int getAppHeapFreeSize(UnkHeap* heap);   // free size of the application heap
unsigned int getAppHeapTotalFreeSize(UnkHeap* heap);   // free size of the application heap
void** getHeap0(UnkHeap* heap);         // heap 0
void** getSystemHeap(UnkHeap* heap);         // system heap
void markHeapBlocks(UnkHeap* heap);           // mark the allocated blocks
void freeSystemMemory(void* p);                 // free (system heap)
int getArchiveFileCount(void* archive);            // file count of an archive
int getArchiveFileSize(void* archive, int index); // file size in an archive
void* getArchiveFile(void* archive, int index);   // file of an archive

extern "C" {
    void func_02067b88(const void* src, void* dst);         // MI_UncompressLZ8
    void func_02067c1c(const void* src, void* dst);         // MI_UncompressHuffman
    void func_02067cf4(const void* src, void* dst);         // MI_UncompressRL8
}
