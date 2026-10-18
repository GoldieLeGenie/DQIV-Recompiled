#include "main/menu/MenuSpriteDisplay.hpp"
#include "nitro/g2.hpp"

THUMB MenuSpriteDisplay::MenuSpriteDisplay()
{
}

THUMB void MenuSpriteDisplay::setup(int id)
{
    id_ = id;
    reset();
    setType(2);
}

THUMB void MenuSpriteDisplay::update(OamBuffer* main, OamBuffer* sub)
{
    count_ = 0;
}

THUMB void MenuSpriteDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuSpriteDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    if (isDrawable(main, sub)) {
        for (int i = 0; i < count_; i++) {
            GXOamAttr* oam = sub->allocEntry();
            G2_SetOBJAttr(oam, posX_[i], posY_[i] - 192, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32,
                          GX_OAM_COLORMODE_256, attr_[i], 0, 0);
        }
    }
}
