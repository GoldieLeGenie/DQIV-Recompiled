#pragma once
#ifdef DQIV_NATIVE
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "ov016/MenuAchievements/AchievementsPanel.hpp"

// The PC version's achievements, a command of the town operation menu.
struct TownMenu_OPERATION_ACHIEVEMENTS : menu::MenuBase
{
    menu::MenuItem item_;
    AchievementsPanel achievements_;

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

extern TownMenu_OPERATION_ACHIEVEMENTS gTownMenu_OPERATION_ACHIEVEMENTS;
#endif
