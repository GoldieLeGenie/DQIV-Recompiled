#pragma once
#include <globaldefs.h>

void unkfunc_020843d4();                        // billboard: keep only the translation of the current matrix
void unkfunc_020847e8();                        // 2D projection (256x192, depth 1)
void unkfunc_020848a8();                        // 2D projection (256x192, depth 1/64)
void unkfunc_02084964();                        // 2D projection (256x192, depth 1024)
void unkfunc_02084a1c();                        // draw the background gradient
void unkfunc_02084c78(int r, int g, int b);     // background gradient: one color everywhere
void unkfunc_02084cec(unsigned char* bottomUpLeft, unsigned char* bottomUpRight, unsigned char* bottomDownLeft, unsigned char* bottomDownRight,
                      unsigned char* topUpLeft, unsigned char* topUpRight, unsigned char* topDownLeft, unsigned char* topDownRight);
void unkfunc_02084dd4(int enable);              // background gradient on/off
void unkfunc_02084de4();
