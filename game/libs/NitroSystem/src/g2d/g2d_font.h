#ifndef G2D_FONT_H
#define G2D_FONT_H

#include <nitro/types.h>

/* Character width entry (3 bytes). */
typedef struct UnkCharWidths {
    s8 unk_00;                  // 0x00 left space
    u8 unk_01;                  // 0x01 glyph width
    s8 unk_02;                  // 0x02 character width
} UnkCharWidths;

/* Glyph image block. */
typedef struct UnkFontGlyph {
    u8 unk_00;                  // 0x00 cell width
    u8 unk_01;                  // 0x01 cell height
    u16 unk_02;                 // 0x02 bytes per glyph
    s8 unk_04;                  // 0x04 baseline
    u8 unk_05;                  // 0x05 max character width
    u8 unk_06;                  // 0x06 bits per pixel
    u8 unk_07;                  // 0x07 glyph rotation / flags
    u8 unk_08[1];               // 0x08 glyph images
} UnkFontGlyph;

/* Character width block. */
typedef struct UnkFontWidth {
    u16 unk_00;                 // 0x00 first glyph index
    u16 unk_02;                 // 0x02 last glyph index
    struct UnkFontWidth* unk_04; // 0x04 next block
    UnkCharWidths unk_08[1];    // 0x08 width table
} UnkFontWidth;

/* Character code map block. */
typedef struct UnkFontCodeMap {
    u16 unk_00;                 // 0x00 first character code
    u16 unk_02;                 // 0x02 last character code
    u16 unk_04;                 // 0x04 mapping method (0 direct, 1 table, 2 scan)
    u16 unk_06;                 // 0x06
    struct UnkFontCodeMap* unk_08; // 0x08 next block
    u16 unk_0c[1];              // 0x0C mapping data
} UnkFontCodeMap;

/* Font information block. */
typedef struct UnkFontInfo {
    u8 unk_00;                  // 0x00 font type
    s8 unk_01;                  // 0x01 line feed
    u16 unk_02;                 // 0x02 replacement glyph index
    UnkCharWidths unk_04;       // 0x04 default widths
    u8 unk_07;                  // 0x07 character encoding
    UnkFontGlyph* unk_08;       // 0x08 glyphs
    UnkFontWidth* unk_0c;       // 0x0C widths
    UnkFontCodeMap* unk_10;     // 0x10 code maps
} UnkFontInfo;

typedef u16 (*UnkCharSplitter)(const void** str);

typedef struct UnkFont {
    UnkFontInfo* unk_00;        // 0x00 font information
    UnkCharSplitter unk_04;     // 0x04 string splitter for the font encoding
} UnkFont;

u16 func_02069060(UnkFont* font, u16 c);
UnkCharWidths* func_020690a8(UnkFont* font, u16 index);
BOOL func_02069ee4(void* resource, UnkFontInfo** info);
void func_0206a038(void* header);

/* string splitters (g2d_splitchar.c) */
u16 func_0206a174(const void** str);
u16 func_0206a188(const void** str);
u16 func_0206a204(const void** str);
u16 func_0206a258(const void** str);

#endif
