#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"

struct MaterielMenu_EXTRA_PRESENT_EXP : menu::MenuBase
{
    int activeChara_;                   /* 0x1C */
    int extraExp_;                      /* 0x20 */
    int subExp_;                        /* 0x24 */
    unsigned char levelUpMode_;         /* 0x28 */
    int bgm_;                           /* 0x2C */
    menu::MenuItem menuItem_;           /* 0x30 */
    CursorMoveGridLoop navigator_;     /* 0x94 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};


extern MaterielMenu_EXTRA_PRESENT_EXP gMaterielMenu_EXTRA_PRESENT_EXP;
