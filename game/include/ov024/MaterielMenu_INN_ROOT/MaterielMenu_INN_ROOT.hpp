#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"

struct MaterielMenu_INN_ROOT : menu::MenuBase
{
    int mode_;                  /* 0x1C */
    int innCharge_;             /* 0x20 */
    int fadeMode_;              /* 0x24 */
    int soundCount_;            /* 0x28 */
    int extraInnType_;          /* 0x2C */
    int revivalPlayer_[10];     /* 0x30 */
    int stayCount_;             /* 0x58 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void checkMoney();
    void fadeEffect();
    void showMessage(int mes);
};

extern MaterielMenu_INN_ROOT gMaterielMenu_INN_ROOT;
