#include "ov015/btl/BattleMenu.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

THUMB void BattleMenu_TACTICSMENU::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    prevPhase_ = 0;
    phase_ = 0;
    tacticsPos_ = status::g_Party.getPlayerStatus(0)->haveStatusInfo_.battleCommand_;
    memberPos_ = 0;
    memberNum_ = 1;
    dss::memset(memberList_, -1, sizeof(memberList_));
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (info->haveStatus_.isPlayer_ != 0 && (int)info->haveStatus_.playerIndex_ > 2) {
            memberList_[memberNum_] = i;
            memberNum_++;
        }
    }
    memberList_[0] = -1;
    memberList_[memberNum_] = -1;
    tacticsNavigator_.setupBase();
}

THUMB void BattleMenu_TACTICSMENU::menuExecute()
{
    tacticsNavigator_.setup(3, 2, 6);
    memberNavigator_.setup(5, 1, memberNum_);
    if (phase_ == 0) {
        MenuTemplate_battle::BATTLE_TACTICSCHANGE_5x1(&menuItem_, memberPos_, memberNum_);
    } else {
        MenuTemplate_battle::BATTLE_TACTICS_2x3(&tacticsItem_, tacticsPos_);
    }
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_TACTICSMENU::menuDraw()
{
    if (phase_ == 0) {
        drawTacticsMemberMenu();
    } else {
        drawTacticsSelectMenu();
    }
    menuItem_.drawActive();
    tacticsItem_.drawActive();
}

THUMB void BattleMenu_TACTICSMENU::menuUpdate()
{
    if (!unkfunc_0216fc4c() && !unkfunc_0216fb88()) {
        unkfunc_0216fbdc();
    }
    memberPos_ = memberNavigator_.getIndex(menuItem_.active_);
    tacticsPos_ = tacticsNavigator_.getIndex(tacticsItem_.active_);
    if (prevPhase_ != phase_) {
        if (phase_ == 0) {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        } else {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
        }
        menuItem_.active_ = memberPos_;
        tacticsItem_.active_ = tacticsPos_;
        prevPhase_ = phase_;
    }
}

THUMB int BattleMenu_TACTICSMENU::unkfunc_0216fb88()
{
    int result = MenuUpdate_Assist::menuSelect(menuItem_, memberNavigator_);
    if (result != 0) {
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        redraw_ = 1;
        if (result == 1) {
            phase_ = 0;
        } else if (result == 2) {
            phase_ = 1;
        }
        memberPos_ = memberNavigator_.getIndex(menuItem_.active_);
        unkfunc_0216fc98();
        return 1;
    }
    return 0;
}

THUMB void BattleMenu_TACTICSMENU::unkfunc_0216fbdc()
{
    int result = MenuUpdate_Assist::menuSelect(tacticsItem_, tacticsNavigator_);
    if (result != 0) {
        if (result == 1) {
            phase_ = 1;
            redraw_ = 1;
            return;
        }
        if (result == 2) {
            if (phase_ == 0) {
                phase_ = 1;
            } else if (phase_ == 1) {
                unkfunc_0216fcec();
                phase_ = 0;
            }
            if (memberPos_ == 0) {
                close();
                gBattleMenu_ROOT.open();
                gBattleMenu_ROOT.menuItem_.active_ = 2;
            }
            tacticsItem_.result_ = 0;
            tacticsItem_.lastresult_ = 0;
            redraw_ = 1;
        }
    }
}

THUMB int BattleMenu_TACTICSMENU::unkfunc_0216fc4c()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        cancelItem_.result_ = 0;
        cancelItem_.lastresult_ = 0;
        if (phase_ == 0) {
            close();
            gBattleMenu_ROOT.open();
            gBattleMenu_ROOT.menuItem_.active_ = 2;
        } else if (phase_ == 1) {
            phase_ = 0;
        }
        redraw_ = 1;
        return 1;
    }
    return 0;
}

THUMB void BattleMenu_TACTICSMENU::unkfunc_0216fc98()
{
    int chara = memberList_[memberPos_];
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
    if (memberPos_ < 0) {
        tacticsPos_ = status::g_Party.getPlayerStatus(memberList_[memberPos_])->haveStatusInfo_.battleCommand_;
    } else if (memberList_[1] != -1) {
        tacticsPos_ = status::g_Party.getPlayerStatus(memberList_[1])->haveStatusInfo_.battleCommand_;
    } else {
        tacticsPos_ = 0;
    }
}

THUMB void BattleMenu_TACTICSMENU::unkfunc_0216fcec()
{
    CommandType command[6] = {
        COMMAND_GANGANIKOUZE,
        COMMAND_BACCHIRIGANBARE,
        COMMAND_ORENIMAKASERO,
        COMMAND_JYUMONTUKAUNA,
        COMMAND_INOCHIDAIZINI,
        COMMAND_MEIREISASERO
    };
    if (memberPos_ != 0) {
        status::g_Party.getPlayerStatus(memberList_[memberPos_])->haveStatusInfo_.battleCommand_ = command[tacticsPos_];
        return;
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (info->haveStatus_.isPlayer_ != 0 && info->haveStatus_.playerIndex_ != 1 && info->haveStatus_.playerIndex_ != 2) {
            info->battleCommand_ = command[tacticsPos_];
        }
    }
}
