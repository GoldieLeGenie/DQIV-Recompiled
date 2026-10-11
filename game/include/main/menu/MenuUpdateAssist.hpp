#pragma once
#include "main/menu/CursorMoveGridLoop.hpp"

namespace menu {
    struct MenuItem;
}

namespace MenuUpdate_Assist {
    int isPageFlip(menu::MenuItem& item, CursorMoveBase& cursor, int& active);
    int isPageFlipOne(menu::MenuItem& item, CursorMoveBase& cursor, int& active);
    int isCancel(menu::MenuItem& item);
    int menuSelect(menu::MenuItem& item, CursorMoveBase& cursor);
}
