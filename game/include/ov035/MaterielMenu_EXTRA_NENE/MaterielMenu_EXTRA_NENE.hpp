#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"

struct MaterielMenu_EXTRA_NENE : menu::MenuBase
{
    unsigned char mode_;                /* 0x1C */
    unsigned char activeChara_;         /* 0x1D */
    unsigned char drawMode_;            /* 0x1E */
    unsigned char neneItemCount_;       /* 0x1F */
    int proceeds_;                      /* 0x20 */
    menu::MenuItem menuItem_;           /* 0x24 */
    CursorMoveGridLoop navigator_;     /* 0x88 */
    int sellItem_;                      /* 0x94 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void checkSellItem();
    void calcProceeds();
    bool checkHaveItem(bool sack);
    void showMessage(int messageID1, int messageID2, int messageID3);
};


extern MaterielMenu_EXTRA_NENE gMaterielMenu_EXTRA_NENE;
