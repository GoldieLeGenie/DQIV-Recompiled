#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

struct MaterielMenuPokerBetcoin : menu::MenuBase {
    int haveCoin_;                              // 0x1C
    int betCoin_;                               // 0x20
    int messageMode_;                           // 0x24
    int unk_28;                                 // 0x28 (DS-only: coin blink counter)
    menu::MenuItem menuItem_;                   // 0x2C (DS-only)
    CursorMoveGridLoop cursor_;                 // 0x90 (DS-only)

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool unkfunc_02170024();
    bool unkfunc_021700f0();
    void showMessage(int mes);
};

extern MaterielMenuPokerBetcoin gMaterielMenu_POKER_BETCOIN;
