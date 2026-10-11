#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"

// Up to 5 sprites drawn on the sub screen (data_020f530c.sprite_)
struct UnkMenuSpriteDisplay : UnkMenuDisplay {
    int posX_[5];                               // 0x30
    int posY_[5];                               // 0x44
    int attr_[5];                               // 0x58  OAM attribute 2
    int count_;                                 // 0x6C

    UnkMenuSpriteDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
};

