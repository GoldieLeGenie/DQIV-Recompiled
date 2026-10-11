#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"
#include "main/dss/UnkBgBuffer.hpp"

// One line of text drawn as sprites (data_020f530c.texts_: message window lines and name plate)
struct UnkMenuTextDisplay : UnkMenuDisplay {
    unsigned char charWidths_[0x100];           // 0x030
    int visibleWidth_;                          // 0x130
    int visibleCount_;                          // 0x134
    int width_;                                 // 0x138
    int count_;                                 // 0x13C
    int xlu_;                                   // 0x140
    UnkCharBuffer char_;                        // 0x144
    char charData_[0x800];                      // 0x154
    int frame_;                                 // 0x954  draw the frame
    int frameWidth_;                            // 0x958
    int unk_95c;                                // 0x95C
    int palette_;                               // 0x960
    int priority_;                              // 0x964
    int timer_;                                 // 0x968
#ifdef DQIV_NATIVE
    int letterPalette_ = -1;                    // the letters' palette when not palette_ (name plates' health)
#endif

    UnkMenuTextDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void unkfunc_0204e340(UnkOamBuffer* oam, int x, int y, int width, int height);
    void unkfunc_0204e430(int font, const char* text, int x, int y, int clear);
    int unkfunc_0204e4f4(int font, const char* text, int priority, int flag);
    void unkfunc_0204e584(int count);
    void unkfunc_0204e5c8(int width);
    int unkfunc_0204e5d0();
    int unkfunc_0204e5d8();
    int unkfunc_0204e5e0();
    void unkfunc_0204e5e8(int scroll);
    void unkfunc_0204e668(int flag);
    void unkfunc_0204e670(UnkOamBuffer* oam, int x, int y, int chr);
    void unkfunc_0204e6ac(UnkOamBuffer* oam, int x, int y, int chr);
};

