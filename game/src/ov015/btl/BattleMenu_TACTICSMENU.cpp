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
    unk_120.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_20 = 0;
    unk_1c = 0;
    unk_28 = status::g_Party.getPlayerStatus(0)->haveStatusInfo_.battleCommand_;
    unk_24 = 0;
    unk_54 = 1;
    dss::memset(unk_2c, -1, sizeof(unk_2c));
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (info->haveStatus_.isPlayer_ != 0 && (int)info->haveStatus_.playerIndex_ > 2) {
            unk_2c[unk_54] = i;
            unk_54++;
        }
    }
    unk_2c[0] = -1;
    unk_2c[unk_54] = -1;
    unk_1f4.setupBase();
}

THUMB void BattleMenu_TACTICSMENU::menuExecute()
{
    unk_1f4.setup(3, 2, 6);
    unk_1e8.setup(5, 1, unk_54);
    if (unk_1c == 0) {
        MenuTemplate_battle::BATTLE_TACTICSCHANGE_5x1(&menuItem_, unk_24, unk_54);
    } else {
        MenuTemplate_battle::BATTLE_TACTICS_2x3(&unk_120, unk_28);
    }
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_TACTICSMENU::menuDraw()
{
    if (unk_1c == 0) {
        unkfunc_0216bbdc();
    } else {
        unkfunc_0216bc84();
    }
    menuItem_.drawActive();
    unk_120.drawActive();
}

THUMB void BattleMenu_TACTICSMENU::menuUpdate()
{
    if (!unkfunc_0216fc4c() && !unkfunc_0216fb88()) {
        unkfunc_0216fbdc();
    }
    unk_24 = unk_1e8.getIndex(menuItem_.active_);
    unk_28 = unk_1f4.getIndex(unk_120.active_);
    if (unk_20 != unk_1c) {
        if (unk_1c == 0) {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            unk_120.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        } else {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            unk_120.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
        }
        menuItem_.active_ = unk_24;
        unk_120.active_ = unk_28;
        unk_20 = unk_1c;
    }
}

THUMB int BattleMenu_TACTICSMENU::unkfunc_0216fb88()
{
    int result = MenuUpdate_Assist::menuSelect(menuItem_, unk_1e8);
    if (result != 0) {
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        redraw_ = 1;
        if (result == 1) {
            unk_1c = 0;
        } else if (result == 2) {
            unk_1c = 1;
        }
        unk_24 = unk_1e8.getIndex(menuItem_.active_);
        unkfunc_0216fc98();
        return 1;
    }
    return 0;
}

THUMB void BattleMenu_TACTICSMENU::unkfunc_0216fbdc()
{
    int result = MenuUpdate_Assist::menuSelect(unk_120, unk_1f4);
    if (result != 0) {
        if (result == 1) {
            unk_1c = 1;
            redraw_ = 1;
            return;
        }
        if (result == 2) {
            if (unk_1c == 0) {
                unk_1c = 1;
            } else if (unk_1c == 1) {
                unkfunc_0216fcec();
                unk_1c = 0;
            }
            if (unk_24 == 0) {
                close();
                gBattleMenu_ROOT.open();
                gBattleMenu_ROOT.menuItem_.active_ = 2;
            }
            unk_120.result_ = 0;
            unk_120.lastresult_ = 0;
            redraw_ = 1;
        }
    }
}

THUMB int BattleMenu_TACTICSMENU::unkfunc_0216fc4c()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        cancelItem_.result_ = 0;
        cancelItem_.lastresult_ = 0;
        if (unk_1c == 0) {
            close();
            gBattleMenu_ROOT.open();
            gBattleMenu_ROOT.menuItem_.active_ = 2;
        } else if (unk_1c == 1) {
            unk_1c = 0;
        }
        redraw_ = 1;
        return 1;
    }
    return 0;
}

THUMB void BattleMenu_TACTICSMENU::unkfunc_0216fc98()
{
    int chara = unk_2c[unk_24];
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
    if (unk_24 < 0) {
        unk_28 = status::g_Party.getPlayerStatus(unk_2c[unk_24])->haveStatusInfo_.battleCommand_;
    } else if (unk_2c[1] != -1) {
        unk_28 = status::g_Party.getPlayerStatus(unk_2c[1])->haveStatusInfo_.battleCommand_;
    } else {
        unk_28 = 0;
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
    if (unk_24 != 0) {
        status::g_Party.getPlayerStatus(unk_2c[unk_24])->haveStatusInfo_.battleCommand_ = command[unk_28];
        return;
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (info->haveStatus_.isPlayer_ != 0 && info->haveStatus_.playerIndex_ != 1 && info->haveStatus_.playerIndex_ != 2) {
            info->battleCommand_ = command[unk_28];
        }
    }
}
