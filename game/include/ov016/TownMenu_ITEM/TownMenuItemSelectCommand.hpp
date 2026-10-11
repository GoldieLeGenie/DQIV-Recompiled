#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: command selection for an item (use, give, throw, equip, show)
struct TownMenuItemSelectCommand : menu::MenuBase
{
    short itemID_;                          /* 0x1C */
    unsigned char useItemPlayer_;           /* 0x1E */
    unsigned char commandFlag_;             /* 0x1F */
    int isLock_;                            /* 0x20 */
    int resetLock_;                         /* 0x24 */
    int boots_;                             /* 0x28 */
    int isResult_;                          /* 0x2C */
    int resultMes_[8];                      /* 0x30 */
    int useItem_;                           /* 0x50 */
    int useInoriNoyubiwa_;                  /* 0x54 */
    int sound_;                             /* 0x58 */
    int updataHP_[4];                       /* 0x5C */
    int updataMP_;                          /* 0x6C */
    int openMessage_;                       /* 0x70 */
    menu::MenuItem menuItem_;               /* 0x74 */
    menu::MenuItem cancelItem_;             /* 0xD8 */
    CursorMoveGridLoop navigator_;          /* 0x13C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0217638c();
    void judgeUseItem();
    void useItemNoTarget();
    void judgeEquipItem();
    void setItemShowAction();
    void judgeThrowItem();
    int unkfunc_02176b90();
    void unkfunc_02176bb0();
    void unkfunc_02176bfc();
    void resultItem();
    void unkfunc_02176e14();
    void openUseItemMessage();
};

extern TownMenuItemSelectCommand gTownMenuItemSelectCommand;

