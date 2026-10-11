#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// MenuItem layouts of the town menus
namespace MenuTemplate_town {
    void TOWN_CANCEL(menu::MenuItem* menuitem);
    void TOWN_OPERATION(menu::MenuItem* menuitem, int active, int max);
    void TOWN_ICON32_5x2(menu::MenuItem* menuitem, int active, int max);
    void TOWN_OP_BGMVOL(menu::MenuItem* menuitem, int active);
    void TOWN_OP_EFFECTVOL(menu::MenuItem* menuitem, int active);
    void TOWN_OP_BATTLE(menu::MenuItem* menuitem, int active);
    void TOWN_OP_NOEQUIP(menu::MenuItem* menuitem);
    void TOWN_CAREER_SWITCH(menu::MenuItem* menuitem, int active);
    void townMenuRootIcon(menu::MenuItem* menuitem, int active);
    void townMenuSelectCharaIcon(menu::MenuItem* menuitem, int max, int active);
    void townMenuSelectMagic(menu::MenuItem* menuitem, int max, int active);
    void townMenuItemCommand(menu::MenuItem* menuitem, int flagItemCommand, int active);
    void townMenuTacticsSelectChara(menu::MenuItem* menuitem, int max, int active, int x, int y);
    void townMenuTacticsSelectHalfChara(menu::MenuItem* menuitem, int max, int active, int x, int y);
    void townMenuItemSelectChara(menu::MenuItem* menuitem, int max, int active);
    void townMenuItemSelectHalfChara(menu::MenuItem* menuitem, int max, int active);
    void townMenuTacticsSelectTC(menu::MenuItem* menuitem, int active);
    void townMenuPageTargetChara(menu::MenuItem* menuitem, int max, int active);
    void townMenuTacticsBoxUp(menu::MenuItem* menuitem, int active);
    void townMenuPageRightArrow(menu::MenuItem* menuitem, int x, int y);
    void townMenuPageRightTwoArrow(menu::MenuItem* menuitem, int active);
    void townMenuPageCenter(menu::MenuItem* menuitem, int active);
}
