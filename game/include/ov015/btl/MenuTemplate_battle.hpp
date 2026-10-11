#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

struct TOUCHRECT;

// MenuItem layouts of the battle menus
namespace MenuTemplate_battle {
    void BATTLE_CANCEL(menu::MenuItem* menuitem);
    void BATTLE_FIGHT_CANCEL(menu::MenuItem* menuitem);
    void BATTLE_MENUICON2x2(menu::MenuItem* menuitem, int num, int active);
    void BATTLE_MAGIC_2x3(menu::MenuItem* menuitem, int num);
    void BATTLE_TACTICS_2x3(menu::MenuItem* menuitem, int active);
    void BATTLE_PARTY_2x2(menu::MenuItem* menuitem, int num);
    void BATTLE_ARRAYMENU_2x1(menu::MenuItem* menuitem, int active);
    void BATTLE_TACTICSCHANGE_5x1(menu::MenuItem* menuitem, int active, int max);
    void BATTLE_ARRAYCHANGE_5x2(menu::MenuItem* menuitem, int active, int max);
    void BATTLE_ARRAYCHANGE_TO_5x2(menu::MenuItem* menuitem, int active, int max);
    void BATTLE_ARRAYALL_5x2(menu::MenuItem* menuitem, int active, int max);
    void BATTLE_ICON_ENEMY(menu::MenuItem* menuitem, int active, TOUCHRECT* touchRect, int enemyMaxNum);
    void BATTLE_RECT_ENEMY(menu::MenuItem* menuitem);
    void BATTLE_ITEM_ICON32_2x3(menu::MenuItem* menuitem, int itemMaxCount, int active);
}
