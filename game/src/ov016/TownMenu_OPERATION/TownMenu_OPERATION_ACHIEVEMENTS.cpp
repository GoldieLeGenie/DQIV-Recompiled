#ifdef DQIV_NATIVE
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ACHIEVEMENTS.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/UnkTownMenuDraw/TownMenuDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"

THUMB void TownMenu_OPERATION_ACHIEVEMENTS::menuSetup()
{
    item_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    item_.active_ = 0;
    achievements_.open();
}

THUMB void TownMenu_OPERATION_ACHIEVEMENTS::menuExecute()
{
    achievements_.execute(item_);
}

THUMB void TownMenu_OPERATION_ACHIEVEMENTS::menuDraw()
{
    achievements_.draw(item_, 0, 0x100, drawMenuFrame);
}

THUMB void TownMenu_OPERATION_ACHIEVEMENTS::menuUpdate()
{
    if (achievements_.update(item_, redraw_)) {
        close();
        gTownMenu_OPERATION_ROOT.open();
    }
}
#endif
