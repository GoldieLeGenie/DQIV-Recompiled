#pragma once
#include <globaldefs.h>

// NitroSystem 2D char canvas
struct G2dCharCanvas {
    void* charBase_;                            // 0x00
    int areaWidth_;                             // 0x04
    int areaHeight_;                            // 0x08
    unsigned char dstBpp_;                      // 0x0C
    const void* vtable_;                        // 0x10
    unsigned int param_;                        // 0x14
};

// font information block of a font file
struct G2dFontInfo {
    unsigned char fontType_;                       // 0x00
    signed char linefeed_;                         // 0x01
    unsigned short alterCharIndex_;             // 0x02 glyph drawn for the missing chars
};

// widths of a glyph
struct GlyphWidths {
    signed char left_;                         // 0x00
    unsigned char glyphWidth_;                       // 0x01
    signed char charWidth_;                     // 0x02
};

// NitroSystem 2D font
struct G2dFont {
    G2dFontInfo* info_;                         // 0x00
    void* splitChar_;                               // 0x04
};

extern "C" {
    void func_02069030(G2dFont* font, void* data);                                      // init from a font file
    unsigned short func_02069060(G2dFont* font, unsigned short c);                      // glyph index (0xffff: none)
    GlyphWidths* func_020690a8(G2dFont* font, unsigned short index);                  // glyph widths
    void func_02069eb8(G2dCharCanvas* canvas, void* charBase, int areaWidth, int areaHeight, int colorMode);  // init for a BG
    int func_02069d94(G2dCharCanvas* canvas, G2dFont* font, int x, int y, int color, unsigned short c);   // draw a char
}

// text font of the dss engine
struct DssFont {
    G2dFont font_;                           // 0x00
    int unk_08;                                 // 0x08
    int height_;                                // 0x0C

    DssFont();
    void load(void* data, int height);
    int drawChar(G2dCharCanvas* canvas, int x, int y, int color, unsigned short c);    // draw a char (NULL: width)
};
