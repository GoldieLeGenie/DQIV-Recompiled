#pragma once
#include <globaldefs.h>
#include "nitro/gx.h"
#include "main/dss/UnkVramRequest.hpp"



// Base of the DS menu display objects (message window, yes/no window, icons, hopping numbers...).
// The vtable (0x020c218c) only holds pure virtual slots; all methods live at 0x0204f198-0x0204f294.
struct UnkMenuDisplay {
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

    UnkMenuDisplay();
    virtual void setup(int id) = 0;
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub) = 0;
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub) = 0;
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub) = 0;
    void unkfunc_0204f1ac();                    // hide
    void unkfunc_0204f1c0();                    // move toward the target
    int unkfunc_0204f214(UnkOamBuffer* main, UnkOamBuffer* sub);
    void unkfunc_0204f260(int type);
    void unkfunc_0204f264(int enable);
    void unkfunc_0204f270(int x, int y);
    void unkfunc_0204f278(int w, int h);
    void unkfunc_0204f280(int key);
    int unkfunc_0204f284(int key);
};
