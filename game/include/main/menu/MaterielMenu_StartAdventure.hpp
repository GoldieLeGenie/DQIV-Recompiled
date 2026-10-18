#pragma once
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// "Start the adventure" menu: new game or start of chapter 5 (level 15 hero), never instantiated
struct MaterielMenu_StartAdventure : menu::MenuBase {
    menu::MenuItem menuItem_;                   /* 0x1C */
    CursorMoveGridLoop cursor_;                 /* 0x80 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();

    void setupMenuItems(menu::MenuItem* item, int active);
    void removeBagItem(int item);            // remove an item from the party bag
};
