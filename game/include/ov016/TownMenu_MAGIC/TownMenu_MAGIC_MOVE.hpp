#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// town magic menu: rura destination selection
struct TownMenu_MAGIC_MOVE : menu::MenuBase
{
    menu::MenuItem menuItem_;               /* 0x1C */
    menu::MenuItem cancelItem_;             /* 0x80 */
    unsigned char activeChara_;             /* 0xE4 */
    unsigned char activeMagic_;             /* 0xE5 */
    short magicID_;                         /* 0xE6 */
    unsigned char haveMagicNum_;            /* 0xE8 */
    unsigned char moveAllocation_;          /* 0xE9 */
    unsigned char moveCount_;               /* 0xEA */
    unsigned char pageStart_;               /* 0xEB */
    unsigned char pageMax_;                 /* 0xEC */
    int isResult_;                          /* 0xF0 */
    unsigned char moveTown_[29];            /* 0xF4 */
    CursorMoveGridLoop navigator_;          /* 0x114 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void moveTown();
    void setActiveChara(unsigned char chara, unsigned char num) { activeChara_ = chara; haveMagicNum_ = num; }
    void setActiveMagic(short id, unsigned char magic) { magicID_ = id; activeMagic_ = magic; }
};

extern TownMenu_MAGIC_MOVE gTownMenu_MAGIC_MOVE;

