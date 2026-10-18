#pragma once
#include "main/menu/MenuBase.hpp"

// town map shop list menu
struct TownShopMenu : menu::MenuBase {
    int unk_1c;     /* shown shop list index (0..5), -1 = none */

    TownShopMenu();
    ~TownShopMenu();
    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int selectShopFrom(int index);
    int selectNextShop();
};

extern TownShopMenu gTownShopMenu;
