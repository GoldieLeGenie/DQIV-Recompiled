#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

// 3D opening scene before the title 
struct LogoPart : GlobalGamePart {
    int state_;                                 // 0x04
    int frame_;                                 // 0x08

    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    void unkfunc_02009f10(int state);           // set state
};

extern LogoPart g_LogoPart;
