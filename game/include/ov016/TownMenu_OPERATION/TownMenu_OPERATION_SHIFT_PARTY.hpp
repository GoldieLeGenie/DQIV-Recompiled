#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// party order page of the town operation menu
struct TownMenu_OPERATION_SHIFT_PARTY : menu::MenuBase
{
    char selectCharaAllocation_[4];         /* 0x1C */
    unsigned char selectCharaMaxCount_;     /* 0x20 */
    int carriageEnable_;                    /* 0x24 */
    int enableEnter_;                       /* 0x28 */
    unsigned char page_;                    /* 0x2C */
    menu::MenuItem menuItem_;               /* 0x30 */
    menu::MenuItem cancelItem_;             /* 0x94 */
    CursorMoveGridLoop navigator_;          /* 0xF8 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0216ccbc();
    void unkfunc_0216cdd4();
};

extern TownMenu_OPERATION_SHIFT_PARTY gTownMenu_OPERATION_SHIFT_PARTY;

