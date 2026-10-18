#pragma once
#include <globaldefs.h>

void loadBillboardMatrix();                        // billboard: keep only the translation of the current matrix
void setupOrtho2D();                        // 2D projection (256x192, depth 1)
void setupOrtho2DFine();                        // 2D projection (256x192, depth 1/64)
void setupOrtho2DBackground();                        // 2D projection (256x192, depth 1024)
void drawBackgroundGradient();                        // draw the background gradient
void setBackgroundColor(int r, int g, int b);     // background gradient: one color everywhere
void setBackgroundGradient(unsigned char* bottomUpLeft, unsigned char* bottomUpRight, unsigned char* bottomDownLeft, unsigned char* bottomDownRight,
                      unsigned char* topUpLeft, unsigned char* topUpRight, unsigned char* topDownLeft, unsigned char* topDownRight);
void setBackgroundGradientEnable(int enable);              // background gradient on/off
void drawSubScreenShade();
