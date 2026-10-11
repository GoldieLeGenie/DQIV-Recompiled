#pragma once
#include <globaldefs.h>

// NitroSystem 2D char canvas
struct UnkG2dCanvas {
    void* charBase_;                            // 0x00
    int areaWidth_;                             // 0x04
    int areaHeight_;                            // 0x08
    unsigned char dstBpp_;                      // 0x0C
    const void* vtable_;                        // 0x10
    unsigned int param_;                        // 0x14
};

// font information block of a font file
struct UnkFontInfo {
    unsigned char unk_00;                       // 0x00
    signed char unk_01;                         // 0x01
    unsigned short alterCharIndex_;             // 0x02 glyph drawn for the missing chars
};

// widths of a glyph
struct UnkCharWidths {
    signed char unk_00;                         // 0x00
    unsigned char unk_01;                       // 0x01
    signed char charWidth_;                     // 0x02
};

// NitroSystem 2D font
struct UnkG2dFont {
    UnkFontInfo* info_;                         // 0x00
    void* unk_04;                               // 0x04
};

extern "C" {
    void func_02069030(UnkG2dFont* font, void* data);                                      // init from a font file
    unsigned short func_02069060(UnkG2dFont* font, unsigned short c);                      // glyph index (0xffff: none)
    UnkCharWidths* func_020690a8(UnkG2dFont* font, unsigned short index);                  // glyph widths
    void func_02069eb8(UnkG2dCanvas* canvas, void* charBase, int areaWidth, int areaHeight, int colorMode);  // init for a BG
    int func_02069d94(UnkG2dCanvas* canvas, UnkG2dFont* font, int x, int y, int color, unsigned short c);   // draw a char
}

// text font of the dss engine
struct UnkFont {
    UnkG2dFont font_;                           // 0x00
    int unk_08;                                 // 0x08
    int height_;                                // 0x0C

    UnkFont();
    void unkfunc_02088554(void* data, int height);
    int unkfunc_0208858c(UnkG2dCanvas* canvas, int x, int y, int color, unsigned short c);    // draw a char (NULL: width)
};
