#include "main/menu/UnkMenuSpriteDisplay.hpp"
#include "nitro/g2.hpp"

THUMB UnkMenuSpriteDisplay::UnkMenuSpriteDisplay()
{
}

THUMB void UnkMenuSpriteDisplay::setup(int id)
{
    id_ = id;
    unkfunc_0204f1ac();
    unkfunc_0204f260(2);
}

THUMB void UnkMenuSpriteDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    count_ = 0;
}

THUMB void UnkMenuSpriteDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuSpriteDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (unkfunc_0204f214(main, sub)) {
        for (int i = 0; i < count_; i++) {
            GXOamAttr* oam = sub->unkfunc_02082694();
            G2_SetOBJAttr(oam, posX_[i], posY_[i] - 192, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_32x32,
                          GX_OAM_COLORMODE_256, attr_[i], 0, 0);
        }
    }
}
