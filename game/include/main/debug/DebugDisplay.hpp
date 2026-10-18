#pragma once
#include <globaldefs.h>
#include "main/debug/AutoBattleTest.hpp"
#include "main/dss/ArrayErrorLog.hpp"

// Base of the debug info pages drawn on the debug console (one page per part slot)
struct DebugDisplay {
    virtual void initialize() = 0;
    virtual void draw() = 0;
};

void debugConsolePrintf(int x, int y, const char* fmt, ...);  // console printf
void debugPrint(int x, int y, const char* str);      // console print (not while a menu is requested)
void drawDebugBox(int x, int y, int w, int h);         // sub screen back BG box
void drawDebugWindow(int x, int y, int w, int h);         // sub screen window frame
void initDebugDisplay();                                   // end of the part initialize
void updateDebugDisplay();                                   // part onDebugPart
void setDebugDisplayPage(int type);                           // select the page of the current part slot
DebugDisplay* getDebugDisplayPage(int type);               // 1: info page, 2: battle page
int getDebugDisplaySlot();                                    // part slot of the current task
