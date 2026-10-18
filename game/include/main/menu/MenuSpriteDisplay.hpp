#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// Up to 5 sprites drawn on the sub screen (g_MenuDisplays.sprite_)
struct MenuSpriteDisplay : MenuDisplay {
    int posX_[5];                               // 0x30
    int posY_[5];                               // 0x44
    int attr_[5];                               // 0x58  OAM attribute 2
    int count_;                                 // 0x6C

    MenuSpriteDisplay();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
};

