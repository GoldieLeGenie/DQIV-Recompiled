#pragma once
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// "Start the adventure" menu: new game or start of chapter 5 (level 15 hero), never instantiated
struct UnkMaterielMenu_0203d7e4 : menu::MenuBase {
    menu::MenuItem menuItem_;                   /* 0x1C */
    CursorMoveGridLoop cursor_;                 /* 0x80 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();

    void unkfunc_0203d9c0(menu::MenuItem* item, int active);
    void unkfunc_0203d9dc(int item);            // remove an item from the party bag
};
