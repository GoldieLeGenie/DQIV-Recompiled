#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

struct MaterielMenuPokerHiandlow : menu::MenuBase {
    int gameMode_;                              // 0x1C
    int doubleupCount_;                         // 0x20
    int haveCoin_;                              // 0x24
    int getCoin_;                               // 0x28
    int win_;                                   // 0x2C 
    int winCoin_[11];                           // 0x30
    menu::MenuItem menu_command_;               // 0x5C
    CursorMoveGridLoop cursor_command_;         // 0xC0

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdate();
    void statusUpdate();
    void showMessage(int mes1, int mes2);
    void judgementHiAndLow();
};

extern MaterielMenuPokerHiandlow gMaterielMenu_POKER_HIANDLOW;
