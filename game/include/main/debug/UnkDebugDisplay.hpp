#pragma once
#include <globaldefs.h>
#include "main/debug/UnkBattleTest.hpp"
#include "main/dss/UnkArrayWarning.hpp"

// Base of the debug info pages drawn on the debug console (one page per part slot)
struct UnkDebugDisplay {
    virtual void initialize() = 0;
    virtual void draw() = 0;
};

void unkfunc_0202c17c(int x, int y, const char* fmt, ...);  // console printf
void unkfunc_0202c1b4(int x, int y, const char* str);      // console print (not while a menu is requested)
void unkfunc_0202c1d8(int x, int y, int w, int h);         // sub screen back BG box
void unkfunc_0202c20c(int x, int y, int w, int h);         // sub screen window frame
void unkfunc_0202c25c();                                   // end of the part initialize
void unkfunc_0202c284();                                   // part onDebugPart
void unkfunc_0202c300(int type);                           // select the page of the current part slot
UnkDebugDisplay* unkfunc_0202c330(int type);               // 1: info page, 2: battle page
int unkfunc_0202c350();                                    // part slot of the current task
