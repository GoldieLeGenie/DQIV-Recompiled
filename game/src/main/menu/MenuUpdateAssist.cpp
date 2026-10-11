#include "main/menu/MenuUpdateAssist.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "globaldefs.h"

THUMB int MenuUpdate_Assist::isPageFlip(menu::MenuItem& item, CursorMoveBase& cursor, int& active)
{
    item.execInput();
    if (item.result_ == 1 || item.result_ == 2) {
        if (item.active_ == 0) {
            active = cursor.pageBack(active);
        } else {
            active = cursor.pageNext(active);
        }
        item.result_ = 0;
        item.lastresult_ = 0;
        return 2;
    }
    return 0;
}

THUMB int MenuUpdate_Assist::isPageFlipOne(menu::MenuItem& item, CursorMoveBase& cursor, int& active)
{
    item.execInput();
    if (item.result_ == 1 || item.result_ == 2) {
        active = cursor.pageNext(active);
        item.result_ = 0;
        item.lastresult_ = 0;
        return 2;
    }
    return 0;
}

THUMB int MenuUpdate_Assist::isCancel(menu::MenuItem& item)
{
    item.execInput();
    if (item.result_ == 4) {
        if (MenuAPI::isTownMenuRoot()) {
            gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_CANCEL;
            MenuAPI::clearMenuAll();
        }
        item.result_ = 0;
        item.lastresult_ = 0;
        return 1;
    }
    if (item.result_ == 3) {
        item.result_ = 0;
        item.lastresult_ = 0;
        return 1;
    }
    item.result_ = 0;
    item.lastresult_ = 0;
    return 0;
}

THUMB int MenuUpdate_Assist::menuSelect(menu::MenuItem& item, CursorMoveBase& cursor)
{
    int ret = 0;
    item.execInput();
    int active = item.active_;
    switch (item.result_) {
        case 1:
            ret = 1;
            break;
        case 2:
            item.result_ = 0;
            item.lastresult_ = 0;
            ret = 2;
            break;
        case 3:
            item.result_ = 0;
            item.lastresult_ = 0;
            ret = 3;
            break;
        case 4:
            if (MenuAPI::isTownMenuRoot()) {
                gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_CANCEL;
                MenuAPI::clearMenuAll();
            }
            item.result_ = 0;
            item.lastresult_ = 0;
            ret = 3;
            break;
        case 5:
            active = cursor.inputUp(active);
            ret = 4;
            break;
        case 6:
            active = cursor.inputDown(active);
            ret = 5;
            break;
        case 7:
            active = cursor.inputLeft(active);
            ret = 6;
            break;
        case 8:
            active = cursor.inputRight(active);
            ret = 7;
            break;
    }
    item.active_ = active;
    return ret;
}
