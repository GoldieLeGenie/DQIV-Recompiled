#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetItem.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_CHECKTARGET.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void TownMenuItemSelectTargetItem::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    pageItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = TownMenuPlayerControl::getSingleton()->targetItem_;
    playerNavigator_.setupBase();
    fukuroNavigator_.setupBase();
    playerNavigator_.setPageNo(TownMenuPlayerControl::getSingleton()->targetItemPage_);
    fukuroNavigator_.setPageNo(TownMenuPlayerControl::getSingleton()->targetItemPage_);
    startPage_ = TownMenuPlayerControl::getSingleton()->targetItemPage_;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        giveItemID_ = status::FukuroItemInfo::getItemId(activeItem, TownMenuPlayerControl::getSingleton()->activeItemPage_);
    } else {
        int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
        giveItemID_ = status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    if (status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->targetChara_) < 12) {
        TownMenuPlayerControl::getSingleton()->targetItem_ = -1;
    }
    isLock_ = 0;
    sound_ = 0;
}

THUMB void TownMenuItemSelectTargetItem::menuExecute()
{
    int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
    int count = 0;
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        if (status::FukuroItemInfo::getItemMaxCount() > 5) {
            MenuTemplate_town::townMenuPageRightTwoArrow(&pageItem_, pageItem_.active_);
            count = status::FukuroItemInfo::getPageItemCount(startPage_);
        }
    } else if (status::PlayerItemInfo::getItemMaxCount(targetChara) > 5) {
        MenuTemplate_town::townMenuPageRightArrow(&pageItem_, 0xe4, 0x84);
        count = status::PlayerItemInfo::getItemMaxCount(targetChara) - startPage_ * 6 + 1;
        if (count > 6) {
            count = 6;
        }
    } else {
        count = status::PlayerItemInfo::getItemMaxCount(targetChara) + 1;
    }
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, count, menuItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenuItemSelectTargetItem::menuDraw()
{
    int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
    unkfunc_0217d900(TownMenuPlayerControl::getSingleton()->activeChara_, targetChara, giveItemID_, startPage_, sound_);
    if (!data_020ed1bc.isOpen() && sound_ == 0) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenuItemSelectTargetItem::menuUpdate()
{
    int active = menuItem_.active_;
    if (isLock_ == 1) {
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        gTownMenuItemSelectTargetChara.open();
        redraw_ = 1;
        return;
    }
    CursorMoveGridLoop* navigator;
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        fukuroNavigator_.setup(2, 3, status::FukuroItemInfo::getItemMaxCount() + 1);
        navigator = &fukuroNavigator_;
    } else {
        int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
        if (status::PlayerItemInfo::getItemMaxCount(targetChara) < 12) {
            playerNavigator_.setup(2, 3, status::PlayerItemInfo::getItemMaxCount(targetChara) + 1);
        } else {
            playerNavigator_.setup(2, 3, 12);
        }
        navigator = &playerNavigator_;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, *navigator);
    if (result != 0) {
        startPage_ = navigator->getPageNo();
        TownMenuPlayerControl::getSingleton()->setTargetItemPage(startPage_);
        if (unkfunc_021734f4()) {
            TownMenuPlayerControl::getSingleton()->targetItem_ = -1;
        } else {
            TownMenuPlayerControl::getSingleton()->setTargetItem(menuItem_.active_);
        }
        if (result == 2) {
            isLock_ = 1;
            int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
            int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
            int item = status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
            if (activeChara == targetChara || status::PlayerItemInfo::checkCurse(activeChara, item)) {
                if (startPage_ * 6 + menuItem_.getActive() == status::PlayerItemInfo::getItemMaxCount(targetChara)) {
                    menuItem_.active_--;
                    if (menuItem_.active_ == -1) {
                        menuItem_.active_ = 5;
                        startPage_ = 0;
                    }
                }
            }
            gTownMenu_ITEM_CHECKTARGET.open();
        }
        redraw_ = 1;
        return;
    }
    int flip;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        flip = MenuUpdate_Assist::isPageFlip(pageItem_, *navigator, active);
    } else {
        flip = MenuUpdate_Assist::isPageFlipOne(pageItem_, *navigator, active);
    }
    if (flip) {
        menuItem_.active_ = active;
        startPage_ = navigator->getPageNo();
        TownMenuPlayerControl::getSingleton()->setTargetItemPage(startPage_);
        if (unkfunc_021734f4()) {
            TownMenuPlayerControl::getSingleton()->targetItem_ = -1;
        } else {
            TownMenuPlayerControl::getSingleton()->setTargetItem(menuItem_.active_);
        }
        redraw_ = 1;
    }
}

THUMB int TownMenuItemSelectTargetItem::unkfunc_021734f4()
{
    int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_ == 0 && status::PlayerItemInfo::getItemMaxCount(targetChara) != 12 &&
        status::PlayerItemInfo::getItemMaxCount(targetChara) == startPage_ * 6 + menuItem_.getActive()) {
        return 1;
    }
    return 0;
}
