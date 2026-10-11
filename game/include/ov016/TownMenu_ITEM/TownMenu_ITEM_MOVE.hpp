#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: destination selection of a chimaera wing
struct TownMenu_ITEM_MOVE : menu::MenuBase
{
    menu::MenuItem menuItem_;               /* 0x1C */
    menu::MenuItem cancelItem_;             /* 0x80 */
    menu::MenuItem pageItem_;               /* 0xE4 */
    CursorMoveGridLoop unk_148;             /* 0x148 */
    unsigned char moveCount_;               /* 0x154 */
    unsigned char unk_155;                  /* 0x155 */
    unsigned char moveTown_[29];            /* 0x156 */
    int rulaOK_;                            /* 0x174 */
    CursorMoveGridLoop navigator_;          /* 0x178 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void moveTown();
};

extern TownMenu_ITEM_MOVE gTownMenu_ITEM_MOVE;
