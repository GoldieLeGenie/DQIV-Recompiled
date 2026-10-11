#pragma once
#include <globaldefs.h>
#include "main/dss/UnkBgBuffer.hpp"
#include "main/dss/UnkFont.hpp"

// text drawing state of the BG text engine
struct UnkBgText {
    int size_;                                  // 0x00 font size (12 or 10)
    UnkFont font12_;                            // 0x04
    UnkFont font10_;                            // 0x14
    UnkFont subFont12_;                         // 0x24 latin chars
    UnkFont subFont10_;                         // 0x34 latin chars
    unsigned char charWidths_[0x100];           // 0x44 widths of the last text
    int count_;                                 // 0x144 char count of the last text
    int width_;                                 // 0x148 width of the last text
    int height_;                                // 0x14C height of the last text
    UnkFont* font_;                             // 0x150
    UnkFont* subFont_;                          // 0x154
    int lineHeight_;                            // 0x158
    int mode_;                                  // 0x15C full-width digits glyphs (1 or 2)
};

extern UnkBgText data_0211a664;

void unkfunc_0207f900();                        // init
void unkfunc_0207f924(void* data, void* subData);   // load the 12 dots fonts
void unkfunc_0207f95c(void* data, void* subData);   // load the 10 dots fonts
int unkfunc_0207f994(UnkCharBuffer* buffer, int x, int y, int color, const char* text);   // draw a text (NULL: measure)
int unkfunc_0207f9b8(UnkCharBuffer* buffer, int x, int y, int color, const char* text, int wrap);
int unkfunc_0207fc88(UnkG2dCanvas* canvas, int x, int y, int color, unsigned short c);    // draw a char (NULL: width)
void unkfunc_02080038(int size);                // set the font size
int unkfunc_02080098(int size);                 // line height
int unkfunc_020800c0(unsigned char* widths, int max);   // char widths of the last text
int unkfunc_02080100();                         // width of the last text
