#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/GameFlag.hpp"

THUMB void BattleMenu_ROOT::menuSetup()
{
    status::g_Party.setBattleMode();
    btl::BattleMenuPlayerControl::getSingleton()->clear();
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
    BattleMenuJudge::getSingleton()->turnSetup();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    if (!gBattleMenuSub_HISTORY.isOpen()) {
        gBattleMenuSub_HISTORY.open();
        gBattleMenuSub_HISTORY.commandChara_ = -1;
    }
    gBattleMenuSub_HISTORY.update_ = 1;
    gBattleMenuSub_HISTORY.history_ = 1;
    navigator_.setupBase();
    unk_f4 = 0;
    unk_f0 = 0;
    unk_f8 = 1;
    if (status::g_Story.chapter_ >= LAST_QUEST && g_AreaFlag.check(0x132)) {
        unk_f8 = 0;
    }
}

THUMB void BattleMenu_ROOT::menuExecute()
{
    if (unk_f4 != 0) {
        unk_f0--;
        menuItem_.active_ = 0;
        if (unk_f0 <= 0) {
            unk_f0 = 0;
            unk_f4 = 0;
            redraw_ = 1;
        }
        return;
    }
    int count = unk_f8 ? 2 : 4;
    navigator_.setup(2, 2, count);
    MenuTemplate_battle::BATTLE_MENUICON2x2(&menuItem_, count, menuItem_.active_);
    MenuTemplate_battle::BATTLE_RECT_ENEMY(&cancelItem_);
}

THUMB void BattleMenu_ROOT::menuDraw()
{
    if (unk_f4 == 0) {
        unkfunc_0216b9c8(unk_f8);
        menuItem_.drawActive();
    }
}

THUMB void BattleMenu_ROOT::menuUpdate()
{
    status::g_Party.setBattleMode();
    if (unk_f4 == 0) {
        if (data_020ed1bc.isOpen()) {
            if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
                data_020ed1bc.close();
                return;
            }
        } else {
            cancelItem_.execInput();
            if (cancelItem_.result_ == 3) {
                cancelItem_.result_ = 0;
                cancelItem_.lastresult_ = 0;
                unk_f4 = 1;
                unk_f0 = CANCEL_WAIT_TIME;
                redraw_ = 1;
                return;
            }
            int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
            if (result == 0) {
                return;
            }
            short active = menuItem_.active_;
            if (result != 2) {
                return;
            }
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            if (unk_f8 != 0 && active == 1) {
                active = 3;
            }
            switch (active) {
            case 0:
                close();
                BattleMenuJudge::getSingleton()->setNextPlayer();
                break;
            case 1:
                status::g_Party.setMemberShiftMode();
                if (status::g_Party.getCount() > 1) {
                    close();
                    gBattleMenu_ARRAYMENU.open();
                    break;
                }
                close();
                gBattleMenu_NGMESSAGE.open();
                gBattleMenu_NGMESSAGE.messageID_ = 0xc3d59;
                gBattleMenu_NGMESSAGE.returnPos_ = 1;
                gBattleMenu_NGMESSAGE.returnMenu_ = BattleMenu_NGMESSAGE::MENU_ROOT;
                break;
            case 2: {
                status::g_Party.setBattleMode();
                int count = status::g_Party.getCount();
                for (int i = 0; i < count; i++) {
                    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
                    if (info->haveStatus_.isPlayer_ != 0 && (int)info->haveStatus_.playerIndex_ > 2) {
                        close();
                        gBattleMenu_TACTICSMENU.open();
                        return;
                    }
                }
                close();
                gBattleMenu_NGMESSAGE.open();
                gBattleMenu_NGMESSAGE.messageID_ = 0xc3d57;
                gBattleMenu_NGMESSAGE.returnPos_ = 2;
                gBattleMenu_NGMESSAGE.returnMenu_ = BattleMenu_NGMESSAGE::MENU_ROOT;
                break;
            }
            case 3:
                btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
                BattleMonsterMask::getSingleton()->select(-1);
                gBattleMenuSub_HISTORY.commandChara_ = -1;
                gBattleMenuSub_HISTORY.update_ = 0;
                close();
                gBattleMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
                btl::BattleActorManager2::getSingleton()->setEscape(1);
                break;
            }
        }
    }
}
