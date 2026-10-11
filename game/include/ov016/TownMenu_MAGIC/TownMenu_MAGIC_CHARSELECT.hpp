#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town magic menu: target selection of a healing spell
struct TownMenu_MAGIC_CHARSELECT : menu::MenuBase
{
    unsigned char fromActiveChara_;         /* 0x1C */
    unsigned char toActiveChara_;           /* 0x1D */
    short magicID_;                         /* 0x1E */
    unsigned char activeMagic_;             /* 0x20 */
    unsigned char haveMagicNum_;            /* 0x21 */
    int isResult_;                          /* 0x24 */
    int resultMes_[4];                      /* 0x28 */
    int openMessage_;                       /* 0x38 */
    unsigned short updateHP_;               /* 0x3C */
    int conditionPoison_;                   /* 0x40 */
    unsigned char page_;                    /* 0x44 */
    menu::MenuItem charaItem_;              /* 0x48 */
    menu::MenuItem cancelItem_;             /* 0xAC */
    CursorMoveGridLoop navigator_;          /* 0x110 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void useMagic();
};

extern TownMenu_MAGIC_CHARSELECT gTownMenu_MAGIC_CHARSELECT;

