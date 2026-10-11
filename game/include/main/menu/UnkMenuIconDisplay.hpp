#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"
#include "main/menu/UnkMenuIconData.hpp"

struct UnkHoppingDigit {
    int frame_;                                 // 0x00  -99: free, < 0: waiting
    int number_;                                // 0x04
    int x_;                                     // 0x08
    int y_;                                     // 0x0C
    int drawY_;                                 // 0x10

    void unkfunc_02055e00();
    void unkfunc_02055e08(int x, int y, int number, int delay);
    void unkfunc_02055e24(UnkOamBuffer* oam);
    void unkfunc_02055e58(UnkOamBuffer* oam);
    int unkfunc_02055ef4();
};


struct UnkHoppingNumber : UnkMenuDisplay {
    UnkHoppingDigit digit_[40];                 // 0x30

    UnkHoppingNumber();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    UnkHoppingDigit* unkfunc_02055f90();
    void unkfunc_02055fbc(int x, int y, int value, int delay);
};

// Four 48x48 icons on the main screen 
struct UnkMenuIconDisplay : UnkMenuDisplay {
    int posX_[4];                               // 0x30
    int posY_[4];                               // 0x40
    int value_[4];                              // 0x50
    int flag_[4];                               // 0x60

    UnkMenuIconDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void unkfunc_02055b4c(UnkOamBuffer* oam, int x, int y, int index);
    void unkfunc_02055c48(int index, int value);
    void unkfunc_02055dc8(int index, int x, int y);
    void unkfunc_02055dd4(int index, int flag);
    void unkfunc_02055ddc(int x, int y, void* src, int srcX, int srcY, int width);
};

