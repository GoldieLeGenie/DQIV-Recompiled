#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town menu root
struct TownMenu_ROOT : menu::MenuBase
{
    enum MENU_LIST {
        ROOT_NONE = -1,
        ROOT_TALK = 0,
        ROOT_MAGIC = 1,
        ROOT_ITEM = 2,
        ROOT_SEARCH = 3,
        ROOT_STATUS = 4,
        ROOT_OPERATION = 5,
        ROOT_ABORT = 6,
    };

    menu::MenuItem menuItem_;               /* 0x1C */
    menu::MenuItem cancelItem_;             /* 0x80 */
    CursorMoveGridLoop navigator_;          /* 0xE4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_021788d8(int active);
    void unkfunc_021789cc();
};

extern TownMenu_ROOT gTownMenu_ROOT;

