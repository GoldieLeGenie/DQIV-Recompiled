#include "main/menu/TownMenu_TOWN.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/status/OptionStatus.hpp"

static UnkMenuParts s_menuIconParts[] = {
    {0x13, 0, 0, 5, 0x50, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static UnkMenuParts s_shopParts[] = {
    {0x13, 0, 0, 3, 0xc0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static UnkMenuParts s_mapParts[] = {
    {0x13, 0, 0, 2, 0xa0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static UnkMenuParts s_cameraParts[] = {
    {0x13, 0, 0, 0, 0, 0xa0, 0, 0},
    {0x13, 0, 0, 1, 0xe0, 0xa0, 0, 0},
    {0xff, 0, 0, 0, 0, 0, 0, 0},
};
static UnkMenuParts s_messageParts[] = {
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
    unkfunc_020273a8(0, 0, 0, 0);
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.enableSE_ = 0;
    unk_80 = -1;
    unk_98 = 1;
    unk_9c = 2;
    unk_94 = 0;
}

ARM void TownMenu_TOWN::menuExecute()
{
    if (data_020ed1bc.isOpen()) {
        unk_98 = 0;
    }
    if (!unk_98) {
        return;
    }
    if (unk_9c) {
        if (--unk_9c) {
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
    if (!unk_98) {
        return;
    }
    if (unk_9c) {
        return;
    }
    if (unk_84) {
        unkfunc_02050ea8(s_cameraParts, NULL);
    }
    if (unk_88) {
        unkfunc_02050ea8(s_mapParts, NULL);
    }
    if (unk_8c) {
        unkfunc_02050ea8(s_shopParts, NULL);
    }
    if (unk_90) {
        unkfunc_02050ea8(s_menuIconParts, NULL);
    }
    menuItem_.drawActive();
    if (!unk_94) {
        return;
    }
    const char* text[2];
    text[0] = "\xff\xfe\xac$";
    if (unk_94 == 1) {
        text[1] = "@\x83\x7d\x83\x62\x83\x76\x82\xb6\x82\xe5\x82\xa4\x82\xd9\x82\xa4";  // "@マップじょうほう" (map information)
    }
    if (unk_94 == 2) {
        text[1] = "@\x83\x56\x83\x87\x83\x62\x83\x76\x82\xa2\x82\xbf\x82\xe7\x82\xf1";  // "@ショップいちらん" (shop list)
    }
    unkfunc_02050ea8(s_messageParts, (int*)text);
}

ARM void TownMenu_TOWN::menuUpdate()
{
    if (!unk_98) {
        return;
    }
    if (unk_9c) {
        return;
    }
    menuItem_.result_ = 0;
    menuItem_.lastresult_ = 0;
    menuItem_.execInput();
    int select = menuItem_.unk_34;
    unk_80 = -1;
    switch (select) {
    case 0:
        if (unk_84) {
            unk_80 = 0;
        }
        break;
    case 1:
        if (unk_84) {
            unk_80 = 1;
        }
        break;
    case 2:
        if (unk_88) {
            unk_80 = 2;
        }
        break;
    case 3:
        if (unk_8c) {
            unk_80 = 3;
        }
        break;
    case 4:
        if (unk_90) {
            unk_80 = 4;
        }
        break;
    }
}

ARM void TownMenu_TOWN::unkfunc_020273a8(int camera, int map, int shop, int menuIcon)
{
    unk_84 = camera;
    unk_88 = shop;
    unk_8c = map;
    unk_90 = menuIcon;
    switch (g_Option.getButton()) {
    case 0:
        break;
    case 1:
        unk_84 = 0;
        break;
    case 2:
        unk_84 = 0;
        unk_88 = 0;
        unk_8c = 0;
        unk_90 = 0;
        break;
    case 3:
        unk_84 = 0;
        unk_88 = 0;
        unk_8c = 0;
        break;
    }
}

ARM void TownMenu_TOWN::unkfunc_02027424()
{
}

ARM void TownMenu_TOWN::menuClose()
{
}
