#ifndef G2D_CHARCANVAS_H
#define G2D_CHARCANVAS_H

#include "g2d_font.h"

struct UnkCharCanvas;

typedef struct UnkGlyph {
    UnkCharWidths* unk_00;      // 0x00 widths
    u8* unk_04;                 // 0x04 glyph image
} UnkGlyph;

typedef void (*UnkDrawGlyphFunc)(struct UnkCharCanvas* cc, UnkFont* font, int x, int y, int cl, UnkGlyph* glyph);
typedef void (*UnkClearFunc)(struct UnkCharCanvas* cc, int cl);
typedef void (*UnkClearAreaFunc)(struct UnkCharCanvas* cc, int cl, int x, int y, int w, int h);

typedef struct UnkCharCanvasVTable {
    UnkDrawGlyphFunc unk_00;    // 0x00 draw glyph
    UnkClearFunc unk_04;        // 0x04 clear
    UnkClearAreaFunc unk_08;    // 0x08 clear area
} UnkCharCanvasVTable;

/* BG canvas: characters per row; OBJ canvas: object width / height shifts. */
typedef union UnkCharCanvasLayout {
    int unk_00;                 // 0x00 characters per row
    struct {
        u32 unk_00 : 8;         // 0x00 object width shift
        u32 unk_08 : 8;         // 0x01 object height shift
        u32 unk_10 : 16;        // 0x02
    } unk_obj;
} UnkCharCanvasLayout;

typedef struct UnkCharCanvas {
    u8* unk_00;                 // 0x00 character data
    int unk_04;                 // 0x04 width in characters
    int unk_08;                 // 0x08 height in characters
    u8 unk_0c;                  // 0x0C bits per pixel
    UnkCharCanvasLayout unk_10; // 0x10 layout parameter (BG: row width; OBJ: size shifts)
    const UnkCharCanvasVTable* unk_14; // 0x14 functions
} UnkCharCanvas;

/* Parameters for drawing the part of a glyph that falls into one character. */
#ifndef UNK_GLYPH_CHAR_PARAM_DEFINED
#define UNK_GLYPH_CHAR_PARAM_DEFINED
typedef struct UnkGlyphCharParam {
    void* unk_00;               // 0x00 destination character
    u8* unk_04;                 // 0x04 glyph image
    int unk_08;                 // 0x08 glyph x relative to the character
    int unk_0c;                 // 0x0C glyph y relative to the character
    int unk_10;                 // 0x10 glyph width
    int unk_14;                 // 0x14 glyph height
    int unk_18;                 // 0x18 glyph row size in bits
    int unk_1c;                 // 0x1C glyph bits per pixel
    int unk_20;                 // 0x20 destination bits per pixel
    int unk_24;                 // 0x24 color offset
} UnkGlyphCharParam;
#endif

typedef struct UnkBitReader {
    u8* unk_00;                 // 0x00 source
    s8 unk_04;                  // 0x04 bits left in unk_05
    u8 unk_05;                  // 0x05 current byte
} UnkBitReader;

typedef struct UnkChar8bppRow {
    u32 unk_00;                 // 0x00 pixels 0-3
    u32 unk_04;                 // 0x04 pixels 4-7
} UnkChar8bppRow;

extern void MI_CpuFill(u32 value, void* dest, u32 size);
u32 func_0206a114(UnkBitReader* reader, int nBits);
void func_02069d6c(UnkCharCanvas* cc, void* charBase, int areaW, int areaH, int bpp, const UnkCharCanvasVTable* vtable, u32 param);

#endif
