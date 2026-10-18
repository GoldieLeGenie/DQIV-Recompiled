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

void setArrayErrorEnabled(int enable);
void addArrayError(int index, int size, unsigned int caller);   // add a warning
UnkArrayWarning* getArrayError(int index);
int getArrayErrorCount();                         // number of array warnings
void reportArrayError(int index, int size);     // "ARRAY ERROR %d/%d %08x !!!!"
