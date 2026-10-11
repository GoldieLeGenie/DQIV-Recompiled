#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"

struct DebugMenu;

struct DebugMenuItem {
    int type_;                                  // 0x00  3: end of the list
    const char* name_;                          // 0x04
    int enable_;                                // 0x08
    int min_;                                   // 0x0C
    int max_;                                   // 0x10
    int* value_;                                // 0x14
    const char** valueName_;                    // 0x18
    void (*onChange_)();                        // 0x1C
    void (*onExec_)();                          // 0x20

    void unkfunc_02058568(int delta);
    void unkfunc_020585c0();
    void unkfunc_020585d8(DebugMenu* menu, int x, int y);
    void unkfunc_02058640(DebugMenu* menu, int x, int y);
};

struct DebugMenuPad {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04

    int unkfunc_020582e0();
    int unkfunc_020582f8();
    int unkfunc_02058310();
};

struct DebugMenu : UnkGlobalPart {
    // vtable                                   // 0x00
    int x_;                                     // 0x04
    int y_;                                     // 0x08
    int cursor_;                                // 0x0C
    char name_[0x10];                           // 0x10
    int unk_20[18];                             // 0x20
    int unk_68;                                 // 0x68
    int unk_6c;                                 // 0x6C
    int unk_70;                                 // 0x70
    const char* title_;                         // 0x74
    DebugMenuItem* item_;                       // 0x78

    DebugMenu(const char* name);
    int unkfunc_02058378();
    int unkfunc_02058380();
    virtual void update();
    virtual void draw();
    virtual int isEnd();
    void unkfunc_020583a8(int count);
    void print(int x, int y, const char* fmt, ...);
    static void unkfunc_02058430(const char* date);
    void unkfunc_02058448();
    void unkfunc_020584c4();
};

extern DebugMenu data_0210bc40;
