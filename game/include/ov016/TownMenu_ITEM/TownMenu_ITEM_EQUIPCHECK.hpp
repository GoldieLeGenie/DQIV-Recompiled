#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// town item menu: proposes to equip an item that was just given
struct TownMenu_ITEM_EQUIPCHECK : menu::MenuBase
{
    unsigned char mode_;                    /* 0x1C */
    int cursedMessage_;                     /* 0x20 */
    unsigned char target_;                  /* 0x24 */
    short itemID_;                          /* 0x26 */
    short targetItem_;                      /* 0x28 */

    virtual void menuSetup();
    virtual void menuDraw() {}
    virtual void menuExecute() {}
    virtual void menuUpdate();
    void equipItem();
    void openItemRoot();
};

extern TownMenu_ITEM_EQUIPCHECK gTownMenu_ITEM_EQUIPCHECK;
