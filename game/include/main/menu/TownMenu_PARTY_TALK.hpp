#pragma once
#include "main/text/TextAPI.hpp"
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"

struct TownStageManager;

struct TownMenu_PARTY_TALK : menu::MenuBase
{
    char unk_1c;                            /* 0x1C */

    virtual void menuSetup();
    virtual void menuDraw() {}
    virtual void menuExecute() {}
    virtual void menuUpdate();
    void setLeaderMacro(int sortIndex);
};

extern TownMenu_PARTY_TALK gTownMenu_PARTY_TALK;

extern "C" {
}
