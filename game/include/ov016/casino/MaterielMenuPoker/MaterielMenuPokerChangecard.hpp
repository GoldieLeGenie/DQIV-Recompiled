#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/dss/DssUtils.hpp"

struct MaterielMenuPokerChangecard : menu::MenuBase {
    int gameMode_;                              // 0x1C
    int betCoin_;                               // 0x20
    int getCoin_;                               // 0x24
    int haveCoin_;                              // 0x28
    int combination_;                           // 0x2C
    int blink_;                                 // 0x30 
    int change_[5];                             // 0x34 
    int isPlaySound_;                           // 0x48 
    int effectCount_;                           // 0x4C
    int animation_;                             // 0x50
    unsigned short ang_;                        // 0x54
    int gyre_;                                  // 0x58
    int index_;                                 // 0x5C
    dss::Fix32 distance_;                       // 0x60
    menu::MenuItem menuItem_;                   // 0x64 
    menu::MenuItem menuItem2_;                  // 0xC8 
    CursorMoveGridLoop cursor_;                 // 0x12C
    CursorMoveGridLoop cursor2_;                // 0x138

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdata();
    void menuUpdata();
    void changeCard();
    void showMessage(int mes1, int mes2);
    void pokerDealCard();
    void pokerChangeCard();
    void pokerReverseCard(bool doubleupNext);
    void hopCard(int index);
    void setSoundNo();
};

extern MaterielMenuPokerChangecard gMaterielMenu_POKER_CHANGECARD;
