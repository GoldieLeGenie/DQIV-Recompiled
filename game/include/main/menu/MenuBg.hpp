#pragma once
#include <globaldefs.h>
#include "main/dss/BgBuffer.hpp"

// Sub screen window BG: characters + screen (data_020fb8f4)
struct UnkMenuWindowBg {
    UnkCharBuffer char_;                        // 0x0000
    UnkScreenBuffer screen_;                    // 0x0010
    char charData_[0xc000];                     // 0x0024  32x48 tiles
    unsigned short screenData_[0x600];          // 0xC024  32x48 cells

    void init();
    UnkCharBuffer* getCharBuffer();
    UnkScreenBuffer* getScreenBuffer();
};

// VRAM slot of 32 icon characters
struct UnkMenuIconSlot {
    unsigned int key_;                          // 0x00  icon type | index, -1: none
    int offset_;                                // 0x04  first character
    int state_;                                 // 0x08  0: free, 1: used this frame, 2: cached

    void init(int offset);
    void setKey(unsigned int key);
    unsigned int getKey();
    void setState(int state);
    int getState();
    int getOffset();
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

    int set(int x, int y, int w, int h, int type, int index);
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
    void init(int enable);
    void loadPalettes();                    // load the icon palettes
    void clearScreen();                    // clear the screen
    int loadIcon(unsigned int key, void* src, int size);
    int findSlot(unsigned int key);
    int findSameIconSlot(unsigned int key);
    int findFreeSlot();
    int findCachedSlot(unsigned int key);
    int findReusableSlot();
    void drawIcon(int x, int y, int w, int h, int type, int index);
    void requestIcon(int x, int y, int w, int h, int type, int index);
    void drawRequests();
    void drawRequestsOfType(int type);
    void flush(int frame);
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

    void init();
    UnkScreenBuffer* getScreenBuffer();
    void startFade(int r, int g, int b, int frame);
    void update();
};

// Sub screen menu BGs (g_MenuBg)
struct UnkMenuBg {
    int transferCount_;                         // 0x0000  frames left to transfer the BGs
    UnkMenuBackBg back_;                        // 0x0004
    UnkMenuWindowBg window_;                    // 0x0C3C
    UnkMenuIconBg icon_;                        // 0xD860
};

extern UnkMenuBg g_MenuBg;

void requestMenuBgTransfer();                        // transfer the sub screen BGs on the next two frames
