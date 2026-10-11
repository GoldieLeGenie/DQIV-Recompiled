#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

// Debug part 
struct IshikuroTestPart : GlobalGamePart {
    IshikuroTestPart();
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
};

extern IshikuroTestPart g_IshikuroTestPart;
