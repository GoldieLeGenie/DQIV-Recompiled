#include "g2d_charcanvas.h"

/* Character canvas: renders font glyphs into 4/8 bpp character (tile) memory. */

/* OBJ area of 1/2/4/8 x 1/2/4/8 characters [h][w] -> (width shift, height shift) */
static const u8 data_020ba560[4][4][2] = {
    {{0, 0}, {1, 0}, {2, 0}, {2, 0}},
    {{0, 1}, {1, 1}, {2, 1}, {2, 1}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
    {{0, 2}, {1, 2}, {2, 3}, {3, 3}},
};

/* object (height shift, width shift) [h][w] -> OAM shape / size bits */
static const u16 data_020ba580[4][4][2] = {
    {{0x0000, 0x0000}, {0x4000, 0x0000}, {0x4000, 0x4000}, {0x0000, 0x0000}},
    {{0x8000, 0x0000}, {0x0000, 0x4000}, {0x4000, 0x8000}, {0x0000, 0x0000}},
    {{0x8000, 0x4000}, {0x8000, 0x8000}, {0x0000, 0x8000}, {0x4000, 0xc000}},
    {{0x0000, 0x0000}, {0x0000, 0x0000}, {0x8000, 0xc000}, {0x0000, 0xc000}},
};

static inline void MI_CpuFill32(void* dest, u32 data, u32 size) {
    MI_CpuFill(data, dest, size);
}

#define CHR_SIZE(bpp) (8 * 8 * (bpp) / 8)

static inline u32 CountLeadingZeros(u32 x) {
#ifdef DQIV_NATIVE
    u32 count = 0;
    if (x == 0) return 32;
    while (!(x & 0x80000000u)) { ++count; x <<= 1; }
    return count;
#else
    asm { clz x, x }
    return x;
#endif
}

/* Returns the OBJ size table entry (width shift, height shift) for an area of w*h characters. */
static inline const u8* GetObjShiftWH(int w, int h) {
    int sw = (w >= 8) ? 3 : 31 - CountLeadingZeros(w);
    int sh = (h >= 8) ? 3 : 31 - CountLeadingZeros(h);

    return data_020ba560[sh][sw];
}

static inline u32 GetLowBits(u32 v, int shift) {
    return v & ~(0xffffffff << shift);
}

static inline u32 GetHighBits(u32 v, int shift) {
    return v & (0xffffffff << shift);
}

/* Returns the character number of character (x, y) in an OBJ area made of 2D-mapped objects. */
u32 func_020690f4(u32 x, u32 y, int areaW, int areaH, int wShift, int hShift) {
    u32 charNo = 0;

    for (;;) {
        u32 ones = 0xffffffff;
        int ws = wShift;
        int hs = hShift;
        u32 fullW = GetHighBits(areaW, ws);
        u32 fullH = GetHighBits(areaH, hs);

        if (fullH <= y) {
            charNo += areaW * fullH;
            if (fullW <= x) {
                areaH -= fullH;
                charNo += fullW * areaH;
                x -= fullW;
                y -= fullH;
                areaW -= fullW;
            } else {
                areaW = fullW;
                y -= fullH;
                areaH -= fullH;
            }
        } else {
            u32 lowH = ~(ones << hs);

            if (fullW <= x) {
                charNo += fullW * fullH;
                areaH = fullH;
                x -= fullW;
                areaW -= fullW;
            } else {
                charNo += fullW * GetHighBits(y, hs);
                return charNo + (GetHighBits(x, ws) << hs) + ((y & lowH) << ws) + GetLowBits(x, ws);
            }
        }
        {
            const u8* shift = GetObjShiftWH(areaW, areaH);

            wShift = shift[0];
            hShift = shift[1];
        }
    }
}

/* Fills a w*h pixel rectangle at (x, y) of one character with a color pattern. */
void func_020691ec(void* chr, int x, int y, int w, int h, u32 pattern, u32 bpp) {
    if (w == 8 && h == 8) {
        MI_CpuFill(pattern, chr, bpp * 8);
        return;
    }

    if (bpp == 4) {
        int lsft;
        int rsft;
        u32 mask;
        u32 val;
        u32* p;
        u32* end;

        lsft = x * 4;
        rsft = 32 - (lsft + w * 4);
        mask = ((0xffffffff >> lsft) << (rsft + x * 4)) >> rsft;
        p = (u32*)chr + y;
        end = p + h;
        val = pattern & mask;
        mask = ~mask;
        for (; p < end; p++) {
            *p = val | (*p & mask);
        }
    } else {
        u32 maskLo;
        u32 maskHi;
        u32 t;
        u32 valLo;
        u32 valHi;
        UnkChar8bppRow* p;
        UnkChar8bppRow* end;

        x *= 8;
        w = 64 - (x + w * 8);
        maskLo = 0xffffffff >> x;
        if ((u32)w < 32) {
            maskLo <<= x;
        } else {
            u32 s = w - 32;
            maskLo = (maskLo << (x + s)) >> s;
        }
        t = 0xffffffff << w;
        if ((u32)x < 32) {
            maskHi = t >> w;
        } else {
            u32 s = x - 32;
            maskHi = (t >> (s + w)) << s;
        }

        p = (UnkChar8bppRow*)chr + y;
        end = p + h;
        valLo = pattern & maskLo;
        valHi = pattern & maskHi;
        maskLo = ~maskLo;
        maskHi = ~maskHi;
        for (; p < end; p++) {
            p->unk_00 = valLo | (p->unk_00 & maskLo);
            p->unk_04 = valHi | (p->unk_04 & maskHi);
        }
    }
}

static inline int UnkCanvasMin(int a, int b) { return (a <= b) ? a : b; }
static inline int UnkCanvasMax(int a, int b) { return (a >= b) ? a : b; }
static inline void UnkCanvasReaderInit(UnkBitReader* reader, const void* src) {
    reader->unk_04 = 0;
    reader->unk_00 = (u8*)src;
    reader->unk_05 = 0;
}
static inline u8 UnkCanvasFontHeight(const UnkFont* font) { return font->unk_00->unk_08->unk_01; }
static inline u8 UnkCanvasFontWidth(const UnkFont* font) { return font->unk_00->unk_08->unk_00; }
static inline u8 UnkCanvasFontBpp(const UnkFont* font) { return font->unk_00->unk_08->unk_06; }
static inline int UnkCanvasCharSize(const UnkCharCanvas* cc) { return CHR_SIZE(cc->unk_0c); }
/* LetterChar, adapted from the user-provided NitroSystem g2d_CharCanvas.c.
 * Copyright 2004-2007 Nintendo. All rights reserved. */
void func_02069324(UnkGlyphCharParam* i)
{
    const u8* pSrc;
    u32 x_st;
    u32 x_ed;
    u32 y_st;
    u32 y_ed;
    u32 offset;

    {
        u32 bit_y_begin;

        x_st = (unsigned int)UnkCanvasMax(i->unk_08, 0);
        y_st = (unsigned int)UnkCanvasMax(i->unk_0c, 0);
        x_ed = (unsigned int)UnkCanvasMin(8, i->unk_08 + i->unk_10);
        y_ed = (unsigned int)UnkCanvasMin(8, i->unk_0c + i->unk_14);

        bit_y_begin = (unsigned int)- UnkCanvasMin(i->unk_0c, 0);
        offset = - UnkCanvasMin(i->unk_08, 0) * i->unk_1c + bit_y_begin * i->unk_18;

        pSrc = i->unk_04;
    }

    {
        u32 x;
        const int dsrc = i->unk_18;
        const int srcBpp = i->unk_1c;
        const int dstBpp = i->unk_20;

        x_st *= dstBpp;
        x_ed *= dstBpp;

        if( dstBpp == 4 )
        {
            u32* pDst = (u32*)i->unk_00 + y_st;
            u32* pDstEnd = (u32*)i->unk_00 + y_ed;
            u32 cl = i->unk_24;

            for( ; pDst < pDstEnd; pDst++ )
            {
                UnkBitReader reader;
                u32 out_line = *pDst;

                UnkCanvasReaderInit(&reader, pSrc + offset/8);
                (void)func_0206a114(&reader, (int)offset%8);

                for( x = x_st; x < x_ed; x += 4 )
                {
                    u32 bits = func_0206a114(&reader, srcBpp);

                    if( bits != 0 )
                    {
                        out_line = (out_line & ~(0xF << x)) | ((cl + bits) << x);
                    }
                }

                *pDst = out_line;

                offset += dsrc;
            }
        }
        else
        {
            u32* pDst = (u32*)( (u64*)i->unk_00 + y_st );
            u32* const pDstEnd = (u32*)( (u64*)i->unk_00 + y_ed );
            u32 cl = i->unk_24;

            for( ; pDst < pDstEnd; pDst +=2 )
            {
                UnkBitReader reader;
                u32 out_line_0 = *pDst;
                u32 out_line_1 = *(pDst + 1);

                UnkCanvasReaderInit(&reader, pSrc + offset/8);
                (void)func_0206a114(&reader, (int)offset%8);

                for( x = x_st; x < x_ed; x += 8 )
                {
                    u32 bits = func_0206a114(&reader, srcBpp);

                    if( bits != 0 )
                    {
                        if( x < 32 )
                        {
                            out_line_0 = (out_line_0 & ~((u32)0xFF << x)) | ((u32)(cl + bits) << x);
                        }
                        else
                        {
                            const u32 x_32 =  x - 32;
                            out_line_1 = (out_line_1 & ~((u32)0xFF << x_32)) | ((u32)(cl + bits) << x_32);
                        }
                    }
                }

                *pDst = out_line_0;
                *(pDst + 1) = out_line_1;

                offset += dsrc;
            }
        }
    }
}
/* Draws a glyph into a BG-style canvas (characters laid out in rows of unk_10). */
void func_02069564(UnkCharCanvas* cc, UnkFont* font, int x, int y, int cl, UnkGlyph* glyph) {
    UnkFontGlyph* cglp = font->unk_00->unk_08;
    int areaW = cc->unk_04;
    int areaH = cc->unk_08;
    int glyphW = glyph->unk_00->unk_01;
    u8* charBase = cc->unk_00;
    int charSize = CHR_SIZE(cc->unk_0c);
    int glyphH = cglp->unk_01;
    int cx0;
    UnkGlyphCharParam param;
    int px;
    int ch;
    int xEnd;
    u32 cx1;
    int cw;
    int yEnd;
    int cy0;
    u8* dst;
    u32 cy1;
    int rowSkip;

    if (glyphW == 0) {
        return;
    }
    if (x + glyphW < 0) {
        return;
    }
    if (y + glyphH < 0) {
        return;
    }
    cx0 = (x <= 0) ? 0 : (u32)x / 8;
    cy0 = (y <= 0) ? 0 : (u32)y / 8;
    cx1 = (u32)(x + glyphW + 7) / 8;
    if (areaW <= cx1) {
        cx1 = areaW;
    }
    cy1 = (u32)(y + glyphH + 7) / 8;
    if (areaH <= cy1) {
        cy1 = areaH;
    }
    cw = cx1 - cx0;
    ch = cy1 - cy0;
    if (cw < 0 || ch < 0) {
        return;
    }

    if (x >= 0) {
        x &= 7;
    }
    dst = charBase + charSize * (cc->unk_10.unk_00 * cy0 + cx0);
    rowSkip = charSize * (cc->unk_10.unk_00 - cw);
    if (y >= 0) {
        y &= 7;
    }
    param.unk_04 = glyph->unk_04;
    xEnd = x - cw * 8;
    param.unk_10 = glyphW;
    param.unk_24 = cl - 1;
    param.unk_14 = glyphH;
    yEnd = y - ch * 8;
    param.unk_1c = font->unk_00->unk_08->unk_06;
    param.unk_20 = cc->unk_0c;
    param.unk_18 = param.unk_1c * font->unk_00->unk_08->unk_00;
    for (; y > yEnd; y -= 8) {
        param.unk_0c = y;
        for (px = x; px > xEnd; px -= 8) {
            param.unk_00 = dst;
            param.unk_08 = px;
            func_02069324(&param);
            dst += charSize;
        }
        dst += rowSkip;
    }
}

/* DrawGlyph1D, adapted from the user-provided NitroSystem g2d_CharCanvas.c.
 * Copyright 2004-2007 Nintendo. All rights reserved. */
void func_020696fc(
    UnkCharCanvas* pCC,
    UnkFont* pFont,
    int x,
    int y,
    int cl,
    UnkGlyph* pGlyph
)
{
    int ofs_x_base;
    int ofs_x;
    int ofs_y;
    int ofs_x_end;
    int ofs_y_end;
    int cx, cx_base;
    int cy;
    u8 glyphWidth;
    u8 charHeight;
    int charSize;
    u16* mapTable;

    charSize = UnkCanvasCharSize(pCC);
    mapTable = (u16*)(pCC->unk_10.unk_00);

    {
        int chara_x_num;
        int chara_y_num;
        const unsigned int areaWidth = (unsigned int)pCC->unk_04;
        const unsigned int areaHeight = (unsigned int)pCC->unk_08;
        const UnkCharWidths* const pWidth = pGlyph->unk_00;

        u32 chara_x_begin;
        u32 chara_x_last;
        u32 chara_y_begin;
        u32 chara_y_last;

        glyphWidth = pWidth->unk_01;
        charHeight = UnkCanvasFontHeight(pFont);

        if( glyphWidth <= 0 )
        {
            return;
        }

        if( (x + glyphWidth < 0) || (y + charHeight) < 0 )
        {
            return;
        }

        chara_x_begin = (x <= 0) ? 0: ((u32)x / 8);
        chara_y_begin = (y <= 0) ? 0: ((u32)y / 8);

        chara_x_last = (u32)(x + glyphWidth + (8 - 1)) / 8;
        if( chara_x_last >= areaWidth )
        {
            chara_x_last = areaWidth;
        }
        chara_y_last = (u32)(y + charHeight + (8 - 1)) / 8;
        if( chara_y_last >= areaHeight )
        {
            chara_y_last = areaHeight;
        }

        chara_x_num = (int)(chara_x_last - chara_x_begin);
        chara_y_num = (int)(chara_y_last - chara_y_begin);

        if( (chara_x_num < 0) || (chara_y_num < 0) )
        {
            return;
        }

        cx_base = (int)chara_x_begin;
        cy      = (int)chara_y_begin;

        ofs_x_base = (x < 0) ? x: x & 0x7;
        ofs_y = (y < 0) ? y: y & 0x7;
        ofs_x_end = ofs_x_base - 8 * chara_x_num;
        ofs_y_end = ofs_y - 8 * chara_y_num;
    }

    {
        UnkGlyphCharParam i;
        u8* const pCharBase = pCC->unk_00;
        UnkCharCanvasLayout p;

        i.unk_04       = pGlyph->unk_04;
        i.unk_10     = glyphWidth;
        i.unk_14    = charHeight;
        i.unk_24        = (u32)(cl - 1);
        i.unk_1c    = UnkCanvasFontBpp(pFont);
        i.unk_20    = pCC->unk_0c;
        i.unk_18      = UnkCanvasFontWidth(pFont) * i.unk_1c;

        p.unk_00 = pCC->unk_10.unk_00;

        {
            const u32 areaWidth         = (u32)pCC->unk_04;
            const u32 areaHeight        = (u32)pCC->unk_08;
            const u32 baseWidthShift    = p.unk_obj.unk_00;
            const u32 baseHeightShift   = p.unk_obj.unk_08;

            for( ; ofs_y > ofs_y_end; ofs_y -= 8 )
            {
                i.unk_0c = ofs_y;
                cx = cx_base;
                for( ofs_x = ofs_x_base; ofs_x > ofs_x_end; ofs_x -= 8 )
                {
                    const unsigned int iChar = func_020690f4((u32)cx, (u32)cy, areaWidth, areaHeight, baseWidthShift, baseHeightShift);

                    i.unk_08 = ofs_x;
                    i.unk_00 = pCharBase + iChar * charSize;

                    func_02069324(&i);
                    cx++;
                }
                cy++;
            }
        }
    }
}

/* Clears a canvas whose characters are contiguous. */
void func_0206990c(UnkCharCanvas* cc, int cl) {
    u32 pat;
    int nChars;

    if (cc->unk_0c == 4) {
        pat = cl | (cl << 4);
        pat |= pat << 8;
        cl = pat | (pat << 16);
    } else {
        pat = cl | (cl << 8);
        cl = pat | (pat << 16);
    }
    nChars = cc->unk_04 * cc->unk_08;
    MI_CpuFill32(cc->unk_00, cl, nChars * CHR_SIZE(cc->unk_0c));
}

/* Clears a BG-style canvas row by row. */
void func_0206995c(UnkCharCanvas* cc, int cl) {
    int i;
    u8* p;
    int charSize;
    int stride;
    int rowSize;
    u32 pat;

    if (cc->unk_0c == 4) {
        pat = cl | (cl << 4);
        pat |= pat << 8;
        cl = pat | (pat << 16);
    } else {
        pat = cl | (cl << 8);
        cl = pat | (pat << 16);
    }
    p = cc->unk_00;
    charSize = CHR_SIZE(cc->unk_0c);
    stride = charSize * cc->unk_10.unk_00;
    rowSize = charSize * cc->unk_04;

    for (i = 0; i < cc->unk_08; i++) {
        MI_CpuFill(cl, p, rowSize);
        p += stride;
    }
}

/* Clears a pixel rectangle of a BG-style canvas. */
void func_020699dc(UnkCharCanvas* cc, int cl, int x, int y, int w, int h) {
    int cx;
    int dh;
    int cy;
    int xEnd;
    int yEnd;
    int cyEnd;
    u32 row;
    int cxStart;
    int charSize;
    int stride;
    int cxEnd;
    u32 chr;
    int bpp;

    bpp = cc->unk_0c;
    xEnd = x + w;
    yEnd = y + h;
    {
        u32 pat;

        if (bpp == 4) {
            pat = cl | (cl << 4);
            pat |= pat << 8;
            cl = pat | (pat << 16);
        } else {
            pat = cl | (cl << 8);
            cl = pat | (pat << 16);
        }
    }
    cxEnd = (xEnd + 7) & ~7;
    cy = y & ~7;
    cyEnd = (yEnd + 7) & ~7;
    cxStart = x & ~7;
    charSize = CHR_SIZE(bpp);
    row = (u32)cc->unk_00 + charSize * (cy / 8 * cc->unk_10.unk_00 + cxStart / 8);
    stride = cc->unk_10.unk_00 * charSize;

    for (; cy < cyEnd; cy += 8) {
        int dy = (cy < y) ? y - cy : 0;

        dh = ((yEnd - cy > 8) ? 8 : yEnd - cy) - dy;
        chr = row;
        for (cx = cxStart; cx < cxEnd; cx += 8) {
            int dx = (cx < x) ? x - cx : 0;
            int dw = ((xEnd - cx > 8) ? 8 : xEnd - cx) - dx;

            func_020691ec((void*)chr, dx, dy, dw, dh, cl, bpp);
            chr += charSize;
        }
        row += stride;
    }
}

#pragma opt_loop_invariants off

/* Clears a pixel rectangle of an OBJ-style canvas. */
void func_02069b78(UnkCharCanvas* cc, int cl, int x, int y, int w, int h) {
    int cx;
    int dh;
    int ccx;
    int cy;
    int xEnd;
    int yEnd;
    int cyEnd;
    u8* charBase;
    int ccxStart;
    int cxStart;
    int hShift;
    int wShift;
    int areaH;
    int areaW;
    int charSize;
    int cxEnd;
    int ccy;
    u8 bpp;
    UnkCharCanvasLayout layout;

    bpp = cc->unk_0c;
    xEnd = x + w;
    yEnd = y + h;
    {
        u32 pat;

        if (bpp == 4) {
            pat = cl | (cl << 4);
            pat |= pat << 8;
            cl = pat | (pat << 16);
        } else {
            pat = cl | (cl << 8);
            cl = pat | (pat << 16);
        }
    }

    cxStart = x & ~7;
    cy = y & ~7;
    cyEnd = (yEnd + 7) & ~7;
    charSize = CHR_SIZE(bpp);
    ccxStart = cxStart / 8;
    ccy = cy / 8;
    cxEnd = (xEnd + 7) & ~7;
    areaW = cc->unk_04;
    areaH = cc->unk_08;
    charBase = cc->unk_00;
    layout = cc->unk_10;
    wShift = layout.unk_obj.unk_00;
    hShift = layout.unk_obj.unk_08;
    for (; cy < cyEnd; ccy++, cy += 8) {
        int dy = (cy < y) ? y - cy : 0;

        dh = ((yEnd - cy > 8) ? 8 : yEnd - cy) - dy;
        ccx = ccxStart;
        for (cx = cxStart; cx < cxEnd; ccx++, cx += 8) {
            int charNo = func_020690f4(ccx, ccy, areaW, areaH, wShift, hShift);
            int dx = (cx < x) ? x - cx : 0;
            int dw = ((xEnd - cx > 8) ? 8 : xEnd - cx) - dx;

            func_020691ec(charBase + charSize * charNo, dx, dy, dw, dh, cl, bpp);
        }
    }
}

#pragma opt_loop_invariants reset

/* canvas functions: BG, OBJ with 1D character mapping, OBJ with 2D character mapping */
static const UnkCharCanvasVTable data_020ba548 = {func_02069564, func_0206990c, func_020699dc};
static const UnkCharCanvasVTable data_020ba53c = {func_02069564, func_0206995c, func_020699dc};
static const UnkCharCanvasVTable data_020ba554 = {func_020696fc, func_0206990c, func_02069b78};

void func_02069d6c(UnkCharCanvas* cc, void* charBase, int areaW, int areaH, int bpp, const UnkCharCanvasVTable* vtable,
                   u32 param) {
    cc->unk_04 = areaW;
    cc->unk_08 = areaH;
    cc->unk_0c = bpp;
    cc->unk_00 = charBase;
    cc->unk_14 = vtable;
    cc->unk_10.unk_00 = param;
}

/* Draws character c at (x, y) with color cl, returns its advance width. */
int func_02069d94(UnkCharCanvas* cc, UnkFont* font, int x, int y, int cl, u16 c) {
    UnkGlyph glyph;
    u16 index = func_02069060(font, c);
    UnkFontGlyph* cglp;

    if (index == 0xffff) {
        index = font->unk_00->unk_02;
    }
    glyph.unk_00 = func_020690a8(font, index);
    glyph.unk_04 = font->unk_00->unk_08->unk_08 + index * font->unk_00->unk_08->unk_02;

    cglp = font->unk_00->unk_08;
    switch (cglp->unk_07) {
    case 0:
    case 7:
        x += glyph.unk_00->unk_00;
        break;
    case 1:
    case 2:
        x -= cglp->unk_00;
        y += glyph.unk_00->unk_00;
        break;
    case 3:
    case 4:
        x -= glyph.unk_00->unk_00 + glyph.unk_00->unk_01;
        y -= cglp->unk_01;
        break;
    case 5:
    case 6:
        y -= glyph.unk_00->unk_00 + cglp->unk_01;
        break;
    }

    cc->unk_14->unk_00(cc, font, x, y, cl, &glyph);
    return glyph.unk_00->unk_02;
}

void func_02069eb8(UnkCharCanvas* cc, void* charBase, int areaW, int areaH, int bpp) {
    func_02069d6c(cc, charBase, areaW, areaH, bpp, &data_020ba548, areaW);
}

/* The OBJ canvas initializers and the OBJ arrange function are not linked in the ROM (no caller);
   the tables they use stay in this unit's .rodata. The 12-byte {-1, -1, -1} initializer at 0x020ba530
   belongs to one of these functions; its real use is unknown (`none` below only reproduces it). */
void unkfunc_unused_20(UnkCharCanvas* cc, void* charBase, int areaW, int areaH, int bpp) {
    func_02069d6c(cc, charBase, areaW, areaH, bpp, &data_020ba53c, 0);
}

void unkfunc_unused_21(UnkCharCanvas* cc, void* charBase, int areaW, int areaH, int bpp) {
    func_02069d6c(cc, charBase, areaW, areaH, bpp, &data_020ba554, 0);
}

void unkfunc_unused_22(u16* attr, int w, int h) {
    const u8* shift = GetObjShiftWH(w, h);
    const int none[3] = {-1, -1, -1};

    attr[0] = data_020ba580[shift[1]][shift[0]][0];
    attr[1] = data_020ba580[shift[1]][shift[0]][1];
    attr[2] = (u16)none[w & 1];
}
