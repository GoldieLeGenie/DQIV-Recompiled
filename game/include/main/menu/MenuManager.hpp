#pragma once
#include "globaldefs.h"
#include "main/dss/DssCore.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/DebugMenu.hpp"
#include "main/dss/UnkBgBuffer.hpp"

enum MENUDISPLAY_MODE {
    MENUDISPLAY_OFF = 0,
    MENUDISPLAY_NORMAL = 1,
    MENUDISPLAY_EXTRA = 2
};

struct MenuSubManager {
    menu::MenuBase* m_menu[8];   /* 0x00 */
    menu::MenuBase* m_next[8];   /* 0x20 */
    int m_update;                /* 0x40 */

    void setup();
    void execute();
    void draw(int x, int y);
    void update();
    void addMenu(menu::MenuBase* menu);
    void deleteMenu(menu::MenuBase* menu);
    void clearMenuAll();
    int isOpenMenu(menu::MenuBase* menu);
    int getMenuIndex(menu::MenuBase* menu);
};

struct MenuManager {
    static void setup();
    static void setMenuDisplay(int mode);
    static void requestMenuMode(MENUDISPLAY_MODE mode);
    static int isMenuMode(MENUDISPLAY_MODE mode);
    static int isRequesting();
    static void execute();
    static void addMenu(menu::MenuBase* menu);
    static void deleteMenu(menu::MenuBase* menu);
    static void clearMenuAll();
    static int isOpenMenu(menu::MenuBase* menu);
    static int getUpdateTime();
    static void setMenuEnable(int enable);
    static int isExtraMenu();
};


