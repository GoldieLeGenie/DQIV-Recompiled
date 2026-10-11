#pragma once
#include <globaldefs.h>
#include "main/dss/UnkBgBuffer.hpp"

// Sub screen window BG: characters + screen (data_020fb8f4)
struct UnkMenuWindowBg {
    UnkCharBuffer char_;                        // 0x0000
    UnkScreenBuffer screen_;                    // 0x0010
    char charData_[0xc000];                     // 0x0024  32x48 tiles
    unsigned short screenData_[0x600];          // 0xC024  32x48 cells

    void unkfunc_0204fe2c();
    UnkCharBuffer* unkfunc_0204fe54();
    UnkScreenBuffer* unkfunc_0204fe58();
};

// VRAM slot of 32 icon characters
struct UnkMenuIconSlot {
    unsigned int key_;                          // 0x00  icon type | index, -1: none
    int offset_;                                // 0x04  first character
    int state_;                                 // 0x08  0: free, 1: used this frame, 2: cached

    void unkfunc_0204fe5c(int offset);
    void unkfunc_0204fe6c(unsigned int key);
    unsigned int unkfunc_0204fe70();
    void unkfunc_0204fe74(int state);
    int unkfunc_0204fe78();
    int unkfunc_0204fe7c();
};

// Queued icon draw
struct UnkMenuIconRequest {
    int enable_;                                // 0x00
    int x_;                                     // 0x04
    int y_;                                     // 0x08
    int w_;                                     // 0x0C
    int h_;                                     // 0x10
    int type_;                                  // 0x14
    int index_;                                 // 0x18

    int unkfunc_0204fe80(int x, int y, int w, int h, int type, int index);
};

// Sub screen icon BG with its VRAM character cache (data_02108518)
struct UnkMenuIconBg {
    int enable_;                                // 0x0000
    UnkScreenBuffer screen_;                    // 0x0004
    unsigned short screenData_[0x600];          // 0x0018
    UnkMenuIconSlot slots_[28];                 // 0x0C18
    UnkMenuIconRequest requests_[56];           // 0x0D68
    int requestCount_;                          // 0x1388

    UnkMenuIconBg();
    void unkfunc_0204fea8(int enable);
    void unkfunc_0204fef8();                    // load the icon palettes
    void unkfunc_0204ff44();                    // clear the screen
    int unkfunc_0204ff50(unsigned int key, void* src, int size);
    int unkfunc_0204fffc(unsigned int key);
    int unkfunc_02050038(unsigned int key);
    int unkfunc_020500b0();
    int unkfunc_020500d8(unsigned int key);
    int unkfunc_0205010c();
    void unkfunc_02050134(int x, int y, int w, int h, int type, int index);
    void unkfunc_020501ec(int x, int y, int w, int h, int type, int index);
    void unkfunc_02050224();
    void unkfunc_02050290(int type);
    void unkfunc_020502e4(int frame);
};

// Sub screen back BG with a color fade (data_020facbc)
struct UnkMenuBackBg {
    UnkScreenBuffer screen_;                    // 0x000
    unsigned short screenData_[0x600];          // 0x014
    int enable_;                                // 0xC14
    int r_;                                     // 0xC18
    int g_;                                     // 0xC1C
    int b_;                                     // 0xC20
    int targetR_;                               // 0xC24
    int targetG_;                               // 0xC28
    int targetB_;                               // 0xC2C
    int frame_;                                 // 0xC30
    int unk_c34;                                // 0xC34

    void unkfunc_020504a0();
    UnkScreenBuffer* unkfunc_020504d4();
    void unkfunc_020504d8(int r, int g, int b, int frame);
    void unkfunc_02050500();
};

// Sub screen menu BGs (data_020facb8)
struct UnkMenuBg {
    int transferCount_;                         // 0x0000  frames left to transfer the BGs
    UnkMenuBackBg back_;                        // 0x0004
    UnkMenuWindowBg window_;                    // 0x0C3C
    UnkMenuIconBg icon_;                        // 0xD860
};

extern UnkMenuBg data_020facb8;

void unkfunc_02050494();                        // transfer the sub screen BGs on the next two frames
