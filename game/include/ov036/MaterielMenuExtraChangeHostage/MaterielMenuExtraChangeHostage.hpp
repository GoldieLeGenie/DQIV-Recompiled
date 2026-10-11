#pragma once
#include "main/text/TextAPI.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "globaldefs.h"
#include "main/status/PartyStatus.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/global/Global.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "ov000/Commands/TownCommand.hpp"

#include "main/menu/MaterielMenuWindowManager.hpp"

struct MaterielMenuExtraChangeHostage : menu::MenuBase
{                      
    enum HOSTAGE_STATUS {
        HOSTAGE_ISCHANGE = 0,
        HOSTAGE_SELECT   = 1,
        HOSTAGE_CHANGING = 2,
        HOSTAGE_END      = 3
    };
    int ctrlID_;                       
    HOSTAGE_STATUS hostageStatus_;
    short hostageID_;
    short newHostageID_;
    menu::MenuItem menuItem_;            /* 0x28 */
    CursorMoveGridLoop navigator_;        /* 0x8C */
    virtual void menuSetup();      
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void memberUpdate();
    void memberChange();
    int isHostage();

};

extern "C" {

}

extern MaterielMenuExtraChangeHostage gMaterielMenuExtra_ChangeHostage;
