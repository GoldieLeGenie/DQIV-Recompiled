#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// MenuItem layouts shared by the town and materiel menus
namespace MenuTemplate_Common {
    void TOWN_PAGE_1x1(menu::MenuItem* menuitem, int active, int maxPage, int x, int y);
    void COMMON_ITEM_ICON32_2x3(menu::MenuItem* menuitem, int itemMaxCount, int active);
}
