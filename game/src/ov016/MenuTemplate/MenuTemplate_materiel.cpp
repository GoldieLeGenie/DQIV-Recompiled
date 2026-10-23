#pragma ipa file
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#ifdef DQIV_NATIVE
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#endif

THUMB void MenuTemplate_materiel::MATERIEL_SHOP_ROOT(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[4] = {
        {1, 2, 0xa4, 8, 0x54, 0x10},
        {1, 2, 0xa4, 0x20, 0x54, 0x10},
        {1, 2, 0xa4, 0x38, 0x54, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 3, 3);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_CHURCH_ROOT(menu::MenuItem* menuitem, int maxCommand, int active)
{
    const int x = 68, y = 12, ox = 96, oy = 24, w = 80, h = 16;
    static MENUITEM_DATA menu[7] = {
        {1, 2, x, y, w, h},
        {1, 2, x + ox, y, w, h},
        {1, 2, x, y + oy, w, h},
        {1, 2, x + ox, y + oy, w, h},
        {1, 2, x, y + oy * 2, w, h},
        {1, 2, x + ox, y + oy * 2, w, h},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 3, maxCommand);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_ICON32_5x2_CHURCH(menu::MenuItem* menuitem, int active, int max)
{
    const int X = 20;
    const int Y = 44;
    const int W = 32;
    const int H = 24;
    const int OFFSET_X = 40;
    const int OFFSET_Y = 40;
    static MENUITEM_DATA menu[11] = {
        {1, 2, X, Y, W, H},
        {1, 2, X + OFFSET_X, Y, W, H},
        {1, 2, X + OFFSET_X * 2, Y, W, H},
        {1, 2, X + OFFSET_X * 3, Y, W, H},
        {1, 2, X + OFFSET_X * 4, Y, W, H},
        {1, 2, X, Y + OFFSET_Y, W, H},
        {1, 2, X + OFFSET_X, Y + OFFSET_Y, W, H},
        {1, 2, X + OFFSET_X * 2, Y + OFFSET_Y, W, H},
        {1, 2, X + OFFSET_X * 3, Y + OFFSET_Y, W, H},
        {1, 2, X + OFFSET_X * 4, Y + OFFSET_Y, W, H},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_BET_COIN(menu::MenuItem* menuitem, int active, int max, int x, int y)
{
    int tab = x - (max - 1) * 6 + 2;
    static MENUITEM_DATA menu[7] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < 6; i++) {
        menu[i].h = 14;
        menu[i].w = 8;
        menu[i].x = tab + i * 6;
        menu[i].y = y;
    }
    menuitem->setMenuItem(menu, 6, 1, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_YESNO_BUTTON(menu::MenuItem* menuitem, int active, int y)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0x20, 0, 0x58, 0x18},
        {1, 2, 0x88, 0, 0x58, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menu[0].y = y;
    menu[1].y = y;
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_POKER_SELECT_CARD(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[6] = {
        {1, 2, 0xc, 0x7a, 0x21, 0x10},
        {1, 2, 0x3c, 0x7a, 0x21, 0x10},
        {1, 2, 0x6c, 0x7a, 0x21, 0x10},
        {1, 2, 0x9c, 0x7a, 0x21, 0x10},
        {1, 2, 0xcc, 0x7a, 0x21, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 1, 5);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_POKER_SELECT_DEAL(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[2] = {
        {1, 2, 0x6d, 0x90, 0x21, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_POKER_BET_COIN(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[2] = {
        {1, 2, 0xf0, 0x18, 0xa, 0xa},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_POKER_HI_AND_LOW(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0x70, 0x90, 0x20, 0x10},
        {1, 2, 0x70, 0xa8, 0x20, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 2, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_MONSTER_LIST(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[5] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < 4; i++) {
        menu[i].x = 12;
        menu[i].y = 14 + i * 24;
        menu[i].w = 162;
        menu[i].h = 16;
    }
    menuitem->setMenuItem(menu, 1, 4, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_GET_UPDOWN(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
}

THUMB void MenuTemplate_materiel::MATERIEL_DIARY_MENU(menu::MenuItem* menuitem, int num)
{
    static MENUITEM_DATA menu3[4] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menu4[5] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    if (num == 3) {
        for (int i = 0; i < 3; i++) {
            menu3[i].x = 18;
            menu3[i].y = 16 + i * 14;
            menu3[i].w = 140;
            menu3[i].h = 12;
        }
        menuitem->setMenuItem(menu3, 1, 3, 3);
    }
    if (num == 4) {
        for (int i = 0; i < 4; i++) {
            menu4[i].x = 18;
            menu4[i].y = 16 + i * 14;
            menu4[i].w = 140;
            menu4[i].h = 12;
        }
        menuitem->setMenuItem(menu4, 1, 4, 4);
    }
#ifdef DQIV_NATIVE
    // The settings, achievements and quit entries after the original ones.
    static MENUITEM_DATA menu6[8];
    if (num >= 5 && num <= 7) {
        static const MENUITEM_DATA end = {-1, -1, 0, 0, 0, 0};
        for (int i = 0; i < num; i++) {
            menu6[i] = end;
            menu6[i].code = 1;
            menu6[i].view = 2;
            menu6[i].x = 18;
            menu6[i].y = 16 + i * 14;
            menu6[i].w = 140;
            menu6[i].h = 12;
        }
        menu6[num] = end;
        menuitem->setMenuItem(menu6, 1, num, num);
    }
#endif
}

#ifdef DQIV_NATIVE
THUMB void MenuTemplate_materiel::MATERIEL_SETTINGS(menu::MenuItem* menuitem, int num, int step)
{
    static MENUITEM_DATA item[2] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    static MENUITEM_DATA menu[16];
    for (int i = 0; i < num && i < 15; i++) {
        menu[i] = item[0];
        menu[i].x = 18;
        menu[i].y = 32 + i * step;
        menu[i].w = 214;
        menu[i].h = 12;
    }
    menu[num] = item[1];
    menuitem->setMenuItem(menu, 1, num, num);
}

THUMB void MenuTemplate_materiel::MATERIEL_CELECT_BUY_TABS(menu::MenuItem* menuitem, int active, int max)
{
    // MATERIEL_CELECT_BUY's rows, below the category tabs.
    static MENUITEM_DATA menu[7];
    for (int i = 0; i < 6; i++) {
        menu[i] = {1, 2, 0x10, (short)unkfunc_native_shopRowY(i), 0xe8, 0x10};
    }
    menu[6] = {0xff, 0xff, 0, 0, 0, 0};
    menuitem->setMenuItem(menu, 1, 6, max);
    menuitem->active_ = active;
}
#endif

THUMB void MenuTemplate_materiel::MATERIEL_DIARY_SELECT(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[4] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < 3; i++) {
        menu[i].x = 18;
        menu[i].y = 34 + i * 26;
        menu[i].w = 224;
        menu[i].h = 12;
    }
    menuitem->setMenuItem(menu, 1, 3, 3);
}

THUMB void MenuTemplate_materiel::MATERIEL_DIARY_LOAD(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[4] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < 3; i++) {
        menu[i].x = 18;
        menu[i].y = 90 + i * 26;
        menu[i].w = 224;
        menu[i].h = 12;
    }
    menuitem->setMenuItem(menu, 1, 3, 3);
}

THUMB void MenuTemplate_materiel::MATERIEL_KEYBOARD_JAP(menu::MenuItem* menuitem)
{
    static const int keyX[11] = {19, 35, 51, 67, 83, 107, 123, 139, 155, 171, 195};
    static int keyY[6] = {85, 97, 109, 121, 133, 145};
    static MENUITEM_DATA menu[67] = {
        {1, 2, keyX[0], keyY[0] - 1, 14, 12},
        {1, 2, keyX[1], keyY[0] - 1, 14, 12},
        {1, 2, keyX[2], keyY[0] - 1, 14, 12},
        {1, 2, keyX[3], keyY[0] - 1, 14, 12},
        {1, 2, keyX[4], keyY[0] - 1, 14, 12},
        {1, 2, keyX[5], keyY[0] - 1, 14, 12},
        {1, 2, keyX[6], keyY[0] - 1, 14, 12},
        {1, 2, keyX[7], keyY[0] - 1, 14, 12},
        {1, 2, keyX[8], keyY[0] - 1, 14, 12},
        {1, 2, keyX[9], keyY[0] - 1, 14, 12},
        {1, 2, keyX[10], keyY[0] - 1, 14, 12},
        {1, 2, keyX[0], keyY[1] - 1, 14, 12},
        {1, 2, keyX[1], keyY[1] - 1, 14, 12},
        {1, 2, keyX[2], keyY[1] - 1, 14, 12},
        {1, 2, keyX[3], keyY[1] - 1, 14, 12},
        {1, 2, keyX[4], keyY[1] - 1, 14, 12},
        {1, 2, keyX[5], keyY[1] - 1, 14, 12},
        {1, 2, keyX[6], keyY[1] - 1, 14, 12},
        {1, 2, keyX[7], keyY[1] - 1, 14, 12},
        {1, 2, keyX[8], keyY[1] - 1, 14, 12},
        {1, 2, keyX[9], keyY[1] - 1, 14, 12},
        {1, 2, keyX[10], keyY[1] - 1, 14, 12},
        {1, 2, keyX[0], keyY[2] - 1, 14, 12},
        {1, 2, keyX[1], keyY[2] - 1, 14, 12},
        {1, 2, keyX[2], keyY[2] - 1, 14, 12},
        {1, 2, keyX[3], keyY[2] - 1, 14, 12},
        {1, 2, keyX[4], keyY[2] - 1, 14, 12},
        {1, 2, keyX[5], keyY[2] - 1, 14, 12},
        {1, 2, keyX[6], keyY[2] - 1, 14, 12},
        {1, 2, keyX[7], keyY[2] - 1, 14, 12},
        {1, 2, keyX[8], keyY[2] - 1, 14, 12},
        {1, 2, keyX[9], keyY[2] - 1, 14, 12},
        {1, 2, keyX[10], keyY[2] - 1, 14, 12},
        {1, 2, keyX[0], keyY[3] - 1, 14, 12},
        {1, 2, keyX[1], keyY[3] - 1, 14, 12},
        {1, 2, keyX[2], keyY[3] - 1, 14, 12},
        {1, 2, keyX[3], keyY[3] - 1, 14, 12},
        {1, 2, keyX[4], keyY[3] - 1, 14, 12},
        {1, 2, keyX[5], keyY[3] - 1, 14, 12},
        {1, 2, keyX[6], keyY[3] - 1, 14, 12},
        {1, 2, keyX[7], keyY[3] - 1, 14, 12},
        {1, 2, keyX[8], keyY[3] - 1, 14, 12},
        {1, 2, keyX[9], keyY[3] - 1, 14, 12},
        {1, 2, keyX[10], keyY[3] - 1, 14, 12},
        {1, 2, keyX[0], keyY[4] - 1, 14, 12},
        {1, 2, keyX[1], keyY[4] - 1, 14, 12},
        {1, 2, keyX[2], keyY[4] - 1, 14, 12},
        {1, 2, keyX[3], keyY[4] - 1, 14, 12},
        {1, 2, keyX[4], keyY[4] - 1, 14, 12},
        {1, 2, keyX[5], keyY[4] - 1, 14, 12},
        {1, 2, keyX[6], keyY[4] - 1, 14, 12},
        {1, 2, keyX[7], keyY[4] - 1, 14, 12},
        {1, 2, keyX[8], keyY[4] - 1, 14, 12},
        {1, 2, keyX[9], keyY[4] - 1, 14, 12},
        {1, 2, keyX[10], keyY[4] - 1, 14, 12},
        {1, 2, keyX[0], keyY[5] - 1, 14, 12},
        {1, 2, keyX[1], keyY[5] - 1, 14, 12},
        {1, 2, keyX[2], keyY[5] - 1, 14, 12},
        {1, 2, keyX[3], keyY[5] - 1, 14, 12},
        {1, 2, keyX[4], keyY[5] - 1, 14, 12},
        {1, 2, keyX[5], keyY[5] - 1, 14, 12},
        {1, 2, keyX[6], keyY[5] - 1, 14, 12},
        {1, 2, keyX[7], keyY[5] - 1, 14, 12},
        {1, 2, keyX[8], keyY[5] - 1, 14, 12},
        {1, 2, keyX[9], keyY[5] - 1, 14, 12},
        {1, 2, keyX[10], keyY[5] - 1, 14, 12},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 11, 6, 66);
}

THUMB void MenuTemplate_materiel::MATERIEL_KEYBOARD_JAP_MESSAGE(menu::MenuItem* menuitem)
{
    static const int keyX[11] = {19, 35, 51, 67, 83, 107, 123, 139, 155, 171, 195};
    static int keyY[6] = {109, 121, 133, 145, 157, 169};
    static MENUITEM_DATA menu[67] = {
        {1, 2, keyX[0], keyY[0] - 1, 14, 12},
        {1, 2, keyX[1], keyY[0] - 1, 14, 12},
        {1, 2, keyX[2], keyY[0] - 1, 14, 12},
        {1, 2, keyX[3], keyY[0] - 1, 14, 12},
        {1, 2, keyX[4], keyY[0] - 1, 14, 12},
        {1, 2, keyX[5], keyY[0] - 1, 14, 12},
        {1, 2, keyX[6], keyY[0] - 1, 14, 12},
        {1, 2, keyX[7], keyY[0] - 1, 14, 12},
        {1, 2, keyX[8], keyY[0] - 1, 14, 12},
        {1, 2, keyX[9], keyY[0] - 1, 14, 12},
        {1, 2, keyX[10], keyY[0] - 1, 14, 12},
        {1, 2, keyX[0], keyY[1] - 1, 14, 12},
        {1, 2, keyX[1], keyY[1] - 1, 14, 12},
        {1, 2, keyX[2], keyY[1] - 1, 14, 12},
        {1, 2, keyX[3], keyY[1] - 1, 14, 12},
        {1, 2, keyX[4], keyY[1] - 1, 14, 12},
        {1, 2, keyX[5], keyY[1] - 1, 14, 12},
        {1, 2, keyX[6], keyY[1] - 1, 14, 12},
        {1, 2, keyX[7], keyY[1] - 1, 14, 12},
        {1, 2, keyX[8], keyY[1] - 1, 14, 12},
        {1, 2, keyX[9], keyY[1] - 1, 14, 12},
        {1, 2, keyX[10], keyY[1] - 1, 14, 12},
        {1, 2, keyX[0], keyY[2] - 1, 14, 12},
        {1, 2, keyX[1], keyY[2] - 1, 14, 12},
        {1, 2, keyX[2], keyY[2] - 1, 14, 12},
        {1, 2, keyX[3], keyY[2] - 1, 14, 12},
        {1, 2, keyX[4], keyY[2] - 1, 14, 12},
        {1, 2, keyX[5], keyY[2] - 1, 14, 12},
        {1, 2, keyX[6], keyY[2] - 1, 14, 12},
        {1, 2, keyX[7], keyY[2] - 1, 14, 12},
        {1, 2, keyX[8], keyY[2] - 1, 14, 12},
        {1, 2, keyX[9], keyY[2] - 1, 14, 12},
        {1, 2, keyX[10], keyY[2] - 1, 14, 12},
        {1, 2, keyX[0], keyY[3] - 1, 14, 12},
        {1, 2, keyX[1], keyY[3] - 1, 14, 12},
        {1, 2, keyX[2], keyY[3] - 1, 14, 12},
        {1, 2, keyX[3], keyY[3] - 1, 14, 12},
        {1, 2, keyX[4], keyY[3] - 1, 14, 12},
        {1, 2, keyX[5], keyY[3] - 1, 14, 12},
        {1, 2, keyX[6], keyY[3] - 1, 14, 12},
        {1, 2, keyX[7], keyY[3] - 1, 14, 12},
        {1, 2, keyX[8], keyY[3] - 1, 14, 12},
        {1, 2, keyX[9], keyY[3] - 1, 14, 12},
        {1, 2, keyX[10], keyY[3] - 1, 14, 12},
        {1, 2, keyX[0], keyY[4] - 1, 14, 12},
        {1, 2, keyX[1], keyY[4] - 1, 14, 12},
        {1, 2, keyX[2], keyY[4] - 1, 14, 12},
        {1, 2, keyX[3], keyY[4] - 1, 14, 12},
        {1, 2, keyX[4], keyY[4] - 1, 14, 12},
        {1, 2, keyX[5], keyY[4] - 1, 14, 12},
        {1, 2, keyX[6], keyY[4] - 1, 14, 12},
        {1, 2, keyX[7], keyY[4] - 1, 14, 12},
        {1, 2, keyX[8], keyY[4] - 1, 14, 12},
        {1, 2, keyX[9], keyY[4] - 1, 14, 12},
        {1, 2, keyX[10], keyY[4] - 1, 14, 12},
        {1, 2, keyX[0], keyY[5] - 1, 14, 12},
        {1, 2, keyX[1], keyY[5] - 1, 14, 12},
        {1, 2, keyX[2], keyY[5] - 1, 14, 12},
        {1, 2, keyX[3], keyY[5] - 1, 14, 12},
        {1, 2, keyX[4], keyY[5] - 1, 14, 12},
        {1, 2, keyX[5], keyY[5] - 1, 14, 12},
        {1, 2, keyX[6], keyY[5] - 1, 14, 12},
        {1, 2, keyX[7], keyY[5] - 1, 14, 12},
        {1, 2, keyX[8], keyY[5] - 1, 14, 12},
        {1, 2, keyX[9], keyY[5] - 1, 14, 12},
        {1, 2, keyX[10], keyY[5] - 1, 14, 12},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 11, 6, 66);
}

THUMB void MenuTemplate_materiel::MATERIEL_SEXUALITY(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0, 0, 0x100, 0xc0},
        {1, 2, 0, 0, 0x100, 0xc0},
        {-1, -1, 0, 0, 0, 0},
    };
    for (int i = 0; i < 3; i++) {
        menu[i].x = 108;
        menu[i].y = 90 + i * 16;
        menu[i].w = 32;
        menu[i].h = 10;
    }
    menuitem->setMenuItem(menu, 1, 2, 2);
}

THUMB void MenuTemplate_materiel::MATERIEL_PAGE_2x1(menu::MenuItem* menuitem, int active, int x, int y)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, -23, 3, 0x14, 0x14},
        {1, 2, 0x10, 3, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_CELECT_BUY(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[7] = {
        {1, 2, 0x10, 0xc, 0xe8, 0x10},
        {1, 2, 0x10, 0x24, 0xe8, 0x10},
        {1, 2, 0x10, 0x3c, 0xe8, 0x10},
        {1, 2, 0x10, 0x54, 0xe8, 0x10},
        {1, 2, 0x10, 0x6c, 0xe8, 0x10},
        {1, 2, 0x10, 0x84, 0xe8, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 6, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_CELECT_SELL(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[8] = {
        {1, 2, 0x48, 0x26, 0xa8, 0xe},
        {1, 2, 0x48, 0x36, 0xa8, 0xe},
        {1, 2, 0x48, 0x46, 0xa8, 0xe},
        {1, 2, 0x48, 0x56, 0xa8, 0xe},
        {1, 2, 0x48, 0x66, 0xa8, 0xe},
        {1, 2, 0x48, 0x76, 0xa8, 0xe},
        {1, 2, 0x48, 0x86, 0xa8, 0xe},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 6, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_ICON32_5x2(menu::MenuItem* menuitem, int active, int max)
{
    const int SHOP_X = 20;
    const int SHOP_Y = 84;
    const int SHOP_W = 32;
    const int SHOP_H = 24;
    const int SHOP_OFFSET_X = 40;
    const int SHOP_OFFSET_Y = 40;
    static MENUITEM_DATA menu[11] = {
        {1, 2, SHOP_X, SHOP_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X, SHOP_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 2, SHOP_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 3, SHOP_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 4, SHOP_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X, SHOP_Y + SHOP_OFFSET_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X, SHOP_Y + SHOP_OFFSET_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 2, SHOP_Y + SHOP_OFFSET_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 3, SHOP_Y + SHOP_OFFSET_Y, SHOP_W, SHOP_H},
        {1, 2, SHOP_X + SHOP_OFFSET_X * 4, SHOP_Y + SHOP_OFFSET_Y, SHOP_W, SHOP_H},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_MONEY_SELECT(menu::MenuItem* menuitem, int active, int digit)
{
    static MENUITEM_DATA menu[7] = {
        {1, 2, 0xb8, 0x59, 0xa, 0xe},
        {1, 2, 0xc2, 0x59, 0xa, 0xe},
        {1, 2, 0xcc, 0x59, 0xa, 0xe},
        {1, 2, 0xd6, 0x59, 0xa, 0xe},
        {1, 2, 0xe0, 0x59, 0xa, 0xe},
        {1, 2, 0xea, 0x59, 0xa, 0xe},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 6, 1, digit);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_COIN_SELECT(menu::MenuItem* menuitem, int active, int digit)
{
    static MENUITEM_DATA menu[7] = {
        {1, 2, 0xb8, 9, 0xa, 0xe},
        {1, 2, 0xc2, 9, 0xa, 0xe},
        {1, 2, 0xcc, 9, 0xa, 0xe},
        {1, 2, 0xd6, 9, 0xa, 0xe},
        {1, 2, 0xe0, 9, 0xa, 0xe},
        {1, 2, 0xea, 9, 0xa, 0xe},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 6, 1, digit);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_CHENGEGIFT_5x2(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[11] = {
        {1, 2, 0x26, 0x3e, 0x1c, 0x1c},
        {1, 2, 0x4e, 0x3e, 0x1c, 0x1c},
        {1, 2, 0x76, 0x3e, 0x1c, 0x1c},
        {1, 2, 0x9e, 0x3e, 0x1c, 0x1c},
        {1, 2, 0xc6, 0x3e, 0x1c, 0x1c},
        {1, 2, 0x26, 0x5e, 0x1c, 0x1c},
        {1, 2, 0x4e, 0x5e, 0x1c, 0x1c},
        {1, 2, 0x76, 0x5e, 0x1c, 0x1c},
        {1, 2, 0x9e, 0x5e, 0x1c, 0x1c},
        {1, 2, 0xc6, 0x5e, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 5, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_PICTUREBOOK_SELECT(menu::MenuItem* menuitem, int active, int max)
{
    const int PICTUREBOOK_RX = 144;
    const int PICTUREBOOK_LX = 32;
    const int PICTUREBOOK_W = 80;
    const int PICTUREBOOK_H = 12;
    static MENUITEM_DATA menu[17] = {
        {1, 2, PICTUREBOOK_LX, 29, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 29, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 45, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 45, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 61, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 61, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 77, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 77, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 93, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 93, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 109, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 109, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 125, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 125, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_LX, 141, PICTUREBOOK_W, PICTUREBOOK_H},
        {1, 2, PICTUREBOOK_RX, 141, PICTUREBOOK_W, PICTUREBOOK_H},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 8, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::MATERIEL_PICTUREBOOK_MONSTERANIME(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[2] = {
        {1, 2, 0xa, 0x2a, 0x80, 0x80},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
    menuitem->active_ = 0;
}

THUMB void MenuTemplate_materiel::MATERIEL_CANCEL(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[2] = {
        {2, -1, 0xe2, 0xa6, 0x1c, 0x1c},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
}

THUMB void MenuTemplate_materiel::shopPlayerArrow(menu::MenuItem* menuitem)
{
    static MENUITEM_DATA menu[2] = {
        {1, 2, 0xe2, 0x82, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
    menuitem->active_ = 0;
}

THUMB void MenuTemplate_materiel::shopSackArrow(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0xc2, 0x82, 0x14, 0x14},
        {1, 2, 0xe2, 0x82, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::shopSellQuantity(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[2] = {
        {1, 2, 0xa4, 0x5c, 0x54, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 1, 1);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::shopSellArrow(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0xa4, 0xe, 0x14, 0x14},
        {1, 2, 0xdc, 0xe, 0x14, 0x14},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiRoot(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[6] = {
        {1, 2, 0xc, 0xc, 0x90, 0x10},
        {1, 2, 0xc, 0x20, 0x90, 0x10},
        {1, 2, 0xc, 0x34, 0x90, 0x10},
        {1, 2, 0xc, 0x48, 0x90, 0x10},
        {1, 2, 0xc, 0x5c, 0x90, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 1, 5, 5);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiViewCommand(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[3] = {
        {1, 2, 0xc, 0x88, 0x50, 0x10},
        {1, 2, 0x68, 0x88, 0x50, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 1, 2);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::suretigaiSelectChiaus(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[9] = {
        {1, 2, 0x14, 0x2c, 0x68, 0x18},
        {1, 2, 0x8c, 0x2c, 0x68, 0x18},
        {1, 2, 0x14, 0x4c, 0x68, 0x18},
        {1, 2, 0x8c, 0x4c, 0x68, 0x18},
        {1, 2, 0x14, 0x6c, 0x68, 0x18},
        {1, 2, 0x8c, 0x6c, 0x68, 0x18},
        {1, 2, 0x14, 0x8c, 0x68, 0x18},
        {1, 2, 0x8c, 0x8c, 0x68, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 4, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiSelectObject(menu::MenuItem* menuitem, int active, int max)
{
    static MENUITEM_DATA menu[13] = {
        {1, 2, 0xc, 0x30, 0x20, 0x18},
        {1, 2, 0x34, 0x30, 0x20, 0x18},
        {1, 2, 0x5c, 0x30, 0x20, 0x18},
        {1, 2, 0x84, 0x30, 0x20, 0x18},
        {1, 2, 0xac, 0x30, 0x20, 0x18},
        {1, 2, 0xd4, 0x30, 0x20, 0x18},
        {1, 2, 0xc, 0x58, 0x20, 0x18},
        {1, 2, 0x34, 0x58, 0x20, 0x18},
        {1, 2, 0x5c, 0x58, 0x20, 0x18},
        {1, 2, 0x84, 0x58, 0x20, 0x18},
        {1, 2, 0xac, 0x58, 0x20, 0x18},
        {1, 2, 0xd4, 0x58, 0x20, 0x18},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 6, 2, max);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiSelectSex(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[4] = {
        {1, 2, 0x18, 0x54, 0x40, 0x10},
        {1, 2, 0x60, 0x54, 0x40, 0x10},
        {1, 2, 0xa8, 0x54, 0x40, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 3, 1, 3);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiSelectAetas(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[9] = {
        {1, 2, 0x14, 0x54, 0x2c, 0x10},
        {1, 2, 0x4c, 0x54, 0x2c, 0x10},
        {1, 2, 0x84, 0x54, 0x2c, 0x10},
        {1, 2, 0xbc, 0x54, 0x2c, 0x10},
        {1, 2, 0x14, 0x6c, 0x2c, 0x10},
        {1, 2, 0x4c, 0x6c, 0x2c, 0x10},
        {1, 2, 0x84, 0x6c, 0x2c, 0x10},
        {1, 2, 0xbc, 0x6c, 0x2c, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 4, 2, 8);
    menuitem->active_ = active;
}

THUMB void MenuTemplate_materiel::surechigaiSelectSkill(menu::MenuItem* menuitem, int active)
{
    static MENUITEM_DATA menu[9] = {
        {1, 2, 0x10, 0x5c, 0x70, 0x10},
        {1, 2, 0x88, 0x5c, 0x70, 0x10},
        {1, 2, 0x10, 0x74, 0x70, 0x10},
        {1, 2, 0x88, 0x74, 0x70, 0x10},
        {1, 2, 0x10, 0x8c, 0x70, 0x10},
        {1, 2, 0x88, 0x8c, 0x70, 0x10},
        {1, 2, 0x10, 0xa4, 0x70, 0x10},
        {1, 2, 0x88, 0xa4, 0x70, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    menuitem->setMenuItem(menu, 2, 4, 8);
    menuitem->active_ = active;
}
