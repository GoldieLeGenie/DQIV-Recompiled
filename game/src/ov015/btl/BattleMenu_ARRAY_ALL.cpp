#include "ov015/btl/BattleMenu.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include <mem.h>

THUMB void BattleMenu_ARRAY_ALL::menuSetup()
{
    status::g_Party.setMemberShiftMode();
    gBattleMenuSub_HISTORY.close();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    selectNum_ = 0;
    newOrder_[0] = -1;
    newOrder_[1] = -1;
    newOrder_[2] = -1;
    newOrder_[3] = -1;
    navigator_.setupBase();
}

THUMB void BattleMenu_ARRAY_ALL::menuExecute()
{
    status::g_Party.setMemberShiftMode();
    int num = selectNum_;
    int count = status::g_Party.getCount() - num;
    if (num > 0 && status::g_Party.getCarriageEnableOnGame()) {
        count++;
    }
    navigator_.setup(5, 2, count);
    MenuTemplate_battle::BATTLE_ARRAYALL_5x2(&menuItem_, menuItem_.active_, count);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_ARRAY_ALL::menuDraw()
{
    int list[10];
    memset(list, -1, sizeof(list));
    status::g_Party.setMemberShiftMode();
    status::g_Party.getCarriageOutCount();
    int count = status::g_Party.getCount();
    int num = 0;
    for (int i = 0; i < count; i++) {
        int free = 1;
        for (int j = 0; j < selectNum_; j++) {
            if (i == newOrder_[j]) {
                free = 0;
            }
        }
        if (free) {
            if (num == menuItem_.active_) {
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = i;
            }
            list[num] = i;
            num++;
        }
    }
    if (selectNum_ > 0 && status::g_Party.getCarriageEnableOnGame()) {
        if (num == menuItem_.active_) {
            int target = status::g_Party.getCount();
            btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
        }
        list[num] = -1;
        num++;
    }
    drawFormationAllMenu(list, num, newOrder_, selectNum_);
    menuItem_.drawActive();
    cancelItem_.drawActive();
    int actionCount;
    int chara = btl::BattleMenuPlayerControl::getSingleton()->targetChara_;
    if (chara != -1) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
        actionCount = info->haveAction_.getCount();
        int actions[20];
        int actionNum = 0;
        dss::memset(actions, 0, sizeof(actions));
        for (int i = 0; i < actionCount; i++) {
            if (status::UseAction::isBattleUse(info->haveAction_.getAction(i))) {
                actions[actionNum] = info->haveAction_.getAction(i);
                actionNum++;
            }
        }
        drawCharaBattleSpellList(actions, actionNum, chara);
    }
}

THUMB void BattleMenu_ARRAY_ALL::menuUpdate()
{
    status::g_Party.setMemberShiftMode();
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        cancelItem_.result_ = 0;
        cancelItem_.lastresult_ = 0;
        if (selectNum_ == 0) {
            close();
            gBattleMenu_ARRAYMENU.open();
            gBattleMenu_ARRAYMENU.cursorPos_ = 1;
            redraw_ = 1;
        } else {
            selectNum_--;
            newOrder_[selectNum_] = -1;
        }
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        redraw_ = 1;
        return;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        redraw_ = 1;
        if (result == 2) {
            int used = 0;
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            int list[10];
            dss::memset(list, 0, sizeof(list));
            int num = 0;
            int active = menuItem_.active_;
            int count = status::g_Party.getCount();
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < selectNum_; j++) {
                    if (i == newOrder_[j]) {
                        used = 1;
                    }
                }
                if (!used) {
                    list[num] = i;
                    num++;
                }
                used = 0;
            }
            if (num - 1 == active) {
                menuItem_.active_ = active - 1;
            }
            if (active == count - selectNum_ && selectNum_ != 0) {
                ChangeParty(this);
                close();
                gBattleMenu_ARRAYMENU.open();
                gBattleMenu_ARRAYMENU.cursorPos_ = 0;
                return;
            }
            newOrder_[selectNum_] = list[active];
            selectNum_++;
            int max = status::g_Party.getCount();
            if (max > 4) {
                max = 4;
            }
            if (selectNum_ == max) {
                ChangeParty(this);
                close();
                gBattleMenu_ARRAYMENU.open();
                gBattleMenu_ARRAYMENU.cursorPos_ = 0;
            }
        }
    }
}

THUMB void ChangeParty(BattleMenu_ARRAY_ALL* self)
{
    for (int i = 0; i < 4; i++) {
        if (self->newOrder_[i] >= 0) {
            self->newOrder_[i] = status::g_Party.getPlayerIndex(self->newOrder_[i]);
        } else {
            self->newOrder_[i] = 0;
        }
    }
    status::g_Party.reorder(self->newOrder_[0], self->newOrder_[1], self->newOrder_[2], self->newOrder_[3]);
    dss::memset(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_, 0, sizeof(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_));
}
