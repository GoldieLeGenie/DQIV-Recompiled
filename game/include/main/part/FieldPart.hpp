#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

struct FieldPart : GlobalGamePart {
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    virtual void onSwapBuffersPart();
};

extern FieldPart g_FieldPart;
