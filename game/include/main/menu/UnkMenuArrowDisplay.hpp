#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"

// Up to 16 arrows / cursors drawn on the sub screen (data_020f530c.arrow_)
struct UnkMenuArrowDisplay : UnkMenuDisplay {
    int posX_[16];                              // 0x030
    int posY_[16];                              // 0x070
    int kind_[16];                              // 0x0B0
    int flag_[16];                              // 0x0F0  blinking
    int count_;                                 // 0x130
    int frame_;                                 // 0x134
    int animIndex_;                             // 0x138
    int animWait_;                              // 0x13C

    UnkMenuArrowDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    int unkfunc_020525f4();
    void unkfunc_0205260c();
    void unkfunc_02052658(int x, int y, int kind, int flag);
    void unkfunc_02052694();
};

