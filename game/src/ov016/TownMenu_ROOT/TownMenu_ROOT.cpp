#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/TownMenu_STATUS/TownMenu_STATUS.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_ROOT.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_ROOT::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = ROOT_ITEM;
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    TownMenuPlayerControl::getSingleton()->initialize();
}

THUMB void TownMenu_ROOT::menuExecute()
{
    navigator_.setup(2, 3, 6);
    MenuTemplate_town::townMenuRootIcon(&menuItem_, menuItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_ROOT::menuDraw()
{
    unkfunc_0217d560();
    if (!data_020ed1bc.isOpen()) {
        menuItem_.drawActive();
        cancelItem_.drawActive();
    }
}

THUMB void TownMenu_ROOT::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK || data_020ed1bc.stat_ == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (menuItem_.active_ == ROOT_SEARCH) {
                close();
                stat_ = MENUBASE_STAT_OK;
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        stat_ = MENUBASE_STAT_OK;
        return;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        redraw_ = 1;
        if (result == 2) {
            unkfunc_021788d8(menuItem_.active_);
        }
    }
}

THUMB void TownMenu_ROOT::unkfunc_021788d8(int active)
{
    switch (active) {
    case ROOT_TALK:
        if (data_0210bb94.unkfunc_02058114(0xc) == 1 && TownPlayerManager::getSingleton()->checkTalkToCharacter()) {
            stat_ = MENUBASE_STAT_CANCEL;
            redraw_ = 1;
            g_Stage.menuTalk_ = 1;
            return;
        }
        close();
        gTownMenu_PARTY_TALK.open();
        break;
    case ROOT_MAGIC:
        unkfunc_021789cc();
        break;
    case ROOT_ITEM:
        close();
        gTownMenuItemSelectChara.open();
        break;
    case ROOT_SEARCH: {
        int carriageOutCount = status::g_Party.getCarriageOutCount();
        int aliveCarriageOut = 0;
        for (int i = 0; i < carriageOutCount; i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                aliveCarriageOut = 1;
            }
        }
        if (aliveCarriageOut == 1) {
            stat_ = MENUBASE_STAT_CANCEL;
            redraw_ = 1;
            g_Stage.menuSearch_ = 1;
            return;
        }
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessage(0xc40ba, 0xc40cf);
        break;
    }
    case ROOT_STATUS:
        close();
        gTownMenu_STATUS.open();
        break;
    case ROOT_OPERATION:
        close();
        gTownMenu_OPERATION_ROOT.open();
        break;
    }
}

THUMB void TownMenu_ROOT::unkfunc_021789cc()
{
    status::g_Party.setBattleMode();
    int count = 0;
    int actor = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveAction_.getCount() > 0) {
            count++;
        } else {
            actor = i;
        }
    }
    if (count > 0) {
        close();
        gTownMenu_MAGIC_ROOT.open();
    } else {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(actor)->haveStatusInfo_.haveStatus_.playerIndex_);
        data_020ed1bc.addMessage(0xc3cd5);
    }
}
