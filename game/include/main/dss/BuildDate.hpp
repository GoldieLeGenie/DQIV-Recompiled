#pragma once
#include <globaldefs.h>

struct BuildDate {
    const char* date_;                          // 0x00
    char time_[0x10];                           // 0x04
    char dateString_[0x10];                     // 0x14

    BuildDate();
    void unkfunc_02057000();
};

// contents of "data/date.dtd"
struct UnkConvertDate {
    char unk_00[2];                             // 0x00
    char date_[8];                              // 0x02
    char end0_;                                 // 0x0A
    char end1_;                                 // 0x0B
    char time_[5];                              // 0x0C
    char end2_;                                 // 0x11
    char unk_12[6];                             // 0x12
};

extern UnkConvertDate data_0210baf8;
