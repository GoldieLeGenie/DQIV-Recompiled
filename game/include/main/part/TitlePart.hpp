#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"
#include "main/dss/BuildDate.hpp"

// Debug part select screen 
struct TitlePart : GlobalGamePart {
    int x_;                                     // 0x04
    int cursor_;                                // 0x08
    BuildDate buildDate_;                       // 0x0C

    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
};

// entry of the TitlePart list 
struct TITLE_PART_ITEM {
    int part;                                   // 0x00
    char name[32];                              // 0x04
};

extern TitlePart g_TitlePart;
