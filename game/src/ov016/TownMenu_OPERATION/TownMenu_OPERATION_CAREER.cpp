#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_CAREER.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov037/PlayerTitle.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_02173d7c.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"
#include "main/menu/UnkMenuOverlay.hpp"

THUMB void TownMenu_OPERATION_CAREER::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.enableSE_ = 0;
    navigator_.setupBase();
    bookOfBeasts_ = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.isItem(0x9d)) {
            bookOfBeasts_ = 1;
            break;
        }
    }
    if (status::g_Party.haveItemSack_.isItem(0x9d)) {
        bookOfBeasts_ = 1;
    }
    unkfunc_02039460(14);
    cmn::PlayerTitle::setPlayerTitle(0);
    for (int i = 0; i < 3; i++) {
        status::g_BattleHistory.historyType_ = (status::BattleHistory::HistoryType)i;
        if (status::g_BattleHistory.getAdventureTime() != 0) {
            pageMax_ = i;
        }
        unk_28[i] = i;
    }
    page_ = 0;
    status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
    status::g_BattleHistory.regenesisAdventureTime();
}

THUMB void TownMenu_OPERATION_CAREER::menuExecute()
{
    MenuTemplate_town::TOWN_CAREER_SWITCH(&menuItem_, menuItem_.active_);
}

THUMB void TownMenu_OPERATION_CAREER::menuDraw()
{
    int type;
    if (page_ != 0) {
        type = 2;
    } else {
        type = 1;
    }
    SetTextPage(0, type);
    unkfunc_0201e194(0, 0, 0x100, 0xc0, -1);
    if (pageMax_ >= 1) {
        SetTextPage(1, type);
        unkfunc_0201e194(0, 0, 0x100, 0xc0, -1);
    }
}

THUMB void TownMenu_OPERATION_CAREER::menuUpdate()
{
    navigator_.setup(1, 1, 1);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if ((result == 2 || result == 7) && pageMax_ == 2) {
        page_ = !page_;
    }
    if (result == 3) {
        close();
        gTownMenu_OPERATION_ROOT.open();
    }
    redraw_ = 1;
}

THUMB void TownMenu_OPERATION_CAREER::SetTextPage(int page, int type)
{
    if (page != 0) {
        status::g_BattleHistory.historyType_ = (status::BattleHistory::HistoryType)type;
        unkfunc_02050698(0, 0);
        unkfunc_02173dec(status::g_BattleHistory.getHeroLevel(), 7, bookOfBeasts_, 1);
        if (type == 1) {
            unkfunc_02173e7c(status::g_BattleHistory.getAdventureTime(), 1);
        } else {
            unkfunc_02173e7c(status::g_BattleHistory.getAdventureTime(), 2);
        }
        unkfunc_02173f24(status::g_BattleHistory.getTitle());
    } else {
        status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
        if (bookOfBeasts_ != 0) {
            unkfunc_02173dec(status::g_BattleHistory.getRestMonsterCount(), 7, bookOfBeasts_, 0);
        }
        if (pageMax_ == 2) {
            if (type == 1) {
                unkfunc_02173f4c(0);
            } else {
                unkfunc_02173f4c(1);
            }
        }
        unkfunc_02173f24(status::g_BattleHistory.getTitle());
        unkfunc_02173e7c(status::g_BattleHistory.getAdventureTime(), 0);
    }
    unkfunc_02173dec(status::g_BattleHistory.getBattleCount(), 0, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getMonsterCount(), 1, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getTotalGold(), 2, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getVictoryCount(), 3, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getWipeoutCount(), 4, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getEscapeCount(), 5, 0, 0);
    unkfunc_02173dec(status::g_BattleHistory.getMaxDamage(), 6, 0, 0);
}
