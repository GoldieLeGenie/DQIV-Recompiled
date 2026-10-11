#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

extern int data_0211d310;                       // fog enabled
extern dss::Fix32 data_0211d314[2];
extern int data_0211d31c[2];                    // fog offset of each screen
extern unsigned int data_0211d324[2][8];        // fog table of each screen

void unkfunc_0208313c();                        // init
void unkfunc_020831d8(int side, int offset);    // fog offset
void unkfunc_020831e8(int side, dss::Fix32 rate);   // fog rate
void unkfunc_0208328c(int r, int g, int b);     // fog color
void unkfunc_020832b0(int enable);              // fog enable
void unkfunc_020832d8();                        // update

extern "C" {
    void func_02065350(int enable, int mode, int slope, int offset);   // G3X_SetFog
    void func_02065408(const unsigned int* table);                      // G3X_SetFogTable
}
