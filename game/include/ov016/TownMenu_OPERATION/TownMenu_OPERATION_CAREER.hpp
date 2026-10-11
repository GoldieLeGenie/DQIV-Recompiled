#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// career (battle history) page of the town operation menu 
struct TownMenu_OPERATION_CAREER : menu::MenuBase
{
    unsigned char pageMax_;             /* 0x1C */
    int page_;                          /* 0x20 */
    int bookOfBeasts_;                  /* 0x24 */
    unsigned char unk_28[3];            /* 0x28 */
    menu::MenuItem menuItem_;           /* 0x2C */
    CursorMoveGridLoop navigator_;      /* 0x90 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void SetTextPage(int page, int type);
};

extern TownMenu_OPERATION_CAREER gTownMenu_OPERATION_CAREER;
