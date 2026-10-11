#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: selection of the character whose items are shown
struct TownMenuItemSelectChara : menu::MenuBase
{
    unsigned char charaCount_;              /* 0x1C */
    int closeMenuMessage_;                  /* 0x20 */
    menu::MenuItem menuItem_;               /* 0x24 */
    menu::MenuItem cancelItem_;             /* 0x88 */
    CursorMoveGridLoop navigator_;          /* 0xEC */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

extern TownMenuItemSelectChara gTownMenuItemSelectChara;

