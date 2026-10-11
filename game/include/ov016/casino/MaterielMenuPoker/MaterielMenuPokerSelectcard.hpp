#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/dss/DssUtils.hpp"

struct MaterielMenuPokerSelectcard : menu::MenuBase {
    int activeCard_;                            // 0x1C
    int doubleUpCount_;                         // 0x20
    int haveCoin_;                              // 0x24
    int getCoin_;                               // 0x28
    int gameMode_;                              // 0x2C
    int winCoin_[11];                           // 0x30
    int isPlaySound_;                           // 0x5C 
    int cardCounter_[5];                        // 0x60
    int win_;                                   // 0x74 
    int blink_;                                 // 0x78 
    int animation_;                             // 0x7C
    unsigned short ang_;                        // 0x80
    int gyre_;                                  // 0x84
    int index_;                                 // 0x88
    dss::Fix32 distance_;                       // 0x8C
    menu::MenuItem menuItem_;                   // 0x90 
    CursorMoveGridLoop cursor_;                 // 0xF4 
    CursorMoveGridLoop cursor2_;                // 0x100

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdate();
    void statusUpdate();
    void doubleupUpdate();
    bool startDoubleup();
    void showMessage(int mes1, int mes2);
    void pokerOpenCard(bool first);
    void pokerReverseCard();
    void hopCard(int index);
    void setSoundNo();
};

extern MaterielMenuPokerSelectcard gMaterielMenu_POKER_SELECTCARD;
