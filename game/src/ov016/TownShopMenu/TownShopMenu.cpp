#include "ov016/TownShopMenu/TownShopMenu.hpp"
#include "main/status/ShopList.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_02173d7c.hpp"

THUMB TownShopMenu::TownShopMenu()
{
}

THUMB TownShopMenu::~TownShopMenu()
{
}

THUMB void TownShopMenu::menuSetup()
{
    unkfunc_02177250(0);
}

THUMB void TownShopMenu::menuExecute()
{
}

THUMB void TownShopMenu::menuDraw()
{
    if (unk_1c != -1) {
        unkfunc_02173f7c(unk_1c);
    }
}

THUMB void TownShopMenu::menuUpdate()
{
}

THUMB int TownShopMenu::unkfunc_02177250(int index)
{
    switch (index) {
        case 0:
            if (status::g_Shop.getShopCount(2)) {
                unk_1c = 0;
                redraw_ = 1;
                return 1;
            }
        case 1:
            if (status::g_Shop.getShopCount(3)) {
                unk_1c = 1;
                redraw_ = 1;
                return 1;
            }
        case 2:
            if (status::g_Shop.getShopCount(4)) {
                unk_1c = 2;
                redraw_ = 1;
                return 1;
            }
        case 3:
            if (status::g_Shop.getShopCount(8)) {
                unk_1c = 3;
                redraw_ = 1;
                return 1;
            }
        case 4:
            if (status::g_Shop.getShopCount(9)) {
                unk_1c = 4;
                redraw_ = 1;
                return 1;
            }
        case 5:
            if (status::g_Shop.getShopCount(10)) {
                unk_1c = 5;
                redraw_ = 1;
                return 1;
            }
    }
    unk_1c = -1;
    return 0;
}

THUMB int TownShopMenu::unkfunc_02177300()
{
    if (unkfunc_02177250(unk_1c + 1)) {
        return 1;
    }
    return 0;
}
