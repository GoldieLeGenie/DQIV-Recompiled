#pragma once
#include <globaldefs.h>
#include "main/dss/DisplayPlugin.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/dss/UnkVramRequest.hpp"

// BG used for the system text on each screen
struct UnkDisplayText {
    int type_;                                  // 0x00 0: none, 1: main + sub, 2: sub + main, 3: alternated
    int screen1_;                               // 0x04 BG of the first screen
    int screen2_;                               // 0x08 BG of the second screen
    int unk_0c;                                 // 0x0C
    int unk_10;                                 // 0x10
    int unk_14;                                 // 0x14
};

// window of the main screen
struct UnkDisplayWindow {
    int plane_;                                 // 0x00
    int x_;                                     // 0x04
    int y_;                                     // 0x08
    int w_;                                     // 0x0C
    int h_;                                     // 0x10
};

// display manager: plugin, palettes and OAM shadow buffers, debug screen, windows, capture
struct UnkDisplay {
    dss::DisplayPlugin* plugin_;                // 0x0000
    dss::DisplayPlugin* nextPlugin_;            // 0x0004
    unsigned int frame_;                        // 0x0008
    unsigned short backColor_;                  // 0x000C
    unsigned short unk_0e;                      // 0x000E
    int blendMode_;                             // 0x0010
    UnkPaletteBuffer bgPalette_[2];             // 0x0014
    UnkPaletteBuffer objPalette_[2];            // 0x0414
    UnkOamBuffer oam_[2];                       // 0x0814
    UnkDisplayText text_;                       // 0x102C
    UnkScreenBuffer screen_;                    // 0x1044 debug screen
    unsigned short screenData_[2][0x300];       // 0x1058
    int windowEnable_[2];                       // 0x1C58
    UnkDisplayWindow window_[2];                // 0x1C60
    char unk_1c88[0x50];                        // 0x1C88
    int capture_;                               // 0x1CD8
    int captureStep_;                           // 0x1CDC
    int captureX_;                              // 0x1CE0
    int captureY_;                              // 0x1CE4
    int blendA_;                                // 0x1CE8
    int blendB_;                                // 0x1CEC

    UnkDisplay() { unkfunc_02080dd8(); }
    UnkDisplay* unkfunc_02080dd8();             // init
};

extern UnkDisplay data_0211a7d0;

void unkfunc_02080e48(int* x, int* y);          // capture position
void unkfunc_02080e64(int x, int y);
void unkfunc_02080e78();
void unkfunc_02080e90(dss::DisplayPlugin* plugin);  // change the plugin now
void unkfunc_02080f3c();                        // clear the OAM buffers
void unkfunc_02080f5c();                        // update
void unkfunc_0208120c(dss::DisplayPlugin* plugin);  // change the plugin
int unkfunc_0208121c();                         // plugin changed and text shown
int unkfunc_02081254();                         // frame counter
void unkfunc_02081264(unsigned short color);    // backdrop color
void unkfunc_02081274(unsigned short value);
void unkfunc_02081284(int mode);                // blend mode
UnkPaletteBuffer* unkfunc_02081364(int bg);     // BG palette (0-3 main, 4-7 sub)
UnkPaletteBuffer* unkfunc_020813e0(int screen); // OBJ palette
UnkOamBuffer* unkfunc_0208141c(int screen);     // OAM buffer
UnkOamBuffer* unkfunc_02081454(int y);          // OAM buffer of a position (0: main screen, 192: sub screen)
int unkfunc_02081534(int y);                    // text BG of a position
int unkfunc_02081608(int y);
void unkfunc_020816dc(int type, int screen1, int screen2, int unk_0c, int unk_10, int unk_14);
int unkfunc_0208170c();
UnkScreenBuffer* unkfunc_0208171c();            // debug screen
void unkfunc_02081728(int counter);             // show the text
void unkfunc_0208174c(int enable);              // window 0
void unkfunc_0208175c(int enable);              // window 1
void unkfunc_0208176c(int plane, int x, int y, int w, int h);
void unkfunc_02081794(int plane, int x, int y, int w, int h);
int unkfunc_020817bc();                         // plugin type
void unkfunc_020817d8();                        // request a screen capture
void unkfunc_02081814();                        // capture step
int unkfunc_0208198c();                         // screen capture done

extern "C" {
    void func_0206500c(unsigned int* reg, int plane, int brightness);   // brightness blend
    void _G2_SetBlend(unsigned int* reg, int srcPlane, int dstPlane, int eva, int evb);
}
