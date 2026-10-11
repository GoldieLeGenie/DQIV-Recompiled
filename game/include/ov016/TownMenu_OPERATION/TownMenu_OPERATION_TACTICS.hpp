#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// tactics page of the town operation menu
struct TownMenu_OPERATION_TACTICS : menu::MenuBase
{
    unsigned char mode_;                    /* 0x1C */
    unsigned char prevMode_;                /* 0x1D */
    unsigned char activeChara_;             /* 0x1E */
    unsigned char activeTactics_;           /* 0x1F */
    char chara_[10];                        /* 0x20 */
    unsigned char charaCount_;              /* 0x2A */
    menu::MenuItem tacticsItem_;            /* 0x2C */
    menu::MenuItem charaItem_;              /* 0x90 */
    menu::MenuItem cancelItem_;             /* 0xF4 */
    CursorMoveGridLoop charaNavigator_;     /* 0x158 */
    CursorMoveGridLoop tacticsNavigator_;   /* 0x164 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0216c9bc();
    void unkfunc_0216c9f8();
};

extern TownMenu_OPERATION_TACTICS gTownMenu_OPERATION_TACTICS;

