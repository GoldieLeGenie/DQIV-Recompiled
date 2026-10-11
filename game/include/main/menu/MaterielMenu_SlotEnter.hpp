#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"

struct MaterielMenu_SlotEnter : menu::MenuBase
{
    static int machineSelect_;

    int count_;         /* 0x1C */
    int enableFlag_;    /* 0x20 */
    int machine_;       /* 0x24 */

    virtual void menuSetup();
    virtual void menuDraw() {}
    virtual void menuExecute() {}
    virtual void menuUpdate();
    void setSlotType(int type);
    void enableUpdate();
};

extern MaterielMenu_SlotEnter gMaterielMenu_SlotEnter;
