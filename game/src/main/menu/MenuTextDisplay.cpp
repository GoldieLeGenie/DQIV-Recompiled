#include "main/menu/MenuTextDisplay.hpp"
#include "main/menu/MenuIconDisplay.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/g2.hpp"

THUMB MenuTextDisplay::MenuTextDisplay()
{
}

THUMB void MenuTextDisplay::setup(int id)
{
    id_ = id;
    initCharBuffer(&char_, charData_, 32, 2);
    enable_ = 0;
    xlu_ = 1;
    timer_ = 0;
    setType(2);
}

THUMB void MenuTextDisplay::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuTextDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuTextDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    int baseY;
    int i;
    int left;
    int top;
    int y;
    int chr;
    int x;
    int width;
    int right;
    int pos;
    int back;
    if (timer_ > 0) {
        timer_--;
        return;
    }
    if (!isDrawable(main, sub)) {
        return;
    }
    y = y_ % 192;
    chr = id_;
    x = 0;
    for (i = 0; i < 8; i++, x += 32, chr += 4) {
        if (x < visibleWidth_) {
            if (visibleWidth_ < width_ && x + 32 > visibleWidth_) {
                drawHiddenMask(sub, x_ + visibleWidth_, y, x + 32 - visibleWidth_, 14);
            }
#ifdef DQIV_NATIVE
            G2_SetOBJAttr(sub->allocEntry(), x_ + x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                          GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, chr, frame_ && letterPalette_ >= 0 ? letterPalette_ : palette_, 0);
#else
            G2_SetOBJAttr(sub->allocEntry(), x_ + x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                          GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, chr, palette_, 0);
#endif
        }
    }
    if (frame_) {
        left = x_ - 2;
        baseY = y_;
        top = baseY - 2;
        width = frameWidth_ + 4;
        if (width < 32) {
            drawFrameSprite16(sub, left, top, 0x109);
            drawFrameSprite16(sub, left, baseY + 2, 0x189);
            right = left + width - 16;
            drawFrameSprite16(sub, right, top, 0x10d);
            drawFrameSprite16(sub, right, baseY + 2, 0x18d);
        } else {
            drawFrameSprite32(sub, left, top, 0x109);
            drawFrameSprite32(sub, left, baseY + 2, 0x189);
            right = left + width - 32;
            drawFrameSprite32(sub, right, top, 0x10b);
            drawFrameSprite32(sub, right, baseY + 2, 0x18b);
        }
        width -= 64;
        left += 32;
        for (pos = 0; pos < width; pos += 32) {
        if (pos + 32 > width) {
            back = pos + 32 - width;
            if (back > 16) {
                back = 16;
            }
        } else {
            back = 0;
        }
        drawFrameSprite32(sub, left + pos - back, top, 0x10a);
        drawFrameSprite32(sub, left + pos - back, baseY + 2, 0x18a);
        }
        for (pos = 0; pos < frameWidth_; pos += 16) {
            back = 0;
            if (pos + 16 >= frameWidth_) {
                back = pos + 16 - frameWidth_;
            }
            GXOamAttr* oam = sub->allocEntry();
            G2_SetOBJAttr(oam, x_ + pos - back, y_, priority_, GX_OAM_MODE_XLU, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x8,
                          GX_OAM_COLORMODE_16, 0x101, 15, 0);
            oam = sub->allocEntry();
            G2_SetOBJAttr(oam, x_ + pos - back, y_ + 6, priority_, GX_OAM_MODE_XLU, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x8,
                          GX_OAM_COLORMODE_16, 0x101, 15, 0);
        }
        frame_ = 0;
    }
}

THUMB void MenuTextDisplay::drawHiddenMask(OamBuffer* oam, int x, int y, int width, int height)
{
    int y2 = y + height - 8;
    int size;
    int chr;
    int shape;
    if (width > 16) {
        size = 16;
        chr = size + 0xf1;
        shape = size << 10;
    } else if (width > 8) {
        size = 8;
        chr = size + 0xf9;
        shape = 0;
    } else if (width > 4) {
        size = 4;
        chr = 0x10f;
        shape = 0;
    } else if (width > 2) {
        size = 2;
        chr = 0x12f;
        shape = 0;
    } else if (width >= 1) {
        chr = 0x14f;
        size = 1;
        shape = 0;
    } else {
        return;
    }
    int mode = xlu_ ? 1 : 0;
    GXOamAttr* obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y, 0, mode, 0, GX_OAM_EFFECT_NONE, shape, GX_OAM_COLORMODE_16, chr, 15, 0);
    obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y2, 0, mode, 0, GX_OAM_EFFECT_NONE, shape, GX_OAM_COLORMODE_16, chr, 15, 0);
    obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x + size - (size * 2 - width), y, 0, mode, 0, GX_OAM_EFFECT_NONE, shape, GX_OAM_COLORMODE_16, chr, 15, 0);
    obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x + size - (size * 2 - width), y2, 0, mode, 0, GX_OAM_EFFECT_NONE, shape, GX_OAM_COLORMODE_16, chr, 15, 0);
}

THUMB void MenuTextDisplay::setText(int font, const char* text, int x, int y, int clear)
{
    setBgTextFontSize(font);
    fillCharBuffer(&char_, 0);
    drawBgText(&char_, x, y, 1, text);
    count_ = getBgTextCharWidths(charWidths_, 0x100);
    width_ = getBgTextWidth();
    visibleCount_ = 0;
    visibleWidth_ = 0;
    setVisibleCount(0);
    requestVramCopy(0x13, id_ << 5, charData_, 0x800);
    palette_ = 15;
#ifdef DQIV_NATIVE
    letterPalette_ = -1;
#endif
    frame_ = 0;
    frameWidth_ = 0;
    unk_95c = 0;
    priority_ = 0;
    timer_ = 1;
    if (clear) {
        for (int i = 0; i < width_ + 1; i++) {
            setCharBufferPixel(&char_, i, 15, 1);
        }
    }
}

THUMB int MenuTextDisplay::setFramedText(int font, const char* text, int priority, int flag)
{
    setBgTextFontSize(10);
    drawBgText(NULL, 0, 0, 0, text);
    int width = getBgTextWidth() + 8;
    if (width & 7) {
        width += 8;
    }
    width = width / 8 * 8;
    setText(font, text, 4, 2, 0);
    palette_ = flag ? 15 : 14;
    frame_ = 1;
    frameWidth_ = width;
    unk_95c = 14;
    priority_ = priority;
    setVisibleWidth(width);
    timer_ = 1;
    return width + 4;
}

THUMB void MenuTextDisplay::setVisibleCount(int count)
{
    if (count == -1) {
        count = count_;
    }
    if (count > count_) {
        count = count_;
    }
    visibleCount_ = count;
    int width = 0;
    for (int i = 0; i < count; i++) {
        width += charWidths_[i];
    }
    visibleWidth_ = width;
}

THUMB void MenuTextDisplay::setVisibleWidth(int width)
{
    visibleWidth_ = width;
}

THUMB int MenuTextDisplay::getVisibleCount()
{
    return visibleCount_;
}

THUMB int MenuTextDisplay::getCharCount()
{
    return count_;
}

THUMB int MenuTextDisplay::getTextWidth()
{
    return width_;
}

THUMB void MenuTextDisplay::clearScrolledLines(int scroll)
{
    int i;
    char* dst;
    int row = 0;
    if (scroll > 0) {
        int size = scroll * 4;
        do {
            dst = (char*)getCharBufferTile(&char_, 0, row);
            if (scroll > 8) {
                MI_CpuFill(0, dst, 0x400);
            } else {
                for (i = 0; i < 32; i++) {
                    MI_CpuFill(0, dst, size);
                    dst += 0x20;
                }
            }
            size -= 0x20;
            scroll -= 8;
            row++;
        } while (scroll > 0);
    }
    requestVramCopy(0x13, id_ << 5, charData_, 0x800);
}

THUMB void MenuTextDisplay::setTranslucent(int flag)
{
    xlu_ = flag;
}

THUMB void MenuTextDisplay::drawFrameSprite32(OamBuffer* oam, int x, int y, int chr)
{
    GXOamAttr* obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16,
                  chr, palette_, 0);
}

THUMB void MenuTextDisplay::drawFrameSprite16(OamBuffer* oam, int x, int y, int chr)
{
    GXOamAttr* obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16,
                  chr, palette_, 0);
}
