#pragma once
#include <globaldefs.h>

// debug text console on the debug BG screen
struct DssDebugConsole {
    DssDebugConsole() { unkfunc_0207e800(); }
    void unkfunc_0207e800();                    // init
    void unkfunc_0207e804();                    // clear
    void unkfunc_0207e810();                    // clear
    void unkfunc_0207e824(int x, int y, int w, int h);  // clear a rect
    void unkfunc_0207e864(int x, int y, const char* str);
    void unkfunc_0207e88c(int x, int y, const char* format, ...);
    void unkfunc_0207e8e0(int x, int y, int w, int h);  // clear a rect
    void unkfunc_0207e8f4(int x, int y);
};

extern DssDebugConsole g_DebugConsole;
extern int g_mainExArenaEnabled;                       // main extended arena enabled

void unkfunc_0207e7e0();
void unkfunc_0207e7e4();
int getTick();                         // tick count
void debugPrintfStub(const char* format, ...); // debug print
void dssVBlankIntr();                        // V-blank interrupt
void initSystem(int exArena);             // OS/system init
void clearVram();                        // clear the VRAM, OAM and palettes
unsigned int roundUp(unsigned int value, unsigned int align);  // round up
unsigned int roundDown(unsigned int value, unsigned int align);  // round down
void unkfunc_0207ecf4();                        // reset the BG/OBJ VRAM
void setMainBrightness(int brightness);          // main screen master brightness
void setSubBrightness(int brightness);          // sub screen master brightness

extern "C" {
    unsigned long long func_02079bf0(void);     // OS_GetTick
    void func_0207b708(void);
    void func_0206366c(void);
    void func_02063920(int);
    void func_02079b10(void);
    void func_02077480(int type, void (*function)());   // OS_SetIrqFunction
    void func_02077650(int mask);               // OS_EnableIrqMask
    void func_02060ddc(int dma);                // FS_Init
    void func_0207315c(void);
    void func_02064abc(void);                   // GX_DisableBankForLcdc
    void func_02065088(void);
    void func_02065228(void);
    void func_0206dfa4(void);
    void func_020638f8(void* reg, int brightness);  // master brightness
}
