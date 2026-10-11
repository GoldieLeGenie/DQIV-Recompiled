#pragma ipa file
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"

THUMB void MenuTemplate_town::TOWN_CANCEL(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[] = {
        {2, -1, 0xde, 0x9e, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_town::TOWN_OPERATION(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu_op[] = {
        {1, 2, 0x20, 0x24, 0x56, 0x10},
        {1, 2, 0x80, 0x24, 0x56, 0x10},
        {1, 2, 0x20, 0x3c, 0x56, 0x10},
        {1, 2, 0x80, 0x3c, 0x56, 0x10},
        {1, 2, 0x20, 0x54, 0x56, 0x10},
        {1, 2, 0x80, 0x54, 0x56, 0x10},
        {1, 2, 0x20, 0x6c, 0x56, 0x10},
        {1, 2, 0x80, 0x6c, 0x56, 0x10},
        {1, 2, 0x20, 0x84, 0x56, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_op, 2, 5, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::TOWN_ICON32_5x2(menu::MenuItem* menuitem, int active, int max)
{
    const int ICON5x2_LEFT = 20;
    const int ICON5x2_TOP = 116;
    const int BLANK_X = 8;
    const int BLANK_Y = 16;
    const int OFFSET_X = 32;
    const int OFFSET_Y = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::TOWN_OP_BGMVOL(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x58, 0x2a, 0x10, 0x10},
        {1, 2, 0x78, 0x2a, 0x10, 0x10},
        {1, 2, 0x98, 0x2a, 0x10, 0x10},
        {1, 2, 0xb8, 0x2a, 0x10, 0x10},
        {1, 2, 0xd8, 0x2a, 0x10, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, 5);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::TOWN_OP_EFFECTVOL(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x58, 0x42, 0x10, 0x10},
        {1, 2, 0x78, 0x42, 0x10, 0x10},
        {1, 2, 0x98, 0x42, 0x10, 0x10},
        {1, 2, 0xb8, 0x42, 0x10, 0x10},
        {1, 2, 0xd8, 0x42, 0x10, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, 5);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::TOWN_OP_BATTLE(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x58, 0x8a, 0x10, 0x10},
        {1, 2, 0x78, 0x8a, 0x10, 0x10},
        {1, 2, 0x98, 0x8a, 0x10, 0x10},
        {1, 2, 0xb8, 0x8a, 0x10, 0x10},
        {1, 2, 0xd8, 0x8a, 0x10, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, 5);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::TOWN_OP_NOEQUIP(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x14, 0x6c, 0x68, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_town::TOWN_CAREER_SWITCH(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    menu[0].x = 0xb8;
    menu[0].y = 0xa0;
    menu[0].w = 0x18;
    menu[0].h = 0x18;
    menuitem->setMenuItem(menu, 1, 1, 1);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuRootIcon(menu::MenuItem* menuitem, int active)
{
    const int rootX = 64;
    const int rootY = 96;
    const int rootW = 72;
    const int rootH = 32;
    static MENUITEM_DATA menu[] = {
        {1, 2, rootX, rootY, rootW, rootH},
        {1, 2, rootX + rootW, rootY, rootW, rootH},
        {1, 2, rootX, rootY + rootH, rootW, rootH},
        {1, 2, rootX + rootW, rootY + rootH, rootW, rootH},
        {1, 2, rootX, rootY + rootH * 2, rootW, rootH},
        {1, 2, rootX + rootW, rootY + rootH * 2, rootW, rootH},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 3, 6);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuSelectCharaIcon(menu::MenuItem* menuitem, int max, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x14, 0x80, 0x20, 0x18},
        {1, 2, 0x3c, 0x80, 0x20, 0x18},
        {1, 2, 0x64, 0x80, 0x20, 0x18},
        {1, 2, 0x8c, 0x80, 0x20, 0x18},
        {1, 2, 0xb4, 0x80, 0x20, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuSelectMagic(menu::MenuItem* menuitem, int max, int active)
{
    const int X = 28;
    const int Y = 12;
    const int W = 96;
    const int H = 16;
    const int offsetX = 120;
    const int offsetY = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, X, Y, W, H},
        {1, 2, X + offsetX, Y, W, H},
        {1, 2, X, Y + offsetY, W, H},
        {1, 2, X + offsetX, Y + offsetY, W, H},
        {1, 2, X, Y + offsetY * 2, W, H},
        {1, 2, X + offsetX, Y + offsetY * 2, W, H},
        {1, 2, X, Y + offsetY * 3, W, H},
        {1, 2, X + offsetX, Y + offsetY * 3, W, H},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 4, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuItemCommand(menu::MenuItem* menuitem, int flagItemCommand, int active)
{
    static MENUITEM_DATA menuALL[] = {
        {1, 2, 0x10, 0x70, 0x40, 0x10},
        {1, 2, 0x60, 0x70, 0x40, 0x10},
        {1, 2, 0xb0, 0x70, 0x40, 0x10},
        {1, 2, 0x10, 0x88, 0x40, 0x10},
        {1, 2, 0x60, 0x88, 0x40, 0x10},
        {1, 2, 0xb0, 0x88, 0x40, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menuNone[] = {
        {1, 2, 0x10, 0x70, 0x40, 0x10},
        {1, 2, 0x60, 0x70, 0x40, 0x10},
        {1, 2, 0xb0, 0x70, 0x40, 0x10},
        {1, 2, 0x10, 0x88, 0x40, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menuOnlyEq[] = {
        {1, 2, 0x10, 0x70, 0x40, 0x10},
        {1, 2, 0x60, 0x70, 0x40, 0x10},
        {1, 2, 0xb0, 0x70, 0x40, 0x10},
        {1, 2, 0x10, 0x88, 0x40, 0x10},
        {1, 2, 0x60, 0x88, 0x40, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menuOnlyShow[] = {
        {1, 2, 0x10, 0x70, 0x40, 0x10},
        {1, 2, 0x60, 0x70, 0x40, 0x10},
        {1, 2, 0xb0, 0x70, 0x40, 0x10},
        {1, 2, 0x10, 0x88, 0x40, 0x10},
        {1, 2, 0x60, 0x88, 0x40, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    switch (flagItemCommand) {
        case 0:
            menuitem->setMenuItem(menuNone, 3, 2, 4);
            menuitem->active_ = active;
            break;
        case 1:
            menuitem->setMenuItem(menuOnlyEq, 3, 2, 5);
            menuitem->active_ = active;
            break;
        case 2:
            menuitem->setMenuItem(menuOnlyShow, 3, 2, 5);
            menuitem->active_ = active;
            break;
        case 3:
            menuitem->setMenuItem(menuALL, 3, 2, 6);
            menuitem->active_ = active;
            break;
    }
}

THUMB void MenuTemplate_town::townMenuTacticsSelectChara(menu::MenuItem* menuitem, int max, int active, int x, int y)
{
    const int ICON5x2_LEFT = 20;
    const int ICON5x2_TOP = 88;
    const int BLANK_X = 8;
    const int BLANK_Y = 16;
    const int OFFSET_X = 32;
    const int OFFSET_Y = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {-1, -1, 0, 0, 0, 0},
    };
    if (x != -1 && y != -1) {
        for (int i = 0; i < 10; i++) {
            unkfunc_020518f8(&menu[i], x, y);
        }
    }
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuTacticsSelectHalfChara(menu::MenuItem* menuitem, int max, int active, int x, int y)
{
    const int ICON5x2_LEFT = 20;
    const int ICON5x2_TOP = 128;
    const int BLANK_X = 8;
    const int BLANK_Y = 16;
    const int OFFSET_X = 32;
    const int OFFSET_Y = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {-1, -1, 0, 0, 0, 0},
    };
    if (x != -1 && y != -1) {
        for (int i = 0; i < 10; i++) {
            unkfunc_020518f8(&menu[i], x, y);
        }
    }
    menuitem->setMenuItem(menu, 5, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuItemSelectChara(menu::MenuItem* menuitem, int max, int active)
{
    const int ICON5x2_LEFT = 20;
    const int ICON5x2_TOP = 88;
    const int BLANK_X = 8;
    const int BLANK_Y = 16;
    const int OFFSET_X = 32;
    const int OFFSET_Y = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP + (OFFSET_Y + BLANK_Y), OFFSET_X, OFFSET_Y},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuItemSelectHalfChara(menu::MenuItem* menuitem, int max, int active)
{
    const int ICON5x2_LEFT = 20;
    const int ICON5x2_TOP = 128;
    const int BLANK_X = 8;
    const int BLANK_Y = 16;
    const int OFFSET_X = 32;
    const int OFFSET_Y = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, ICON5x2_LEFT, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X), ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 2, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 3, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {1, 2, ICON5x2_LEFT + (OFFSET_X + BLANK_X) * 4, ICON5x2_TOP, OFFSET_X, OFFSET_Y},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuTacticsSelectTC(menu::MenuItem* menuitem, int active)
{
    const int X = 28;
    const int Y = 8;
    const int W = 56;
    const int H = 24;
    const int offsetX = 64;
    const int offsetY = 32;
    static MENUITEM_DATA menu[] = {
        {1, 2, X, Y, W, H},
        {1, 2, X + offsetX, Y, W, H},
        {1, 2, X + offsetX * 2, Y, W, H},
        {1, 2, X, Y + offsetY, W, H},
        {1, 2, X + offsetX, Y + offsetY, W, H},
        {1, 2, X + offsetX * 2, Y + offsetY, W, H},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 3, 2, 6);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuPageTargetChara(menu::MenuItem* menuitem, int max, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0xc, 0xc, 0x70, 0x38},
        {1, 2, 0x84, 0xc, 0x70, 0x38},
        {1, 2, 0xc, 0x4c, 0x70, 0x38},
        {1, 2, 0x84, 0x4c, 0x70, 0x38},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuTacticsBoxUp(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x10, 0x88, 0x56, 0x10},
        {1, 2, 0x70, 0x88, 0x56, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuPageRightArrow(menu::MenuItem* menuitem, int x, int y)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0xe6, 0x86, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    unkfunc_020518f8(menu, x, y);
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_town::townMenuPageRightTwoArrow(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0xc6, 0x86, 0x14, 0x14},
        {1, 2, 0xe6, 0x86, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_town::townMenuPageCenter(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x56, 0x86, 0x14, 0x14},
        {1, 2, 0x96, 0x86, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}
