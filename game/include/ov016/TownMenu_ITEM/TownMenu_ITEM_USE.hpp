#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town item menu: target selection of a used item
struct TownMenu_ITEM_USE : menu::MenuBase
{
    unsigned char activeChara_;             /* 0x1C */
    short itemID_;                          /* 0x1E */
    unsigned char page_;                    /* 0x20 */
    int openCharaSelect_;                   /* 0x24 */
    int isResult_;                          /* 0x28 */
    short updateHP_;                        /* 0x2C */
    short updateMP_;                        /* 0x2E */
    int openMessage_;                       /* 0x30 */
    int resultMes_[4];                      /* 0x34 */
    int conditionPoison_;                   /* 0x44 */
    menu::MenuItem charaItem_;              /* 0x48 */
    menu::MenuItem cancelItem_;             /* 0xAC */
    menu::MenuItem pageItem_;               /* 0x110 */
    CursorMoveGridLoop navigator_;          /* 0x174 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_02171e8c();
    void itemUse();
    int checkUseCharaAlive(int target);
    int useHealItem();
    void openHealMessage();
};

extern TownMenu_ITEM_USE gTownMenu_ITEM_USE;

