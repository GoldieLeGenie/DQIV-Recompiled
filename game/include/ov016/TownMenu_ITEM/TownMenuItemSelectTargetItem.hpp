#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: selection of the item slot of the receiving character
struct TownMenuItemSelectTargetItem : menu::MenuBase
{
    char startPage_;                        /* 0x1C */
    short giveItemID_;                      /* 0x1E */
    int isLock_;                            /* 0x20 */
    int sound_;                             /* 0x24 */
    menu::MenuItem menuItem_;               /* 0x28 */
    menu::MenuItem cancelItem_;             /* 0x8C */
    menu::MenuItem pageItem_;               /* 0xF0 */
    CursorMoveGridLoop playerNavigator_;    /* 0x154 */
    CursorMoveGridLoop fukuroNavigator_;    /* 0x160 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int unkfunc_021734f4();
};

extern TownMenuItemSelectTargetItem gTownMenuItemSelectTargetItem;

