#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

// Boot logos 
struct MenuPart : GlobalGamePart {
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
};

extern MenuPart g_MenuPart;
