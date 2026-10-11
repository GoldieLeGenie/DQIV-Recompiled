#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: item list of the selected character / bag
struct UnkTownMenu_02176fa0 : menu::MenuBase
{
    char page_;                             /* 0x1C */
    unsigned char activeChara_;             /* 0x1D */
    menu::MenuItem menuItem_;               /* 0x20 */
    menu::MenuItem cancelItem_;             /* 0x84 */
    CursorMoveGridLoop playerNavigator_;    /* 0xE8 */
    CursorMoveGridLoop fukuroNavigator_;    /* 0xF4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

extern UnkTownMenu_02176fa0 gUnkTownMenu_02176fa0;

