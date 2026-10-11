#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: selection of the character an item is given to
struct TownMenuItemSelectTargetChara : menu::MenuBase
{
    unsigned char charaCount_;              /* 0x1C */
    unsigned char activeChara_;             /* 0x1D */
    unsigned char targetChara_;             /* 0x1E */
    short giveItemID_;                      /* 0x20 */
    int isLock_;                            /* 0x24 */
    int sound_;                             /* 0x28 */
    menu::MenuItem menuItem_;               /* 0x2C */
    menu::MenuItem cancelItem_;             /* 0x90 */
    CursorMoveGridLoop navigator_;          /* 0xF4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

extern TownMenuItemSelectTargetChara gTownMenuItemSelectTargetChara;

