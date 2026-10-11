#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "ov016/UnkTownMenu_02176fa0/UnkTownMenu_02176fa0.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenuItemSelectChara::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    navigator_.setupBase();
    TownMenuPlayerControl::getSingleton()->setActiveCommand(0);
    TownMenuPlayerControl::getSingleton()->targetChara_ = 0;
    TownMenuPlayerControl::getSingleton()->targetFukuro_ = 0;
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        TownMenuPlayerControl::getSingleton()->targetItem_ = 0;
        TownMenuPlayerControl::getSingleton()->targetItemPage_ = 0;
    }
    menuItem_.active_ = TownMenuPlayerControl::getSingleton()->activeChara_;
    charaCount_ = TownMenuPlayerControl::getSingleton()->getDrawCharaCount(TOWN_MENU_ITEM);
    closeMenuMessage_ = 0;
}

THUMB void TownMenuItemSelectChara::menuExecute()
{
    status::g_Party.setPlayerMode();
    if (charaCount_ <= 5) {
        MenuTemplate_town::townMenuItemSelectHalfChara(&menuItem_, charaCount_, menuItem_.active_);
    } else {
        MenuTemplate_town::townMenuItemSelectChara(&menuItem_, charaCount_, menuItem_.active_);
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenuItemSelectChara::menuDraw()
{
    status::g_Party.setPlayerMode();
    unkfunc_0217d598(TownMenuPlayerControl::getSingleton()->activeChara_);
    if (!data_020ed1bc.isOpen()) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenuItemSelectChara::menuUpdate()
{
    status::g_Party.setPlayerMode();
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (closeMenuMessage_ == 1) {
                close();
                gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
                return;
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        gTownMenu_ROOT.open();
        gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_ITEM;
        redraw_ = 1;
        return;
    }
    navigator_.setup(5, 2, charaCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (menuItem_.getActive() == status::g_Party.getCount()) {
        TownMenuPlayerControl::getSingleton()->activeFukuro_ = 1;
    } else {
        TownMenuPlayerControl::getSingleton()->activeFukuro_ = 0;
    }
    TownMenuPlayerControl::getSingleton()->setActiveChara(menuItem_.active_);
    if (result == 2) {
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
            if (status::FukuroItemInfo::getItemMaxCount() > 0) {
                close();
                gUnkTownMenu_02176fa0.open();
            } else {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0xc3d68);
            }
        } else if (status::PlayerItemInfo::getItemMaxCount(menuItem_.active_) > 0) {
            int count = 0;
            for (int i = 0; i < 5; i++) {
                if (status::g_Party.getPlayerStatus(menuItem_.active_)->haveStatusInfo_.haveEquipment_.getEquipment((ItemType)i)) {
                    count++;
                }
            }
            if (count == status::g_Party.getPlayerStatus(menuItem_.active_)->haveStatusInfo_.haveItem_.getCount()) {
                TownMenuPlayerControl::getSingleton()->activeItem_ = 0;
            } else {
                TownMenuPlayerControl::getSingleton()->activeItem_ = count;
            }
            TownMenuPlayerControl::getSingleton()->activeItemPage_ = 0;
            close();
            gUnkTownMenu_02176fa0.open();
        } else {
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(menuItem_.active_)->haveStatusInfo_.haveStatus_.playerIndex_);
            data_020ed1bc.addMessage(0xc3a1b);
        }
    }
    redraw_ = 1;
}
