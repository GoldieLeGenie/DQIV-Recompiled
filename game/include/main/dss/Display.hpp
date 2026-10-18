#pragma once
#include <globaldefs.h>
#include "main/dss/DisplayPlugin.hpp"
#include "main/dss/BgBuffer.hpp"
#include "main/dss/VramRequest.hpp"

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

    UnkDisplay() { init(); }
    UnkDisplay* init();             // init
};

extern UnkDisplay g_Display;

void getCaptureOffset(int* x, int* y);          // capture position
void setCaptureOffset(int x, int y);
void resetCaptureOffset();
void changeDisplayPlugin(dss::DisplayPlugin* plugin);  // change the plugin now
void clearOamBuffers();                        // clear the OAM buffers
void updateDisplay();                        // update
void requestDisplayPlugin(dss::DisplayPlugin* plugin);  // change the plugin
int isDisplayTextReady();                         // plugin changed and text shown
int getDisplayFrame();                         // frame counter
void setBackdropColor(unsigned short color);    // backdrop color
void unkfunc_02081274(unsigned short value);
void setDisplayBlendMode(int mode);                // blend mode
UnkPaletteBuffer* getBgPaletteBuffer(int bg);     // BG palette (0-3 main, 4-7 sub)
UnkPaletteBuffer* getObjPaletteBuffer(int screen); // OBJ palette
UnkOamBuffer* getOamBuffer(int screen);     // OAM buffer
UnkOamBuffer* getOamBufferAtY(int y);          // OAM buffer of a position (0: main screen, 192: sub screen)
int getTextBgAtY(int y);                    // text BG of a position
int unkfunc_02081608(int y);
void setDisplayTextBgs(int type, int screen1, int screen2, int unk_0c, int unk_10, int unk_14);
int unkfunc_0208170c();
UnkScreenBuffer* getDebugScreenBuffer();            // debug screen
void setDisplayTextCounter(int counter);             // show the text
void setWindow0Enable(int enable);              // window 0
void setWindow1Enable(int enable);              // window 1
void setWindow0(int plane, int x, int y, int w, int h);
void setWindow1(int plane, int x, int y, int w, int h);
int getDisplayPluginType();                         // plugin type
void requestScreenCapture();                        // request a screen capture
void updateScreenCapture();                        // capture step
int isScreenCaptureDone();                         // screen capture done

extern "C" {
    void func_0206500c(unsigned int* reg, int plane, int brightness);   // brightness blend
    void _G2_SetBlend(unsigned int* reg, int srcPlane, int dstPlane, int eva, int evb);
}
