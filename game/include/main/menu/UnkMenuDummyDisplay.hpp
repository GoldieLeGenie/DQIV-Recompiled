#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// Display object drawing nothing (g_MenuDisplays.dummy_)
struct UnkMenuDummyDisplay : UnkMenuDisplay {
    UnkMenuDummyDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
};
