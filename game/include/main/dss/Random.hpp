#pragma once
#include <globaldefs.h>

class dssrand {
public:
    struct Random {
        unsigned int state;      // Offset +0 (0x0211a7c4)
        unsigned int multiplier; // Offset +4 (0x0211a7c8)
        unsigned int increment;  // Offset +8 (0x0211a7cc)
    };

    static void unkfunc_02080cec();     // seed from OS_GetLowEntropyData (MATH_InitRand32-like)
    static int rand(int n);
    static Random generator;
};

extern "C" void OS_GetLowEntropyData(unsigned int buffer[8]);
