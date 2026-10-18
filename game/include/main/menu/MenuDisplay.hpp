#pragma once
#include <globaldefs.h>
#include "nitro/gx.h"
#include "main/dss/VramRequest.hpp"



// Base of the DS menu display objects (message window, yes/no window, icons, hopping numbers...).
// The vtable (0x020c218c) only holds pure virtual slots; all methods live at 0x0204f198-0x0204f294.
struct MenuDisplay {
    // vtable                                   // 0x00
    int enable_;                                // 0x04
    int type_;                                  // 0x08  1: drawn on main, 2: drawn on sub, 3: both
    int id_;                                    // 0x0C
    int x_;                                     // 0x10
    int y_;                                     // 0x14
    int w_;                                     // 0x18
    int h_;                                     // 0x1C
    int targetX_;                               // 0x20
    int targetY_;                               // 0x24
    int moveCount_;                             // 0x28
    int key_;                                   // 0x2C  loaded resource, -1: none

    MenuDisplay();
    virtual void setup(int id) = 0;
    virtual void update(OamBuffer* main, OamBuffer* sub) = 0;
    virtual void execute(OamBuffer* main, OamBuffer* sub) = 0;
    virtual void draw(OamBuffer* main, OamBuffer* sub) = 0;
    void reset();                    // hide
    void updateMove();                    // move toward the target
    int isDrawable(OamBuffer* main, OamBuffer* sub);
    void setType(int type);
    void setEnable(int enable);
    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setKey(int key);
    int isKey(int key);
};
