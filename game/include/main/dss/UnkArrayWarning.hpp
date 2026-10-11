#pragma once
#include <globaldefs.h>

// dss array index warnings
struct UnkArrayWarning {
    int index_;                                 // 0x00
    int size_;                                  // 0x04
    unsigned int caller_;                       // 0x08 return address
};

extern int data_020c499c;                       // array warnings enabled
extern int data_02120fb4;                       // number of array warnings
extern UnkArrayWarning data_02120fb8[16];

void unkfunc_02089580(int enable);
void unkfunc_02089590(int index, int size, unsigned int caller);   // add a warning
UnkArrayWarning* unkfunc_020895e8(int index);
int unkfunc_020895fc();                         // number of array warnings
void unkfunc_0208960c(int index, int size);     // "ARRAY ERROR %d/%d %08x !!!!"
