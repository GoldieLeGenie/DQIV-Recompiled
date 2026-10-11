#pragma once
#include "main/menu/MenuBase.hpp"

struct TownMenu_TOWN : menu::MenuBase {
    menu::MenuItem menuItem_;                   /* 0x1C */
    int unk_80;                                 /* 0x80 */
    int unk_84;                                 /* 0x84 */
    int unk_88;                                 /* 0x88 */
    int unk_8c;                                 /* 0x8C */
    int unk_90;                                 /* 0x90 */
    int unk_94;                                 /* 0x94 */
    int unk_98;                                 /* 0x98 */
    int unk_9c;                                 /* 0x9C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    virtual void menuClose();

    void unkfunc_020273a8(int camera, int map, int shop, int menuIcon);   // icon setup
    void unkfunc_02027424();
};

extern TownMenu_TOWN data_020ed11c;             /* gTownMenu_TOWN */
