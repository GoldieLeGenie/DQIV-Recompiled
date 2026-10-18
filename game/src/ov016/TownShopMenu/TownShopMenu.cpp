#include "ov016/TownShopMenu/TownShopMenu.hpp"
#include "main/status/ShopList.hpp"
#include "ov016/UnkTownMenuDraw/TownMenuOperationDraw.hpp"

THUMB TownShopMenu::TownShopMenu()
{
}

THUMB TownShopMenu::~TownShopMenu()
{
}

THUMB void TownShopMenu::menuSetup()
{
    selectShopFrom(0);
}

THUMB void TownShopMenu::menuExecute()
{
}

THUMB void TownShopMenu::menuDraw()
{
    if (shopIndex_ != -1) {
        drawTownShopGoods(shopIndex_);
    }
}

THUMB void TownShopMenu::menuUpdate()
{
}

THUMB int TownShopMenu::selectShopFrom(int index)
{
    switch (index) {
        case 0:
            if (status::g_Shop.getShopCount(2)) {
                shopIndex_ = 0;
                redraw_ = 1;
                return 1;
            }
        case 1:
            if (status::g_Shop.getShopCount(3)) {
                shopIndex_ = 1;
                redraw_ = 1;
                return 1;
            }
        case 2:
            if (status::g_Shop.getShopCount(4)) {
                shopIndex_ = 2;
                redraw_ = 1;
                return 1;
            }
        case 3:
            if (status::g_Shop.getShopCount(8)) {
                shopIndex_ = 3;
                redraw_ = 1;
                return 1;
            }
        case 4:
            if (status::g_Shop.getShopCount(9)) {
                shopIndex_ = 4;
                redraw_ = 1;
                return 1;
            }
        case 5:
            if (status::g_Shop.getShopCount(10)) {
                shopIndex_ = 5;
                redraw_ = 1;
                return 1;
            }
    }
    shopIndex_ = -1;
    return 0;
}

THUMB int TownShopMenu::selectNextShop()
{
    if (selectShopFrom(shopIndex_ + 1)) {
        return 1;
    }
    return 0;
}
