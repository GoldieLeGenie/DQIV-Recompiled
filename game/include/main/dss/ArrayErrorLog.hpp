#pragma once
#include <globaldefs.h>

// dss array index warnings
struct ArrayErrorEntry {
    int index_;                                 // 0x00
    int size_;                                  // 0x04
    unsigned int caller_;                       // 0x08 return address
};

extern int g_arrayErrorEnabled;                       // array warnings enabled
extern int g_arrayWarningCount;                       // number of array warnings
extern ArrayErrorEntry g_arrayWarnings[16];

void setArrayErrorEnabled(int enable);
void addArrayError(int index, int size, unsigned int caller);   // add a warning
ArrayErrorEntry* getArrayError(int index);
int getArrayErrorCount();                         // number of array warnings
void reportArrayError(int index, int size);     // "ARRAY ERROR %d/%d %08x !!!!"
