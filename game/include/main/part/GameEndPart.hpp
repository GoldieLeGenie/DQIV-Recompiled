#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

// Shows a save/card error message then goes to the next part
struct GameEndPart : GlobalGamePart {
    int type_;                                  // 0x04
    int message_;                               // 0x08
    int nextPart_;                              // 0x0C

    GameEndPart();
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    void setType(int type);
    void setTypeAndNextPart(int type, int nextPart);
    void start(int type);
    void startWithNextPart(int type, int nextPart);
};

extern GameEndPart g_GameEndPart;
