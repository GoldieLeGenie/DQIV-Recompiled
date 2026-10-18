#include "ov016/TownMenuItemSelectItem/TownMenuItemSelectItem.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectCommand.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/UnkTownMenuDraw/TownMenuScreens.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void TownMenuItemSelectItem::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = TownMenuPlayerControl::getSingleton()->activeItem_;
    playerNavigator_.setupBase();
    playerNavigator_.setPageNo(TownMenuPlayerControl::getSingleton()->activeItemPage_);
    fukuroNavigator_.setupBase();
    fukuroNavigator_.setup(2, 3, status::g_Party.haveItemSack_.getCount());
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ == 1) {
        fukuroNavigator_.setPageNo(TownMenuPlayerControl::getSingleton()->activeItemPage_);
        menuItem_.active_ = TownMenuPlayerControl::getSingleton()->activeItem_;
    }
    page_ = TownMenuPlayerControl::getSingleton()->activeItemPage_;
    activeChara_ = TownMenuPlayerControl::getSingleton()->activeChara_;
}

THUMB void TownMenuItemSelectItem::menuExecute()
{
    unsigned char chara = TownMenuPlayerControl::getSingleton()->activeChara_;
    TownMenuPlayerControl::getSingleton()->getDrawCharaCount(TOWN_MENU_ITEM);
    TownMenuPlayerControl::getSingleton();
    int count;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        count = status::FukuroItemInfo::getPageItemCount(page_);
    } else {
        count = status::PlayerItemInfo::getItemMaxCount(chara) - page_ * 6;
        if (count > 6) {
            count = 6;
        }
    }
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, count, menuItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenuItemSelectItem::menuDraw()
{
    unsigned char chara = TownMenuPlayerControl::getSingleton()->activeChara_;
    int itemId;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        itemId = status::FukuroItemInfo::getItemId(TownMenuPlayerControl::getSingleton()->activeItem_, page_);
    } else {
        itemId = status::PlayerItemInfo::getItemIndex(chara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    drawItemSelectScreen(chara, itemId, page_);
    if (!g_TownMenuMessage.isOpen()) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenuItemSelectItem::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_ == 1) {
            TownMenuPlayerControl::getSingleton()->setActiveItem(menuItem_.active_);
            TownMenuPlayerControl::getSingleton()->setActiveItemPage(page_);
        } else {
            TownMenuPlayerControl::getSingleton()->activeItem_ = 0;
            TownMenuPlayerControl::getSingleton()->activeItemPage_ = 0;
        }
        close();
        gTownMenuItemSelectChara.open();
        redraw_ = 1;
        return;
    }
    CursorMoveGridLoop* navigator;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        fukuroNavigator_.setup(2, 3, status::FukuroItemInfo::getItemMaxCount());
        navigator = &fukuroNavigator_;
    } else {
        playerNavigator_.setup(2, 3, status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->activeChara_));
        navigator = &playerNavigator_;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, *navigator);
    if (result == 0) {
        return;
    }
    TownMenuPlayerControl::getSingleton()->setActiveItem((unsigned char)menuItem_.active_);
    TownMenuPlayerControl::getSingleton()->setActiveItemPage(navigator->getPageNo());
    page_ = navigator->getPageNo();
    if (result == 2) {
        close();
        gTownMenuItemSelectCommand.open();
        TownMenuPlayerControl::getSingleton()->setTargetItem((unsigned char)menuItem_.active_);
        TownMenuPlayerControl::getSingleton()->setTargetItemPage(navigator->getPageNo());
    }
    redraw_ = 1;
}
