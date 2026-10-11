#include "ov016/TownMenu_STATUS/TownMenu_STATUS.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_STATUS::menuSetup()
{
    status::g_Party.setBattleMode();
    status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    partyCount_ = status::g_Party.getCount() + 1;
    page_ = 0;
    activeChara_ = 0;
    pageSwap_ = 0;
    unk_20 = 2;
    navigator_.setPageNo(0);
    unkfunc_0217ad50();
}

THUMB void TownMenu_STATUS::menuExecute()
{
    int count = partyCount_ - page_ * 5;
    if (count > 5) {
        count = 5;
    }
    MenuTemplate_town::townMenuSelectCharaIcon(&menuItem_, count, activeChara_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_STATUS::menuDraw()
{
    status::g_Party.setBattleMode();
    int index = activeChara_ + page_ * 5;
    if (index == status::g_Party.getCount()) {
        switch (unk_20) {
        case 2:
            unkfunc_0217dc80(page_);
            break;
        case 3:
            unkfunc_0217dc6c(-2, 1, page_);
            break;
        case 4:
            unkfunc_0217dc6c(-2, 0, page_);
            break;
        }
    } else {
        if (pageSwap_ == 0) {
            unkfunc_0217db80(index, page_);
        }
        if (pageSwap_ == 1) {
            unkfunc_0217dc18(index, page_);
        }
    }
    menuItem_.drawActive();
}

THUMB void TownMenu_STATUS::menuUpdate()
{
    status::g_Party.setBattleMode();
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (navigator_.getIndex(activeChara_) == status::g_Party.getCount()) {
            unk_20--;
            if (unk_20 >= 2) {
                redraw_ = 1;
                return;
            }
        } else {
            pageSwap_--;
            if (pageSwap_ == 0) {
                redraw_ = 1;
                return;
            }
        }
        close();
        gTownMenu_ROOT.open();
        gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_STATUS;
        redraw_ = 1;
        return;
    }
    navigator_.setup(5, 1, partyCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        if (navigator_.getIndex(activeChara_) == status::g_Party.getCount()) {
            unk_20++;
            if (unk_20 > 4) {
                unk_20 = 2;
            }
        } else if (status::g_Party.getPlayerStatus(activeChara_ + page_ * 5)->haveStatusInfo_.haveAction_.getCount() > 0) {
            pageSwap_++;
            if (pageSwap_ > 1) {
                pageSwap_ = 0;
            }
        }
        activeChara_ = menuItem_.active_;
        redraw_ = 1;
        return;
    }
    unk_20 = 2;
    status::g_BattleHistory.regenesisAdventureTime();
    pageSwap_ = 0;
    page_ = navigator_.getPageNo();
    activeChara_ = menuItem_.active_;
    unkfunc_0217ad50();
    redraw_ = 1;
}

THUMB void TownMenu_STATUS::unkfunc_0217ad50()
{
    int index = navigator_.getIndex(activeChara_);
    if (index == status::g_Party.getCount()) {
        menuItem_.enableSE_ = 1;
        return;
    }
    if (status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveAction_.getCount() == 0) {
        menuItem_.enableSE_ = 0;
    } else {
        menuItem_.enableSE_ = 1;
    }
}
