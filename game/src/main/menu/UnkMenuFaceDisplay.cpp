#pragma ipa file
#include "main/menu/UnkMenuFaceDisplay.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "nitro/g2.hpp"

// Loading animation, two frames per entry: frame part shown and offset of the moving part (part 8)
static char s_part[] = {0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, -1};
static char s_moveX[] = {3, 3, 4, 5, 4, 3, 3, 4, 5, 6, 6, 7, 8, 9, 10, 12, 11, 10, 9, 10, 11, 12, 13, 13, 13, 12, 11, 10, 9, 7, 6, 4, -1};
static char s_moveY[] = {7, 7, 7, 7, 6, 5, 4, 4, 4, 4, 5, 6, 7, 7, 7, 7, 6, 5, 5, 5, 5, 5, 6, 6, 7, 9, 11, 12, 12, 11, 10, 8, -1};

THUMB UnkMenuFaceDisplay::UnkMenuFaceDisplay()
{
}

THUMB void UnkMenuFaceDisplay::setup(int id)
{
    id_ = id;
    mode_ = 1;
    unkfunc_0204f1ac();
    unkfunc_0204f260(2);
}

THUMB void UnkMenuFaceDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuFaceDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuFaceDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (mode_ != 1) {
        enable_ = 1;
    }
    if (!unkfunc_0204f214(main, sub) || y_ < 192) {
        return;
    }
    int y = y_ % 192;
    if (mode_ == 1) {
        GXOamAttr* obj = sub->unkfunc_02082694();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32,
                      GX_OAM_COLORMODE_256, 0x114, 0, 0);
        obj = sub->unkfunc_02082694();
        G2_SetOBJAttr(obj, x_ + 32, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x32,
                      GX_OAM_COLORMODE_256, 0x11c, 0, 0);
        y += 32;
        obj = sub->unkfunc_02082694();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x16,
                      GX_OAM_COLORMODE_256, 0x194, 0, 0);
        obj = sub->unkfunc_02082694();
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
        GXOamAttr* obj = sub->unkfunc_02082694();
        G2_SetOBJAttr(obj, x_, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16,
                      GX_OAM_COLORMODE_256, chr, 0, 0);
    }
    if (mode_ == 11) {
        int frame = animFrame_ / 2;
        int part = s_part[frame];
        unkfunc_0204f4a4(sub, s_moveX[frame], -s_moveY[frame], 8);
        unkfunc_0204f4a4(sub, 0, 0, part);
        animFrame_++;
        if (s_part[animFrame_ / 2] == -1) {
            animFrame_ = 0;
        }
    }
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f4a4(UnkOamBuffer* oam, int x, int y, int part)
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
    GXOamAttr* obj = oam->unkfunc_02082694();
    G2_SetOBJAttr(obj, x, y, priority_, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_16x16,
                  GX_OAM_COLORMODE_256, chr, 0, 0);
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f53c(int priority)
{
    priority_ = priority;
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f540(int index)
{
    unkfunc_0204f570(0xf5000000, index);
    mode_ = 1;
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f554()
{
    unkfunc_0204f570(0xf9000000, 1);
    mode_ = 11;
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f568()
{
    animFrame_ = 0;
}

THUMB void UnkMenuFaceDisplay::unkfunc_0204f570(int type, int index)
{
    if (index == -1) {
        unkfunc_0204f264(0);
        return;
    }
    if (unkfunc_0204f284(index)) {
        return;
    }
    unkfunc_0204f280(index);
    char* src = (char*)unkfunc_0205182c(type, index);
    int dst = 0x2280;
    for (int i = 0; i < 6; i++) {
        unkfunc_020827f0(0x13, dst, src, 0x180);
        dst += 0x400;
        src += 0x180;
    }
    unkfunc_020813e0(0)->unkfunc_020826d8(0, 0, unkfunc_0205182c(type, 9999), 0xa0);
    priority_ = 1;
}
