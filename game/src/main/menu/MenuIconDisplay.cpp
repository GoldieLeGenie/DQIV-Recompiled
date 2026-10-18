#pragma ipa file
#include "main/menu/MenuIconDisplay.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/DssUtils.hpp"
#include "nitro/g2.hpp"

static const int s_hopping[39] = {
    0, 0, -2, -2, -3, -3, -3, -3, -2, -2, -1, -1, 0, 0, -1, -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -99,
};

THUMB MenuIconDisplay::MenuIconDisplay()
{
}

THUMB void MenuIconDisplay::setup(int id)
{
    id_ = id;
    reset();
    setType(1);
    for (int i = 0; i < 4; i++) {
        posX_[i] = -1;
        posY_[i] = -1;
        value_[i] = -1;
    }
}

THUMB void MenuIconDisplay::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuIconDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuIconDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    if (!isDrawable(main, sub)) {
        return;
    }
    if (y_ > 0xc0) {
        return;
    }
    if (flag_[0] == 1) {
        drawIcon(main, posX_[0], posY_[0], 0);
    }
    if (flag_[1] == 1) {
        drawIcon(main, posX_[1], posY_[1], 1);
    }
    if (flag_[2] == 1) {
        drawIcon(main, posX_[2], posY_[2], 2);
    }
    if (flag_[3] == 1) {
        drawIcon(main, posX_[3], posY_[3], 3);
    }
    flag_[0] = 0;
    flag_[1] = 0;
    flag_[2] = 0;
    flag_[3] = 0;
}

THUMB void MenuIconDisplay::drawIcon(OamBuffer* oam, int x, int y, int index)
{
    index *= 8;
    G2_SetOBJAttr(oam->allocEntry(), x, y, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32, GX_OAM_COLORMODE_256, index + 0xe0, 0, 0);
    G2_SetOBJAttr(oam->allocEntry(), x, y + 0x20, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_256, index + 0x160, 0, 0);
    G2_SetOBJAttr(oam->allocEntry(), x + 0x20, y, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_256, index + 0x1a0, 0, 0);
    G2_SetOBJAttr(oam->allocEntry(), x + 0x20, y + 0x10, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16, GX_OAM_COLORMODE_256, index + 0x1a4, 0, 0);
    G2_SetOBJAttr(oam->allocEntry(), x + 0x20, y + 0x20, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x8, GX_OAM_COLORMODE_256, index + 0x1e0, 0, 0);
    G2_SetOBJAttr(oam->allocEntry(), x + 0x20, y + 0x28, 2, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x8, GX_OAM_COLORMODE_256, index + 0x1e4, 0, 0);
}

THUMB void MenuIconDisplay::loadIcon(int index, int value)
{
    if (value == -1) {
        flag_[index] = 0;
        return;
    }
    posX_[index] = index << 6;
    posY_[index] = 0x80;
    value_[index] = value;
    flag_[index] = 0;
    void* src = getMenuIconData(0xf5000000, value);
    transferIconTiles(index * 8, 7, src, 0, 0, 8);
    transferIconTiles(index * 8, 8, src, 0, 1, 8);
    transferIconTiles(index * 8, 9, src, 0, 2, 8);
    transferIconTiles(index * 8, 10, src, 0, 3, 8);
    transferIconTiles(index * 8, 11, src, 0, 4, 8);
    transferIconTiles(index * 8, 12, src, 0, 5, 8);
    transferIconTiles(index * 8, 13, src, 8, 0, 4);
    transferIconTiles(index * 8, 14, src, 8, 1, 4);
    transferIconTiles(index * 8 + 4, 13, src, 8, 2, 4);
    transferIconTiles(index * 8 + 4, 14, src, 8, 3, 4);
    transferIconTiles(index * 8, 15, src, 8, 4, 4);
    transferIconTiles(index * 8 + 4, 15, src, 8, 5, 4);
    getObjPaletteBuffer(1)->loadColors(0, 0, getMenuIconData(0xf5000000, 9999), 0xa0);
}

THUMB void MenuIconDisplay::setIconPosition(int index, int x, int y)
{
    posX_[index] = x;
    posY_[index] = y;
}

THUMB void MenuIconDisplay::setIconVisible(int index, int flag)
{
    flag_[index] = flag;
}

THUMB void MenuIconDisplay::transferIconTiles(int x, int y, void* src, int srcX, int srcY, int width)
{
    requestVramCopy(0x23, (x + y * 32) * 32, (char*)src + (srcX + srcY * 12) * 32, width * 32);
}

THUMB void HoppingDigit::clear()
{
    frame_ = -99;
}

THUMB void HoppingDigit::start(int x, int y, int number, int delay)
{
    frame_ = 0;
    number_ = number;
    x_ = x;
    y_ = y;
    drawY_ = 0;
    frame_ = (signed char)-delay;
}

THUMB void HoppingDigit::update(OamBuffer* oam)
{
    if (frame_ == -99) {
        return;
    }
    frame_++;
    if (frame_ < 0) {
        return;
    }
    drawY_ = y_ + s_hopping[frame_];
    if (s_hopping[frame_] == -99) {
        frame_ = -99;
    }
}

THUMB void HoppingDigit::draw(OamBuffer* oam)
{
    int name;
    if (frame_ < 0) {
        return;
    }
    switch (number_) {
        case 0:
            name = 0x110;
            break;
        case 1:
            name = 0x111;
            break;
        case 2:
            name = 0x112;
            break;
        case 3:
            name = 0x113;
            break;
        case 4:
            name = 0x130;
            break;
        case 5:
            name = 0x131;
            break;
        case 6:
            name = 0x132;
            break;
        case 7:
            name = 0x133;
            break;
        case 8:
            name = 0x150;
            break;
        case 9:
            name = 0x151;
            break;
        default:
            return;
    }
    G2_SetOBJAttr(oam->allocEntry(), x_, drawY_, 0, GX_OAM_MODE_NORMAL, FALSE, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLORMODE_16, name, 15, 0);
}

THUMB int HoppingDigit::isFree()
{
    if (frame_ == -99) {
        return 1;
    }
    return 0;
}

THUMB HoppingNumberDisplay::HoppingNumberDisplay()
{
}

THUMB void HoppingNumberDisplay::setup(int id)
{
    id_ = id;
    setType(2);
    for (int i = 0; i < 40; i++) {
        digit_[i].clear();
    }
}

THUMB void HoppingNumberDisplay::update(OamBuffer* main, OamBuffer* sub)
{
    for (int i = 0; i < 40; i++) {
        digit_[i].update(sub);
    }
}

THUMB void HoppingNumberDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void HoppingNumberDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    setEnable(1);
    if (isDrawable(main, sub)) {
        for (int i = 0; i < 40; i++) {
            digit_[i].draw(sub);
        }
    }
}

THUMB HoppingDigit* HoppingNumberDisplay::getFreeDigit()
{
    for (int i = 0; i < 40; i++) {
        if (digit_[i].isFree()) {
            return &digit_[i];
        }
    }
    return 0;
}

THUMB void HoppingNumberDisplay::startNumber(int x, int y, int value, int delay)
{
    char buf[8];
    delay++;
    dss::sprintf_s(buf, 8, "%d", value);
    if (value < 10) {
        x -= 4;
    } else if (value < 100) {
        x -= 8;
    } else {
        x -= 12;
    }
    for (char* p = buf; *p != 0; p++) {
        HoppingDigit* digit = getFreeDigit();
        if (digit != 0) {
            digit->start(x, y - 8, *p - '0', delay);
            x += 8;
            delay += 4;
        }
    }
}
