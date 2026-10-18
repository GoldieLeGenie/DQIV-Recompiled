#pragma ipa file
#include "main/menu/MenuArrowDisplay.hpp"
#include "nitro/g2.hpp"

static int s_animIndex[10] = { 0, 1, 2, 3, 4, 3, 2, 1, 0, -1 };
static int s_animWait[10] = { 2, 3, 4, 5, 6, 5, 4, 3, 2, -1 };

THUMB UnkMenuArrowDisplay::UnkMenuArrowDisplay()
{
}

THUMB void UnkMenuArrowDisplay::setup(int id)
{
    id_ = id;
    count_ = 0;
    frame_ = 0;
    animIndex_ = 0;
    animWait_ = 0;
    setType(2);
}

THUMB void UnkMenuArrowDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    count_ = 0;
    updateAnim();
}

THUMB void UnkMenuArrowDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuArrowDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    int blink = (frame_ & 0x20) ? FALSE : TRUE;
    frame_++;
    if (!isDrawable(main, sub)) {
        return;
    }
    int animChar = getAnimCharName();
    for (int i = 0; i < count_; i++) {
        int x = posX_[i] - 8;
        int y = posY_[i] - 8;
        int kind = kind_[i];
        int charName;
        int effect;
        if (flag_[i] && !blink) {
            y = 0xc0;
        }
        switch (kind) {
        case 1:
            charName = 0x192;
            effect = GX_OAM_EFFECT_FLIP_V;
            break;
        case 2:
            charName = 0x192;
            effect = GX_OAM_EFFECT_NONE;
            break;
        case 3:
            charName = 0x190;
            effect = GX_OAM_EFFECT_FLIP_H;
            break;
        case 4:
            charName = 0x190;
            effect = GX_OAM_EFFECT_NONE;
            break;
        case 5:
            charName = animChar;
            effect = GX_OAM_EFFECT_NONE;
            break;
        case 6:
            charName = 0x1d2;
            effect = GX_OAM_EFFECT_NONE;
            break;
        case 7:
            charName = 0x152;
            effect = GX_OAM_EFFECT_NONE;
            break;
        default:
            charName = 0x12a;
            effect = GX_OAM_EFFECT_NONE;
            break;
        }
        if (charName != 0x12a) {
            GXOamAttr* oam = sub->allocEntry();
            oam->attr01 = ((x & 0x1ff) << 16) | ((y & 0xff) | GX_OAM_SHAPE_16x16) | effect;
            oam->attr2 = charName | (15 << 12);
        }
    }
}

THUMB int UnkMenuArrowDisplay::getAnimCharName()
{
    return s_animIndex[animIndex_] * 2 + 0x1ca;
}

THUMB void UnkMenuArrowDisplay::updateAnim()
{
    if (animWait_ < s_animWait[animIndex_]) {
        animWait_++;
        return;
    }
    animWait_ = 0;
    animIndex_++;
    if (s_animIndex[animIndex_] == -1) {
        animIndex_ = 0;
    }
}

THUMB void UnkMenuArrowDisplay::addArrow(int x, int y, int kind, int flag)
{
    posX_[count_] = x;
    posY_[count_] = y;
    kind_[count_] = kind;
    flag_[count_] = flag;
    count_++;
    setEnable(1);
}

THUMB void UnkMenuArrowDisplay::resetBlink()
{
    frame_ = 0;
}
