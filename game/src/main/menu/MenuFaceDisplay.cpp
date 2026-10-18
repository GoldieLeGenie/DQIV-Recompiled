#pragma ipa file
#include "main/menu/MenuFaceDisplay.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/MenuIconDisplay.hpp"
#include "nitro/g2.hpp"

// Loading animation, two frames per entry: frame part shown and offset of the moving part (part 8)
static char s_part[] = {0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, -1};
static char s_moveX[] = {3, 3, 4, 5, 4, 3, 3, 4, 5, 6, 6, 7, 8, 9, 10, 12, 11, 10, 9, 10, 11, 12, 13, 13, 13, 12, 11, 10, 9, 7, 6, 4, -1};
static char s_moveY[] = {7, 7, 7, 7, 6, 5, 4, 4, 4, 4, 5, 6, 7, 7, 7, 7, 6, 5, 5, 5, 5, 5, 6, 6, 7, 9, 11, 12, 12, 11, 10, 8, -1};

THUMB MenuFaceDisplay::MenuFaceDisplay()
{
}

THUMB void MenuFaceDisplay::setup(int id)
{
    id_ = id;
    mode_ = 1;
    reset();
    setType(2);
}

THUMB void MenuFaceDisplay::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuFaceDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuFaceDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    if (mode_ != 1) {
        enable_ = 1;
    }
    if (!isDrawable(main, sub) || y_ < 192) {
        return;
    }
    int y = y_ % 192;
    if (mode_ == 1) {
        GXOamAttr* obj = sub->allocEntry();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32,
                      GX_OAM_COLORMODE_256, 0x114, 0, 0);
        obj = sub->allocEntry();
        G2_SetOBJAttr(obj, x_ + 32, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x32,
                      GX_OAM_COLORMODE_256, 0x11c, 0, 0);
        y += 32;
        obj = sub->allocEntry();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x16,
                      GX_OAM_COLORMODE_256, 0x194, 0, 0);
        obj = sub->allocEntry();
        G2_SetOBJAttr(obj, x_ + 32, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16,
                      GX_OAM_COLORMODE_256, 0x19c, 0, 0);
        return;
    }
    int chr = 0;
    if (mode_ == 2) {
        chr = 0x114;
    }
    if (mode_ == 3) {
        chr = 0x118;
    }
    if (mode_ == 4) {
        chr = 0x11c;
    }
    if (mode_ == 5) {
        chr = 0x154;
    }
    if (mode_ == 6) {
        chr = 0x158;
    }
    if (mode_ == 7) {
        chr = 0x15c;
    }
    if (mode_ == 8) {
        chr = 0x194;
    }
    if (mode_ == 9) {
        chr = 0x198;
    }
    if (mode_ == 10) {
        chr = 0x19c;
    }
    if (chr) {
        GXOamAttr* obj = sub->allocEntry();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16,
                      GX_OAM_COLORMODE_256, chr, 0, 0);
    }
    if (mode_ == 11) {
        int frame = animFrame_ / 2;
        int part = s_part[frame];
        drawPart(sub, s_moveX[frame], -s_moveY[frame], 8);
        drawPart(sub, 0, 0, part);
        animFrame_++;
        if (s_part[animFrame_ / 2] == -1) {
            animFrame_ = 0;
        }
    }
}

THUMB void MenuFaceDisplay::drawPart(OamBuffer* oam, int x, int y, int part)
{
    int chr;
    x += x_;
    y += y_ % 192;
    switch (part) {
    case 0:
        chr = 0x114;
        break;
    case 1:
        chr = 0x118;
        break;
    case 2:
        chr = 0x11c;
        break;
    case 3:
        chr = 0x154;
        break;
    case 4:
        chr = 0x158;
        break;
    case 5:
        chr = 0x15c;
        break;
    case 6:
        chr = 0x194;
        break;
    case 7:
        chr = 0x198;
        break;
    case 8:
        chr = 0x19c;
        break;
    default:
        return;
    }
    GXOamAttr* obj = oam->allocEntry();
    G2_SetOBJAttr(obj, x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16,
                  GX_OAM_COLORMODE_256, chr, 0, 0);
}

THUMB void MenuFaceDisplay::setPriority(int priority)
{
    priority_ = priority;
}

THUMB void MenuFaceDisplay::loadFace(int index)
{
    loadIcon(0xf5000000, index);
    mode_ = 1;
}

THUMB void MenuFaceDisplay::loadLoadingAnim()
{
    loadIcon(0xf9000000, 1);
    mode_ = 11;
}

THUMB void MenuFaceDisplay::resetAnim()
{
    animFrame_ = 0;
}

THUMB void MenuFaceDisplay::loadIcon(int type, int index)
{
    if (index == -1) {
        setEnable(0);
        return;
    }
    if (isKey(index)) {
        return;
    }
    setKey(index);
    char* src = (char*)getMenuIconData(type, index);
    int dst = 0x2280;
    for (int i = 0; i < 6; i++) {
        requestVramCopy(0x13, dst, src, 0x180);
        dst += 0x400;
        src += 0x180;
    }
    getObjPaletteBuffer(0)->loadColors(0, 0, getMenuIconData(type, 9999), 0xa0);
    priority_ = 1;
}
