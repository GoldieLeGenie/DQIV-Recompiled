#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"

// Display object drawing nothing (data_020f530c.dummy_)
struct UnkMenuDummyDisplay : UnkMenuDisplay {
    UnkMenuDummyDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
};
