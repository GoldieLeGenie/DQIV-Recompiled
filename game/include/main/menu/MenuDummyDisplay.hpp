#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// Display object drawing nothing (g_MenuDisplays.dummy_)
struct MenuDummyDisplay : MenuDisplay {
    MenuDummyDisplay();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
};
