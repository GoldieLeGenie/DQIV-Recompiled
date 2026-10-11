#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// town item menu: checks of an item exchange between two characters
struct TownMenu_ITEM_CHECKTARGET : menu::MenuBase
{
    int isSpace_;                           /* 0x1C */
    int isPlaySound_;                       /* 0x20 */
    short targetItem_;                      /* 0x24 */
    short curseItemID_;                     /* 0x26 */
    short changeItemID_;                    /* 0x28 */
    short activeItem_;                      /* 0x2A */
    unsigned char toActiveChara_;           /* 0x2C */
    unsigned char fromActiveChara_;         /* 0x2D */

    virtual void menuSetup();
    virtual void menuDraw() {}
    virtual void menuExecute() {}
    virtual void menuUpdate();
    void openMessage(int actor, int target, int iname, int iname1, int iname2, int leadpc, int mes);
    unsigned int checkGiveFlag();
    void changeItem(unsigned int flag);
    void setTargetItem();
    int isCurse();
    void unkfunc_02172bdc();
};

extern TownMenu_ITEM_CHECKTARGET gTownMenu_ITEM_CHECKTARGET;
