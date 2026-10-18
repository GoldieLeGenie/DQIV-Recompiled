#pragma once
#include <globaldefs.h>
#include "main/debug/DebugDisplay.hpp"
#include "main/status/GameFlag.hpp"

// Debug info page: position, flags, message IDs, world/play time, heap and dss array warnings
struct DebugInfo : DebugDisplay {
    // vtable                                   // 0x000
    int showPosition_;                          // 0x004
    int showWalk_;                              // 0x008
    int showWorldTime_;                         // 0x00C
    int showPlayTime_;                          // 0x010
    int showMessageID_;                         // 0x014
    int showFlag_;                              // 0x018
    int showHeap_;                              // 0x01C
    int showArrayWarning_;                      // 0x020
    int areaFlagIndex_;                         // 0x024  first flag shown
    int globalFlagIndex_;                       // 0x028
    int localFlagIndex_;                        // 0x02C
    status::GameFlag areaFlag_;                 // 0x030  previous flags (changed ones blink)
    status::GameFlag localFlag_;                // 0x0B0
    status::GameFlag globalFlag_;               // 0x130
    short lastDir_;                             // 0x1B0
    int lastX_;                                 // 0x1B4
    int lastY_;                                 // 0x1B8
    int lastZ_;                                 // 0x1BC
    char dirText_[0x20];                        // 0x1C0
    char xText_[0x20];                          // 0x1E0
    char yText_[0x20];                          // 0x200
    char zText_[0x20];                          // 0x220
    int messageID_[16];                         // 0x240

    virtual void initialize();
    virtual void draw();
    void drawArrayWarnings(int x, int y);        // dss array warnings
    void drawPosition(int x, int y);        // party position and direction
    void clearMessageIDs();                    // clear the message IDs
    void addMessageID(int messageID);       // add a message ID
    void drawMessageIDs(int x, int y);        // message IDs
    void drawFlags(int x, int y);        // game flags
    void drawFlagTable(int x, int y, char name, status::GameFlag* flag, int index, int lines, status::GameFlag* prev);
    void drawWorldTime(int x, int y);        // world time
    void drawWalkCounter(int x, int y);        // walk counter
    void drawPlayTime(int x, int y);        // play time
    void drawHeap(int x, int y);        // heap and VRAM
    void setShowArrayWarning(int show);            // show the array warnings
};

extern DebugInfo g_DebugInfo;
