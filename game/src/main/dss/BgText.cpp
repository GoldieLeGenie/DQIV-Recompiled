#include "main/dss/BgText.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Utf8Iterator.hpp"

BgTextEngine g_BgText;

ARM void initBgText()
{
    g_BgText.count_ = 0;
    g_BgText.font_ = NULL;
    g_BgText.subFont_ = NULL;
    g_BgText.mode_ = 2;
}

ARM void loadBgFont12(void* data, void* subData)
{
    g_BgText.font12_.load(data, 12);
    g_BgText.subFont12_.load(subData, 12);
}

ARM void loadBgFont10(void* data, void* subData)
{
    g_BgText.font10_.load(data, 10);
    g_BgText.subFont10_.load(subData, 10);
}

ARM int drawBgText(BgCharBuffer* buffer, int x, int y, int color, const char* text)
{
    return drawBgTextWrapped(buffer, x, y, color, text, 0);
}

ARM int drawBgTextWrapped(BgCharBuffer* buffer, int x, int y, int color, const char* text, int wrap)
{
    BgTextEngine* work = &g_BgText;
    G2dCharCanvas canvas;
    G2dCharCanvas* pCanvas = NULL;
    work->count_ = 0;
    if (buffer != NULL) {
        void* chr = getCharBufferTile(buffer, x / 8, y / 8);
        x %= 8;
        y %= 8;
        int areaWidth = buffer->pitch_;
        int areaHeight = buffer->height_;
        pCanvas = &canvas;
        if (areaHeight > 24) {
            areaHeight = 24;
        }
        func_02069eb8(&canvas, chr, areaWidth, areaHeight, 4);
    }
    int lineWidth = 0;
    int left = x;
    int maxWidth = 0;
    int height = 0;
    int i = 0;
    int lineHeight = work->lineHeight_ + 2;
    Utf8Iterator it;
    it.setText(text);
    while (i < 0x100) {
        unsigned short c = it.getChar();
        it.nextChar();
        if (c == 0xffff || c == 0) {
            break;
        }
        int w;
        switch (c) {
        case 0xfeff:
        case 0xfffe:
            w = 0;
            break;
        case '\t':
            w = 13;
            break;
        case '\n':
            if (maxWidth < lineWidth) {
                maxWidth = lineWidth;
            }
            w = 0;
            lineWidth = 0;
            height += lineHeight;
            x = left;
            y += lineHeight;
            break;
        case ';':
            if (wrap) {
                if (maxWidth < lineWidth) {
                    maxWidth = lineWidth;
                }
                w = 0;
                lineWidth = 0;
                height += lineHeight;
                x = left;
                y += lineHeight;
            } else {
                w = 0;
            }
            break;
        case '$':
            if (wrap) {
                lineWidth += drawBgChar(pCanvas, x, y, color, '-');
                if (maxWidth < lineWidth) {
                    maxWidth = lineWidth;
                }
                w = 0;
                lineWidth = 0;
                height += lineHeight;
                x = left;
                y += lineHeight;
            } else {
                w = 0;
            }
            break;
        case '\r':
            w = 0;
            break;
        default:
            w = drawBgChar(NULL, x, y, color, c);
            if (w != 0) {
                if (x + w < 0x100) {
                    w = drawBgChar(pCanvas, x, y, color, c);
                }
            } else {
                w = lineHeight;
            }
            break;
        }
        x += w;
        lineWidth += w;
        work->charWidths_[i++] = w;
    }
    if (maxWidth < lineWidth) {
        maxWidth = lineWidth;
    }
    work->width_ = maxWidth;
    work->height_ = height;
    work->count_ = i;
    return 0;
}

ARM int drawBgChar(G2dCharCanvas* canvas, int x, int y, int color, unsigned short c)
{
    BgTextEngine* work = &g_BgText;
    DssFont* font = work->font_;
    if (work->mode_ == 1) {
        switch (c) {
        case 0xff10:
            c = 0x249c;
            break;
        case 0xff11:
            c = 0x249d;
            break;
        case 0xff12:
            c = 0x249e;
            break;
        case 0xff13:
            c = 0x249f;
            break;
        case 0xff14:
            c = 0x24a0;
            break;
        case 0xff15:
            c = 0x24a1;
            break;
        case 0xff16:
            c = 0x24a2;
            break;
        case 0xff17:
            c = 0x24a3;
            break;
        case 0xff18:
            c = 0x24a4;
            break;
        case 0xff19:
            c = 0x24a5;
            break;
        case 0xff0f:
            c = 0x24a7;
            break;
        case 0xff1a:
            c = 0x24a8;
            break;
        case 0xff01:
            c = 0x24b0;
            break;
        case 0x3000:
            c = 0x24a6;
            break;
        }
    }
    if (work->mode_ == 2) {
        switch (c) {
        case 0xff10:
            c = 0x322a;
            break;
        case 0xff11:
            c = 0x322b;
            break;
        case 0xff12:
            c = 0x322c;
            break;
        case 0xff13:
            c = 0x322d;
            break;
        case 0xff14:
            c = 0x322e;
            break;
        case 0xff15:
            c = 0x322f;
            break;
        case 0xff16:
            c = 0x3230;
            break;
        case 0xff17:
            c = 0x3231;
            break;
        case 0xff18:
            c = 0x3232;
            break;
        case 0xff19:
            c = 0x3233;
            break;
        case 0xff0f:
            c = 0x3235;
            break;
        case 0xff1a:
            c = 0x3236;
            break;
        case 0xff01:
            c = 0x323e;
            break;
        case 0x3000:
            c = 0x3234;
            break;
        }
    }
    switch (c) {
    case 0x80:
        c = 0x24d5;
        break;
    case 0x81:
        c = 0x24d6;
        break;
    case 0x82:
        c = 0x24d7;
        break;
    case 0x83:
        c = 0x24d8;
        break;
    case 0x84:
        c = 0x24d9;
        break;
    case 0x85:
        c = 0x24c6;
        break;
    }
    if (c == 0x3000) {
        if (work->size_ == 12) {
            return 10;
        }
        if (work->size_ == 10) {
            return 8;
        }
    }
    if (c == ' ') {
        if (work->size_ == 12) {
            return 4;
        }
        if (work->size_ == 10) {
            return 3;
        }
    }
    if ((c & 0xff00) == 0) {
        font = work->subFont_;
    }
    if (c == 0xd7) {
        font = work->font_;
    }
    if (c == 0xf7) {
        font = work->font_;
    }
    if ((c & 0xff00) == 0x100) {
        font = work->subFont_;
    }
    if ((c & 0xff00) == 0x400) {
        font = work->subFont_;
    }
    if ((c & 0xff00) == 0x2400) {
        font = work->subFont_;
    }
    if ((c & 0xff00) == 0x3200) {
        font = work->subFont_;
    }
    return font->drawChar(canvas, x, y, color, c);
}

ARM void setBgTextFontSize(int size)
{
    BgTextEngine* work = &g_BgText;
    if (size == 12) {
        work->font_ = &work->font12_;
        work->subFont_ = &work->subFont12_;
        work->lineHeight_ = getBgTextLineHeight(12);
    }
    if (size == 10) {
        work->font_ = &work->font10_;
        work->subFont_ = &work->subFont10_;
        work->lineHeight_ = getBgTextLineHeight(10);
    }
    work->size_ = size;
}

ARM int getBgTextLineHeight(int size)
{
    switch (size) {
    case 12:
        return 12;
    case 10:
        return 10;
    }
    return 0;
}

ARM int getBgTextCharWidths(unsigned char* widths, int max)
{
    if (max < g_BgText.count_) {
        return -1;
    }
    if (max > g_BgText.count_) {
        max = g_BgText.count_;
    }
    MI_CpuCopyU8(g_BgText.charWidths_, widths, max);
    return max;
}

ARM int getBgTextWidth()
{
    return g_BgText.width_;
}
