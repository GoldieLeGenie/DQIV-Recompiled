#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"

// Window frame drawn as sprites on the sub screen, with its name tab, blinking key cursor and arrow
// (data_020f530c.windowFrame1_, windowFrame2_)
struct UnkMenuWindowFrame : UnkMenuDisplay {
    int cursor_;                                // 0x30  key cursor shown
    int arrow_;                                 // 0x34  arrow shown
    int unk_38;                                 // 0x38
    int arrowX_;                                // 0x3C
    int arrowY_;                                // 0x40
    int nameWidth_;                             // 0x44  name tab width, <= 16: no tab
    int blink_;                                 // 0x48
    int frameType_;                             // 0x4C  1: translucent

    UnkMenuWindowFrame();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void unkfunc_0204f800(int width);
    void unkfunc_0204f80c(int cursor);
    void unkfunc_0204f820(int x, int y, int arrow);
    void unkfunc_0204f828(int type);
    void unkfunc_0204f82c(UnkOamBuffer* oam, int x, int y, int w, int h);
    void unkfunc_0204f900(UnkOamBuffer* oam, int x, int y, int w, int h, int tabW, int tabH);
    void unkfunc_0204fa34(UnkOamBuffer* oam, int x, int y, int length);    // top edge
    void unkfunc_0204fb14(UnkOamBuffer* oam, int x, int y, int length);    // bottom edge
    void unkfunc_0204fc10(UnkOamBuffer* oam, int x, int y, int length);    // left edge
    void unkfunc_0204fcf0(UnkOamBuffer* oam, int x, int y, int length);    // right edge
    void unkfunc_0204fdec(UnkOamBuffer* oam, int x, int y, int chr, int size);
};
