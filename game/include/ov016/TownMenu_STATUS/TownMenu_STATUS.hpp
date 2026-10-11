#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// character status pages of the town menu
struct TownMenu_STATUS : menu::MenuBase
{
    unsigned char partyCount_;          /* 0x1C */
    unsigned char activeChara_;         /* 0x1D */
    unsigned char page_;                /* 0x1E */
    unsigned char pageSwap_;            /* 0x1F */
    unsigned char unk_20;               /* 0x20 */
    CursorMoveGridLoop navigator_;      /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x30 */
    menu::MenuItem cancelItem_;         /* 0x94 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0217ad50();
};

extern TownMenu_STATUS gTownMenu_STATUS;

