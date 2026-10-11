#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// equipment page of the town operation menu
struct TownMenu_OPERATION_EQUIP : menu::MenuBase
{
    char itemType_;                         /* 0x1C */
    char beginItemType_;                    /* 0x1D */
    char curseType_;                        /* 0x1E */
    unsigned char itemIdList_[12];          /* 0x1F */
    unsigned char itemPosList_[12];         /* 0x2B */
    unsigned char itemCount_;               /* 0x37 */
    unsigned char activeChara_;             /* 0x38 */
    unsigned char itemPageStart_;           /* 0x39 */
    int itemListActive_;                    /* 0x3C */
    int msgEnd2NextEquipment_;              /* 0x40 */
    int playSound_;                         /* 0x44 */
    menu::MenuItem itemItem_;               /* 0x48 */
    menu::MenuItem charaItem_;              /* 0xAC */
    menu::MenuItem cancelItem_;             /* 0x110 */
    menu::MenuItem removeItem_;             /* 0x174 */
    CursorMoveGridLoop charaNavigator_;     /* 0x1D8 */
    CursorMoveGridLoop itemNavigator_;      /* 0x1E4 */
    CursorMoveGridLoop removeNavigator_;    /* 0x1F0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setItemTypeList();
    void setEquip();
    int unkfunc_0216c6ac();
};

extern TownMenu_OPERATION_EQUIP gTownMenu_OPERATION_EQUIP;

