#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"

THUMB void BattleMenu_ARRAYMENU::menuSetup()
{
    status::g_Party.setMemberShiftMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    if (!gBattleMenuSub_HISTORY.isOpen()) {
        btl::BattleMenuPlayerControl::getSingleton()->clear();
        btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
        gBattleMenuSub_HISTORY.open();
        gBattleMenuSub_HISTORY.commandChara_ = -1;
    }
    gBattleMenuSub_HISTORY.history_ = 1;
    gBattleMenuSub_HISTORY.update_ = 1;
    gBattleMenuSub_HISTORY.select_ = 0;
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
    unk_1c = 0;
}

THUMB void BattleMenu_ARRAYMENU::menuExecute()
{
    MenuTemplate_battle::BATTLE_ARRAYMENU_2x1(&menuItem_, unk_1c);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_ARRAYMENU::menuDraw()
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0216bd94();
        menuItem_.drawActive();
    }
}

THUMB void BattleMenu_ARRAYMENU::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        cancelItem_.result_ = 0;
        cancelItem_.lastresult_ = 0;
        close();
        gBattleMenu_ROOT.open();
        gBattleMenu_ROOT.menuItem_.active_ = 1;
        redraw_ = 1;
        return;
    }
    menuItem_.execInput();
    switch (menuItem_.result_) {
    case 1:
        unk_1c = menuItem_.active_;
        break;
    case 2:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        redraw_ = 1;
        if (menuItem_.active_ == 0) {
            if (status::g_Party.getCarriageEnableOnGame() && BattleMenuJudge::getSingleton()->judgeBattleArrayChange()) {
                close();
                btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = 0;
                gBattleMenu_ARRAY_CHANGE.open();
                break;
            }
            data_020ed1bc.openMessageForBATTLE();
            data_020ed1bc.addMessage(0xc3c6d);
            break;
        }
        close();
        btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = 0;
        gBattleMenu_ARRAY_ALL.open();
        break;
    case 7:
        unk_1c = 1;
        menuItem_.active_ = 1;
        break;
    case 8:
        unk_1c = 0;
        menuItem_.active_ = 0;
        break;
    }
}
