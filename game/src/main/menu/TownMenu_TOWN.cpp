#include "main/menu/TownMenu_TOWN.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/status/OptionStatus.hpp"

static MenuPartsEntry s_menuIconParts[] = {
    {0x13, 0, 0, 5, 0x50, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static MenuPartsEntry s_shopParts[] = {
    {0x13, 0, 0, 3, 0xc0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static MenuPartsEntry s_mapParts[] = {
    {0x13, 0, 0, 2, 0xa0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static MenuPartsEntry s_cameraParts[] = {
    {0x13, 0, 0, 0, 0, 0xa0, 0, 0},
    {0x13, 0, 0, 1, 0xe0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static MenuPartsEntry s_messageParts[] = {
    {1, 0, -0x1000, 0, 0x40, 0x48, 0x78, 0x28},
    {0xd, 9, -0xf00, 0, 0x53, 0x50, 0x58, 0x10},
    {0xd, 9, -0xf00, 1, 0x50, 0x5a, 0x58, 0xe},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static MENUITEM_DATA s_menuItemData[] = {
    {1, 2, 2, 0xa2, 0x1c, 0x1c},
    {1, 2, 0xe2, 0xa2, 0x1c, 0x1c},
    {1, 2, 0xa2, 0xa2, 0x1c, 0x1c},
    {1, 2, 0xc2, 0xa2, 0x1c, 0x1c},
    {1, 2, 0x52, 0xa2, 0x1c, 0x1c},
    {0xff, 0xff, 0, 0, 0, 0},
};

ARM void TownMenu_TOWN::menuSetup()
{
    setIconVisibility(0, 0, 0, 0);
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.enableSE_ = 0;
    selectedIcon_ = -1;
    enable_ = 1;
    setupWait_ = 2;
    helpType_ = 0;
}

ARM void TownMenu_TOWN::menuExecute()
{
    if (g_TownMenuMessage.isOpen()) {
        enable_ = 0;
    }
    if (!enable_) {
        return;
    }
    if (setupWait_) {
        if (--setupWait_) {
            return;
        }
        unkfunc_02027424();
        redraw_ = 1;
        return;
    }
    menuItem_.setMenuItem(s_menuItemData, 5, 1, 5);
}

ARM void TownMenu_TOWN::menuDraw()
{
    if (!enable_) {
        return;
    }
    if (setupWait_) {
        return;
    }
    if (showCamera_) {
        drawMenuPartsPlain(s_cameraParts, NULL);
    }
    if (showMap_) {
        drawMenuPartsPlain(s_mapParts, NULL);
    }
    if (showShop_) {
        drawMenuPartsPlain(s_shopParts, NULL);
    }
    if (showMenuIcon_) {
        drawMenuPartsPlain(s_menuIconParts, NULL);
    }
    menuItem_.drawActive();
    if (!helpType_) {
        return;
    }
    const char* text[2];
    text[0] = "\xff\xfe\xac$";
    if (helpType_ == 1) {
        text[1] = "@\x83\x7d\x83\x62\x83\x76\x82\xb6\x82\xe5\x82\xa4\x82\xd9\x82\xa4";  // "@マップじょうほう" (map information)
    }
    if (helpType_ == 2) {
        text[1] = "@\x83\x56\x83\x87\x83\x62\x83\x76\x82\xa2\x82\xbf\x82\xe7\x82\xf1";  // "@ショップいちらん" (shop list)
    }
    drawMenuPartsPlain(s_messageParts, (int*)text);
}

ARM void TownMenu_TOWN::menuUpdate()
{
    if (!enable_) {
        return;
    }
    if (setupWait_) {
        return;
    }
    menuItem_.result_ = 0;
    menuItem_.lastresult_ = 0;
    menuItem_.execInput();
    int select = menuItem_.unk_34;
    selectedIcon_ = -1;
    switch (select) {
    case 0:
        if (showCamera_) {
            selectedIcon_ = 0;
        }
        break;
    case 1:
        if (showCamera_) {
            selectedIcon_ = 1;
        }
        break;
    case 2:
        if (showMap_) {
            selectedIcon_ = 2;
        }
        break;
    case 3:
        if (showShop_) {
            selectedIcon_ = 3;
        }
        break;
    case 4:
        if (showMenuIcon_) {
            selectedIcon_ = 4;
        }
        break;
    }
}

ARM void TownMenu_TOWN::setIconVisibility(int camera, int map, int shop, int menuIcon)
{
    showCamera_ = camera;
    showMap_ = shop;
    showShop_ = map;
    showMenuIcon_ = menuIcon;
    switch (g_Option.getButton()) {
    case 0:
        break;
    case 1:
        showCamera_ = 0;
        break;
    case 2:
        showCamera_ = 0;
        showMap_ = 0;
        showShop_ = 0;
        showMenuIcon_ = 0;
        break;
    case 3:
        showCamera_ = 0;
        showMap_ = 0;
        showShop_ = 0;
        break;
    }
}

ARM void TownMenu_TOWN::unkfunc_02027424()
{
}

ARM void TownMenu_TOWN::menuClose()
{
}
