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

    void addValue(int delta);
    void execute();
    void drawLine(DebugMenu* menu, int x, int y);
    void drawValueName(DebugMenu* menu, int x, int y);
};

struct DebugMenuPad {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04

    int isRightRepeat();
    int isLeftRepeat();
    int isAPressed();
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
    static void setBuildDate(const char* date);
    void updateItems();
    void drawItems();
};

extern DebugMenu g_DebugMenu;
