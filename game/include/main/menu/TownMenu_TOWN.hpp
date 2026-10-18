#pragma once
#include "main/menu/MenuBase.hpp"

struct TownMenu_TOWN : menu::MenuBase {
    menu::MenuItem menuItem_;                   /* 0x1C */
    int selectedIcon_;                                 /* 0x80 */
    int showCamera_;                                 /* 0x84 */
    int showMap_;                                 /* 0x88 */
    int showShop_;                                 /* 0x8C */
    int showMenuIcon_;                                 /* 0x90 */
    int helpType_;                                 /* 0x94 */
    int enable_;                                 /* 0x98 */
    int setupWait_;                                 /* 0x9C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    virtual void menuClose();

    void setIconVisibility(int camera, int map, int shop, int menuIcon);   // icon setup
    void unkfunc_02027424();
};

extern TownMenu_TOWN g_TownMenuTown;             /* gTownMenu_TOWN */
