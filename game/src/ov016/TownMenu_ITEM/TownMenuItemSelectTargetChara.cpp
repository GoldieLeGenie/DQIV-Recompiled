#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetItem.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectCommand.hpp"
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_CHECKTARGET.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenuItemSelectTargetChara::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    navigator_.setupBase();
    int targetChara = TownMenuPlayerControl::getSingleton()->targetChara_;
    menuItem_.active_ = targetChara;
    if (targetChara == status::g_Party.getCount()) {
        TownMenuPlayerControl::getSingleton()->targetFukuro_ = 1;
    }
    charaCount_ = TownMenuPlayerControl::getSingleton()->getDrawCharaCount(TOWN_MENU_ITEM);
    activeChara_ = TownMenuPlayerControl::getSingleton()->activeChara_;
    targetChara_ = TownMenuPlayerControl::getSingleton()->targetChara_;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        giveItemID_ = status::FukuroItemInfo::getItemId(activeItem, TownMenuPlayerControl::getSingleton()->activeItemPage_);
    } else {
        giveItemID_ = status::PlayerItemInfo::getItemIndex(activeChara_, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    sound_ = 0;
    isLock_ = 0;
}

THUMB void TownMenuItemSelectTargetChara::menuExecute()
{
    if (isLock_ == 1) {
        return;
    }
    if (charaCount_ <= 5) {
        MenuTemplate_town::townMenuItemSelectHalfChara(&menuItem_, charaCount_, menuItem_.active_);
    } else {
        MenuTemplate_town::townMenuItemSelectChara(&menuItem_, charaCount_, menuItem_.active_);
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenuItemSelectTargetChara::menuDraw()
{
    unkfunc_0217d870(activeChara_, targetChara_, giveItemID_, sound_);
    if (!data_020ed1bc.isOpen() && sound_ == 0) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenuItemSelectTargetChara::menuUpdate()
{
    if (isLock_ == 1) {
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        TownMenuPlayerControl::getSingleton()->targetChara_ = 0;
        TownMenuPlayerControl::getSingleton()->targetFukuro_ = 0;
        redraw_ = 1;
        close();
        gTownMenuItemSelectCommand.open();
        return;
    }
    navigator_.setup(5, 2, charaCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (status::g_Party.fukuro_ == 1 && menuItem_.getActive() == status::g_Party.getCount()) {
        TownMenuPlayerControl::getSingleton()->targetFukuro_ = 1;
    } else {
        TownMenuPlayerControl::getSingleton()->targetFukuro_ = 0;
    }
    TownMenuPlayerControl::getSingleton()->setTargetChara(menuItem_.active_);
    targetChara_ = menuItem_.active_;
    if (result == 2) {
        if (TownMenuPlayerControl::getSingleton()->targetFukuro_) {
            isLock_ = 1;
            gTownMenu_ITEM_CHECKTARGET.open();
        } else {
            int count = status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->targetChara_);
            if (count < 12) {
                if (count == 6) {
                    TownMenuPlayerControl::getSingleton()->targetItem_ = 0;
                    TownMenuPlayerControl::getSingleton()->targetItemPage_ = 1;
                } else {
                    TownMenuPlayerControl::getSingleton()->targetItemPage_ = count / 6;
                    if (count > 6) {
                        count -= 6;
                    }
                    TownMenuPlayerControl::getSingleton()->targetItem_ = (unsigned char)count;
                }
            } else {
                TownMenuPlayerControl::getSingleton()->targetItem_ = 5;
                TownMenuPlayerControl::getSingleton()->targetItemPage_ = 1;
            }
            close();
            gTownMenuItemSelectTargetItem.open();
        }
    }
    redraw_ = 1;
}
