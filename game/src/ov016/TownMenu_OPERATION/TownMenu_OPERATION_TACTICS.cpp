#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_TACTICS.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_OPERATION_TACTICS::menuSetup()
{
    status::g_Party.setBattleMode();
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    tacticsItem_.active_ = -1;
    charaNavigator_.setupBase();
    tacticsNavigator_.setupBase();
    mode_ = 0;
    prevMode_ = 0;
    activeChara_ = 0;
    charaCount_ = 0;
    activeTactics_ = 0;
    for (char i = 0; i < status::g_Party.getCount(); i++) {
        status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (statusInfo.haveStatus_.isPlayer() && (int)statusInfo.haveStatus_.playerIndex_ > 2) {
            chara_[charaCount_] = i;
            charaCount_++;
        }
    }
    chara_[charaCount_] = -1;
}

THUMB void TownMenu_OPERATION_TACTICS::menuExecute()
{
    if (mode_ == 0) {
        if (charaCount_ < 5) {
            MenuTemplate_town::townMenuTacticsSelectHalfChara(&charaItem_, charaCount_ + 1, activeChara_, -1, -1);
        } else {
            MenuTemplate_town::townMenuTacticsSelectChara(&charaItem_, charaCount_ + 1, activeChara_, -1, -1);
        }
    } else {
        MenuTemplate_town::townMenuTacticsSelectTC(&tacticsItem_, activeTactics_);
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_OPERATION_TACTICS::menuDraw()
{
    int chara = chara_[activeChara_];
    if (mode_ == 0) {
        if (activeChara_ == charaCount_) {
            unkfunc_0217ddc8(chara_, -2);
        } else {
            unkfunc_0217ddc8(chara_, chara);
        }
    } else {
        if (activeChara_ == charaCount_) {
            unkfunc_0217de2c(chara_, -2, activeChara_);
        } else {
            unkfunc_0217de2c(chara_, chara, activeChara_);
        }
    }
    charaItem_.drawActive();
    tacticsItem_.drawActive();
    cancelItem_.drawActive();
}

THUMB void TownMenu_OPERATION_TACTICS::menuUpdate()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (mode_ == 0) {
            close();
            gTownMenu_OPERATION_ROOT.open();
        } else if (mode_ == 1) {
            mode_ = 0;
            unkfunc_0216c9bc();
            redraw_ = 1;
        }
    }
    charaNavigator_.setup(5, 2, charaCount_ + 1);
    tacticsNavigator_.setup(3, 2, 6);
    int result = MenuUpdate_Assist::menuSelect(charaItem_, charaNavigator_);
    if (result != 0) {
        if (result == 2) {
            if (mode_ != 0) {
                mode_ = 0;
            } else {
                mode_ = 1;
                unkfunc_0216c9bc();
            }
        }
        activeChara_ = charaItem_.active_;
        redraw_ = 1;
    }
    result = MenuUpdate_Assist::menuSelect(tacticsItem_, tacticsNavigator_);
    if (result != 0) {
        if (result == 2) {
            mode_ = !mode_;
            unkfunc_0216c9f8();
        }
        activeTactics_ = tacticsItem_.active_;
        redraw_ = 1;
    }
    if (prevMode_ != mode_) {
        if (mode_ == 0) {
            charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
            tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        } else {
            charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            tacticsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
        }
        charaItem_.active_ = activeChara_;
        tacticsItem_.active_ = activeTactics_;
        prevMode_ = mode_;
    }
}

THUMB void TownMenu_OPERATION_TACTICS::unkfunc_0216c9bc()
{
    if (activeChara_ == charaCount_) {
        activeTactics_ = status::g_Party.getPlayerStatus(chara_[0])->haveStatusInfo_.battleCommand_;
    } else {
        activeTactics_ = status::g_Party.getPlayerStatus(chara_[activeChara_])->haveStatusInfo_.battleCommand_;
    }
}

THUMB void TownMenu_OPERATION_TACTICS::unkfunc_0216c9f8()
{
    CommandType command[6] = {
        COMMAND_GANGANIKOUZE,
        COMMAND_BACCHIRIGANBARE,
        COMMAND_ORENIMAKASERO,
        COMMAND_JYUMONTUKAUNA,
        COMMAND_INOCHIDAIZINI,
        COMMAND_MEIREISASERO,
    };
    if (activeChara_ == charaCount_) {
        for (int i = 0; chara_[i] != -1; i++) {
            status::g_Party.getPlayerStatus(chara_[i])->setCommandType(command[activeTactics_]);
        }
    } else {
        status::g_Party.getPlayerStatus(chara_[activeChara_])->setCommandType(command[activeTactics_]);
    }
}
