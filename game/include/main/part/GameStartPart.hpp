#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

struct GameStartPart : GlobalGamePart {
    int cursorPos_;                             // 0x04
    int cardcheck_;                             // 0x08

    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
};

extern GameStartPart g_GameStartPart;
