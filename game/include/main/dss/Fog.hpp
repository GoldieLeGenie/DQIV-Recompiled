#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

extern int data_0211d310;                       // fog enabled
extern dss::Fix32 data_0211d314[2];
extern int data_0211d31c[2];                    // fog offset of each screen
extern unsigned int data_0211d324[2][8];        // fog table of each screen

void initFog();                        // init
void setFogOffset(int side, int offset);    // fog offset
void setFogRate(int side, dss::Fix32 rate);   // fog rate
void setFogColor(int r, int g, int b);     // fog color
void setFogEnable(int enable);              // fog enable
void updateFog();                        // update

extern "C" {
    void func_02065350(int enable, int mode, int slope, int offset);   // G3X_SetFog
    void func_02065408(const unsigned int* table);                      // G3X_SetFogTable
}
