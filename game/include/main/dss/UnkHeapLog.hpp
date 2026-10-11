#pragma once
#include <globaldefs.h>

// heap block of the allocation log
struct UnkHeapLogEntry {
    int state_;                                 // 0x00 0: free, 1: allocated, 2: allocated before the mark
    void* addr_;                                // 0x04
    int size_;                                  // 0x08
    void* caller_;                              // 0x0C return address of the allocation
};

// allocation log of the dss heaps
struct UnkHeapLog {
    UnkHeapLogEntry entries_[100];              // 0x000
    int enabled_;                               // 0x640
    int unk_644;                                // 0x644

    void unkfunc_02089214();                    // init
    void unkfunc_02089250(const char* name, void* addr, int size, void* caller, void** heap);   // add a block
    void unkfunc_020892cc(const char* name, void* addr, void* caller, void** heap);             // remove a block
    char* unkfunc_0208934c();                   // text of the allocated blocks
    void unkfunc_020893e0();                    // mark the allocated blocks
};

extern char data_02120548[0x400];
extern UnkHeapLog data_02120948;
