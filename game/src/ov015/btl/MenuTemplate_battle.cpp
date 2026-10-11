#pragma ipa file
#include "ov015/btl/MenuTemplate_battle.hpp"
#include "ov015/btl/BattleMenu.hpp"

THUMB void MenuTemplate_battle::BATTLE_CANCEL(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[] = {
        {2, -1, 0xde, 0x9e, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_battle::BATTLE_FIGHT_CANCEL(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[] = {
        {2, -1, 0xde, 0x86, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_battle::BATTLE_MENUICON2x2(menu::MenuItem* menuitem, int num, int active)
{
    static MENUITEM_DATA menuA[] = {
        {1, 2, 0x40, 0x80, 0x48, 0x20},
        {1, 2, 0x88, 0x80, 0x48, 0x20},
        {1, 2, 0x40, 0xa0, 0x48, 0x20},
        {1, 2, 0x88, 0xa0, 0x48, 0x20},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menuB[] = {
        {1, 2, 0x40, 0x80, 0x48, 0x20},
        {1, 2, 0x88, 0x80, 0x48, 0x20},
        {-1, -1, 0, 0, 0, 0},
    };
    if (num < 4) {
        menuitem->setMenuItem(menuB, 2, 2, num);
    } else {
        menuitem->setMenuItem(menuA, 2, 2, 4);
    }
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_MAGIC_2x3(menu::MenuItem* menuitem, int num)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x14, 0x0c, 0x68, 0x18},
        {1, 2, 0x8c, 0x0c, 0x68, 0x18},
        {1, 2, 0x14, 0x2c, 0x68, 0x18},
        {1, 2, 0x8c, 0x2c, 0x68, 0x18},
        {1, 2, 0x14, 0x4c, 0x68, 0x18},
        {1, 2, 0x8c, 0x4c, 0x68, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 3, num);
}

THUMB void MenuTemplate_battle::BATTLE_TACTICS_2x3(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu_tc[] = {
        {1, 2, 0x24, 0x60, 0x36, 0x18},
        {1, 2, 0x64, 0x60, 0x36, 0x18},
        {1, 2, 0xa4, 0x60, 0x36, 0x18},
        {1, 2, 0x24, 0x80, 0x36, 0x18},
        {1, 2, 0x64, 0x80, 0x36, 0x18},
        {1, 2, 0xa4, 0x80, 0x36, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_tc, 3, 2, 6);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_PARTY_2x2(menu::MenuItem* menuitem, int num)
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x0c, 0x14, 0x70, 0x38},
        {1, 2, 0x84, 0x14, 0x70, 0x38},
        {1, 2, 0x0c, 0x54, 0x70, 0x38},
        {1, 2, 0x84, 0x54, 0x70, 0x38},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 2, num);
}

THUMB void MenuTemplate_battle::BATTLE_ARRAYMENU_2x1(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0x26, 0x86, 0x4e, 0x0e},
        {1, 2, 0x86, 0x86, 0x4e, 0x0e},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_array, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_TACTICSCHANGE_5x1(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0x26, 0x76, 0x1c, 0x1c},
        {1, 2, 0x4e, 0x76, 0x1c, 0x1c},
        {1, 2, 0x76, 0x76, 0x1c, 0x1c},
        {1, 2, 0x9e, 0x76, 0x1c, 0x1c},
        {1, 2, 0xc6, 0x76, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_array, 5, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_ARRAYCHANGE_5x2(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0x36, 0x76, 0x1c, 0x1c},
        {1, 2, 0x5e, 0x76, 0x1c, 0x1c},
        {1, 2, 0x86, 0x76, 0x1c, 0x1c},
        {1, 2, 0xae, 0x76, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_array, 4, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_ARRAYCHANGE_TO_5x2(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0x16, 0x76, 0x1c, 0x1c},
        {1, 2, 0x3e, 0x76, 0x1c, 0x1c},
        {1, 2, 0x66, 0x76, 0x1c, 0x1c},
        {1, 2, 0x8e, 0x76, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_array, 4, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_ARRAYALL_5x2(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0x16, 0x6e, 0x1c, 0x1c},
        {1, 2, 0x3e, 0x6e, 0x1c, 0x1c},
        {1, 2, 0x66, 0x6e, 0x1c, 0x1c},
        {1, 2, 0x8e, 0x6e, 0x1c, 0x1c},
        {1, 2, 0xb6, 0x6e, 0x1c, 0x1c},
        {1, 2, 0x16, 0x96, 0x1c, 0x1c},
        {1, 2, 0x3e, 0x96, 0x1c, 0x1c},
        {1, 2, 0x66, 0x96, 0x1c, 0x1c},
        {1, 2, 0x8e, 0x96, 0x1c, 0x1c},
        {1, 2, 0xb6, 0x96, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu_array, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_ICON_ENEMY(menu::MenuItem* menuitem, int active, TOUCHRECT* touchRect, int enemyMaxNum)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < enemyMaxNum; i++) {
        if (i < enemyMaxNum) {
            menu_array[i].x = touchRect[i].x;
            menu_array[i].y = touchRect[i].y;
            menu_array[i].w = touchRect[i].width;
            menu_array[i].h = touchRect[i].height;
        }
    }
    menuitem->setMenuItem(menu_array, 12, 1, enemyMaxNum);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_battle::BATTLE_RECT_ENEMY(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu_array[] = {
        {1, 2, 0, 0, 0, 0},
        {-1, -1, 0, 0, 0, 0},
    };
    menu_array[0].x = 0;
    menu_array[0].y = 0x20;
    menu_array[0].w = 0x100;
    menu_array[0].h = 0x60;
    menuitem->setMenuItem(menu_array, 1, 1, 1);
}

THUMB void MenuTemplate_battle::BATTLE_ITEM_ICON32_2x3(menu::MenuItem* menuitem, int itemMaxCount, int active)
{
    const int itemBaseX = 28;
    const int itemBaseY = 12;
    const int itemX = 120;
    const int itemY = 32;
    const int itemW = 96;
    const int itemH = 24;
    static MENUITEM_DATA menu[] = {
        {1, 2, itemBaseX, itemBaseY, itemW, itemH},
        {1, 2, itemBaseX + itemX, itemBaseY, itemW, itemH},
        {1, 2, itemBaseX, itemBaseY + itemY, itemW, itemH},
        {1, 2, itemBaseX + itemX, itemBaseY + itemY, itemW, itemH},
        {1, 2, itemBaseX, itemBaseY + itemY * 2, itemW, itemH},
        {1, 2, itemBaseX + itemX, itemBaseY + itemY * 2, itemW, itemH},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 3, itemMaxCount);
    menuitem->active_ = active;
}
