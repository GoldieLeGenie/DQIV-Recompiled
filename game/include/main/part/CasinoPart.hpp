#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

struct CasinoPart : GlobalGamePart {
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
};

extern CasinoPart g_CasinoPart;
