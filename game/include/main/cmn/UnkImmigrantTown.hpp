#pragma once
#include "globaldefs.h"

// Residents of the immigrant town: envoy type and map position of each one
struct UnkImmigrantTown {
    int count_;                                 // 0x00
    unsigned char type_[42];                    // 0x04
    unsigned char x_[42];                       // 0x2E (0xff: not placed)
    unsigned char y_[42];                       // 0x58
    int queueCount_;                            // 0x84
    int queueFlag_;                             // 0x88
    unsigned char queueX_[20];                  // 0x8C positions waiting for a resident
    unsigned char queueY_[20];                  // 0xA0

    static UnkImmigrantTown* getSingleton();
    void unkfunc_02037ca4();                    // rebuild the resident list from the envoys
    void unkfunc_02037d28();                    // clear the positions
    void unkfunc_02037d50(int type);
    int unkfunc_02037d6c(int type);
    void unkfunc_02037db0(int x, int y);
    void unkfunc_02037e20(int x, int y, int count, int* types);
    int unkfunc_02037ef4(int x, int y);
    int unkfunc_02037f40(int x, int y);
    int unkfunc_02037f84(int type);
    void unkfunc_02037f98();
    int unkfunc_02038034(int index, int type);
    int unkfunc_02038140();                     // town level from the area flags
};
