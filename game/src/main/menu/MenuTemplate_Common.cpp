#include "main/menu/MenuTemplate_Common.hpp"

THUMB void MenuTemplate_Common::TOWN_PAGE_1x1(menu::MenuItem* menuitem, int active, int maxPage, int x, int y)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x06, 0x06, 0x1c, 0x1c},
        {-1, -1, 0x00, 0x00, 0x00, 0x00},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
    if (maxPage == 0) {
        menuitem->setMenuItem(menu, 1, 1, 0);
        return;
    }
    menuitem->active_ = active;
    menuitem->setBaseXY(x, y);
}

THUMB void MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(menu::MenuItem* menuitem, int itemMaxCount, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x14, 0x0c, 0x68, 0x18},
        {1, 2, 0x8c, 0x0c, 0x68, 0x18},
        {1, 2, 0x14, 0x2c, 0x68, 0x18},
        {1, 2, 0x8c, 0x2c, 0x68, 0x18},
        {1, 2, 0x14, 0x4c, 0x68, 0x18},
        {1, 2, 0x8c, 0x4c, 0x68, 0x18},
        {-1, -1, 0x00, 0x00, 0x00, 0x00},
    };
    menuitem->setMenuItem(menu, 2, 3, itemMaxCount);
    menuitem->active_ = active;
}
