#include "main/menu/MenuWindowFrame.hpp"
#include "main/menu/MenuDisplays.hpp"
#include "nitro/g2.hpp"

THUMB MenuWindowFrame::MenuWindowFrame()
{
}

THUMB void MenuWindowFrame::setup(int id)
{
    id_ = id;
    cursor_ = 0;
    arrow_ = 0;
    unk_38 = 0;
    arrowX_ = 256;
    arrowY_ = 192;
    setType(2);
}

THUMB void MenuWindowFrame::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuWindowFrame::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuWindowFrame::draw(OamBuffer* main, OamBuffer* sub)
{
    int blink;
    if (blink_ & 0x20) {
        blink = 0;
    } else {
        blink = 1;
    }
    blink_++;
    if (isDrawable(main, sub)) {
        int x = x_;
        int y = y_ - 192;
        int bottom = 32 - (64 - h_);
        if (y < 0 || y >= 192) {
            return;
        }
        if (cursor_ && blink) {
            G2_SetOBJAttr(sub->allocEntry(), x + w_ / 2 - 8, y + bottom + 16, 0, GX_OAM_MODE_NORMAL, 0,
                          GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, 0x192, 15, 0);
        }
        if (arrow_) {
            int chr = g_MenuDisplays.arrow_.getAnimCharName();
            G2_SetOBJAttr(sub->allocEntry(), arrowX_ - 8, arrowY_ - 8, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                          GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, chr, 15, 0);
        }
        if (unk_38) {
            G2_SetOBJAttr(sub->allocEntry(), x_ + w_ - 24, y + bottom + 8, 0, GX_OAM_MODE_NORMAL, 0,
                          GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_16, 0x152, 15, 0);
        }
        MenuFrameDisplay* frames = &g_MenuDisplays.frames_;
        if (frameType_ == 1) {
            frames->addFrame(x_ + 4, y_ + 4, w_ - 8, h_ - 8, 0, 15);
        } else {
            frames->addOpaqueFrame(x_ + 4, y_ + 4, w_ - 8, h_ - 8, 0, 15);
        }
        if (nameWidth_ <= 16) {
            drawBorder(sub, x, y, w_, h_);
            return;
        }
        drawBorderWithTab(sub, x, y, w_, h_, nameWidth_, 16);
        if (frameType_ == 1) {
            frames->addFrame(x_ + 4, y_ - 12, nameWidth_ - 6, 16, 0, 15);
        } else {
            frames->addOpaqueFrame(x_ + 4, y_ - 12, nameWidth_ - 6, 16, 0, 15);
        }
    }
}

THUMB void MenuWindowFrame::setNameWidth(int width)
{
    if (width > 224) {
        width = 224;
    }
    nameWidth_ = width;
}

THUMB void MenuWindowFrame::setCursor(int cursor)
{
    if (cursor_ == 0 && cursor == 1) {
        blink_ = 0;
    }
    cursor_ = cursor;
}

THUMB void MenuWindowFrame::setArrow(int x, int y, int arrow)
{
    arrowX_ = x;
    arrowY_ = y;
    arrow_ = arrow;
}

THUMB void MenuWindowFrame::setFrameType(int type)
{
    frameType_ = type;
}

THUMB void MenuWindowFrame::drawBorder(OamBuffer* oam, int x, int y, int w, int h)
{
    drawSprite(oam, x, y, 0x109, 32);
    drawSprite(oam, x + w - 32, y, 0x10b, 32);
    drawSprite(oam, x, y + h - 32, 0x149, 32);
    drawSprite(oam, x + w - 32, y + h - 32, 0x14b, 32);
    drawTopEdge(oam, x + 32, y, w - 64);
    drawBottomEdge(oam, x + 32, y + h - 8, w - 64);
    drawLeftEdge(oam, x, y + 32, h - 64);
    drawRightEdge(oam, x + w - 8, y + 32, h - 64);
}

THUMB void MenuWindowFrame::drawBorderWithTab(OamBuffer* oam, int x, int y, int w, int h, int tabW, int tabH)
{
    drawSprite(oam, x, y - tabH, 0x109, 16);
    drawSprite(oam, x + w - 32, y, 0x10b, 32);
    drawSprite(oam, x, y + h - 32, 0x149, 32);
    drawSprite(oam, x + w - 32, y + h - 32, 0x14b, 32);
    drawBottomEdge(oam, x + 32, y + h - 8, w - 64);
    drawLeftEdge(oam, x, y + 16 - tabH, h - 32);
    drawRightEdge(oam, x + w - 8, y + 32, h - 64);
    int length = tabW - 32;
    drawTopEdge(oam, x + 16, y - tabH, length);
    drawSprite(oam, x + 16 + length, y - tabH, 0x10d, 16);
    drawSprite(oam, x + 24 + length, y, 0x1af, 8);
    drawTopEdge(oam, x + tabW, y, w - tabW - 32);
}

THUMB void MenuWindowFrame::drawTopEdge(OamBuffer* oam, int x, int y, int length)
{
    if (length <= 0) {
        return;
    }
    if (length > 32) {
        while (length > 32) {
            drawSprite(oam, x, y, 0x10a, 32);
            length -= 32;
            x += 32;
        }
        if (length != 0) {
            drawSprite(oam, x - (32 - length), y, 0x10a, 32);
        }
    } else if (length > 16) {
        while (length > 16) {
            drawSprite(oam, x, y, 0x10b, 16);
            length -= 16;
            x += 16;
        }
        if (length != 0) {
            drawSprite(oam, x - (16 - length), y, 0x10b, 16);
        }
    } else {
        if (length < 8) {
            length = 8;
        }
        while (length > 8) {
            drawSprite(oam, x, y, 0x10b, 8);
            length -= 8;
            x += 8;
        }
        if (length != 0) {
            drawSprite(oam, x - (8 - length), y, 0x10b, 8);
        }
    }
}

THUMB void MenuWindowFrame::drawBottomEdge(OamBuffer* oam, int x, int y, int length)
{
    if (length <= 0) {
        return;
    }
    if (length > 32) {
        while (length > 32) {
            drawSprite(oam, x, y - 24, 0x14a, 32);
            length -= 32;
            x += 32;
        }
        if (length != 0) {
            drawSprite(oam, x - (32 - length), y - 24, 0x14a, 32);
        }
    } else if (length > 16) {
        while (length > 16) {
            drawSprite(oam, x, y - 8, 0x18b, 16);
            length -= 16;
            x += 16;
        }
        if (length != 0) {
            drawSprite(oam, x - (16 - length), y - 8, 0x18b, 16);
        }
    } else {
        if (length < 8) {
            length = 8;
        }
        while (length > 8) {
            drawSprite(oam, x, y, 0x1ab, 8);
            length -= 8;
            x += 8;
        }
        if (length != 0) {
            drawSprite(oam, x - (8 - length), y, 0x1ab, 8);
        }
    }
}

THUMB void MenuWindowFrame::drawLeftEdge(OamBuffer* oam, int x, int y, int length)
{
    if (length <= 0) {
        return;
    }
    if (length > 32) {
        while (length > 32) {
            drawSprite(oam, x, y, 0x129, 32);
            length -= 32;
            y += 32;
        }
        if (length != 0) {
            drawSprite(oam, x, y - (32 - length), 0x129, 32);
        }
    } else if (length > 16) {
        while (length > 16) {
            drawSprite(oam, x, y, 0x149, 16);
            length -= 16;
            y += 16;
        }
        if (length != 0) {
            drawSprite(oam, x, y - (16 - length), 0x149, 16);
        }
    } else {
        if (length < 8) {
            length = 8;
        }
        while (length > 8) {
            drawSprite(oam, x, y, 0x149, 8);
            length -= 8;
            y += 8;
        }
        if (length != 0) {
            drawSprite(oam, x, y - (8 - length), 0x149, 8);
        }
    }
}

THUMB void MenuWindowFrame::drawRightEdge(OamBuffer* oam, int x, int y, int length)
{
    if (length <= 0) {
        return;
    }
    if (length > 32) {
        while (length > 32) {
            drawSprite(oam, x - 24, y, 0x12b, 32);
            length -= 32;
            y += 32;
        }
        if (length != 0) {
            drawSprite(oam, x - 24, y - (32 - length), 0x12b, 32);
        }
    } else if (length > 16) {
        while (length > 16) {
            drawSprite(oam, x - 8, y, 0x14d, 16);
            length -= 16;
            y += 16;
        }
        if (length != 0) {
            drawSprite(oam, x - 8, y - (16 - length), 0x14d, 16);
        }
    } else {
        if (length < 8) {
            length = 8;
        }
        while (length > 8) {
            drawSprite(oam, x, y, 0x14e, 8);
            length -= 8;
            y += 8;
        }
        if (length != 0) {
            drawSprite(oam, x, y - (8 - length), 0x14e, 8);
        }
    }
}

THUMB void MenuWindowFrame::drawSprite(OamBuffer* oam, int x, int y, int chr, int size)
{
    int shape = 0;
    if (size == 8) {
        shape = GX_OAM_SHAPE_8x8;
    }
    if (size == 16) {
        shape = GX_OAM_SHAPE_16x16;
    }
    if (size == 32) {
        shape = GX_OAM_SHAPE_32x32;
    }
    GXOamAttr* obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, shape, GX_OAM_COLORMODE_16, chr, 15, 0);
}
