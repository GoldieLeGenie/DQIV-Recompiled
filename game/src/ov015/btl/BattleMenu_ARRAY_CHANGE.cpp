#include "ov015/btl/BattleMenu.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void BattleMenu_ARRAY_CHANGE::menuSetup()
{
    status::g_Party.setMemberShiftMode();
    gBattleMenuSub_HISTORY.close();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    targetItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    pageItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    prevPhase_ = 0;
    phase_ = 0;
    targetPos_ = 0;
    memberNavigator_.setupBase();
    targetNavigator_.setupBase();
}

THUMB void BattleMenu_ARRAY_CHANGE::menuExecute()
{
    status::g_Party.setMemberShiftMode();
    if (phase_ == 0) {
        memberNavigator_.setup(4, 1, status::g_Party.getCarriageOutCount());
        int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
        MenuTemplate_battle::BATTLE_ARRAYCHANGE_5x2(&menuItem_, chara, status::g_Party.getCarriageOutCount());
    } else {
        int count = status::g_Party.getCount() - status::g_Party.getCarriageOutCount();
        targetNavigator_.setup(4, 1, count);
        int num = count - targetNavigator_.getMaxCountInPage() * targetNavigator_.getPageNo();
        int page;
        int last = targetNavigator_.getPageMaxCount() - 1;
        page = targetNavigator_.getPageNo();
        if (page != last) {
            num = targetNavigator_.getMaxCountInPage();
        }
        MenuTemplate_battle::BATTLE_ARRAYCHANGE_TO_5x2(&targetItem_, targetPos_, num);
        int max = targetNavigator_.getPageMaxCount() - 1;
        MenuTemplate_Common::TOWN_PAGE_1x1(&pageItem_, pageItem_.active_, max, 0xc4, 0x88);
    }
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_ARRAY_CHANGE::menuDraw()
{
    status::g_Party.setMemberShiftMode();
    if (phase_ == 0) {
        drawFormationChangeMemberMenu();
    } else {
        int list[10];
        dss::memset(list, -1, sizeof(list));
        int out = status::g_Party.getCarriageOutCount();
        int num = 0;
        for (int i = out; i < status::g_Party.getCount(); i++) {
            list[num] = i;
            num++;
        }
        int target = out + targetNavigator_.getIndex(targetPos_);
        btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
        drawFormationChangeTargetMenu(list, num, targetNavigator_.getPageNo());
    }
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    if (phase_ != 0) {
        chara = btl::BattleMenuPlayerControl::getSingleton()->targetChara_;
    }
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
    int count = info->haveAction_.getCount();
    int actions[20];
    int num = 0;
    dss::memset(actions, 0, sizeof(actions));
    for (int i = 0; i < count; i++) {
        if (status::UseAction::isBattleUse(info->haveAction_.getAction(i))) {
            actions[num] = info->haveAction_.getAction(i);
            num++;
        }
    }
    drawCharaBattleSpellList(actions, num, chara);
    menuItem_.drawActive();
    targetItem_.drawActive();
}

THUMB void BattleMenu_ARRAY_CHANGE::menuUpdate()
{
    if (!unkfunc_0216f4cc()) {
        if (phase_ == 0) {
            unkfunc_0216f37c();
        } else {
            unkfunc_0216f3c4();
        }
    }
    if (prevPhase_ != phase_) {
        if (phase_ == 0) {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            targetItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            menuItem_.active_ = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
            targetItem_.active_ = 0;
        } else {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            targetItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            menuItem_.active_ = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
            targetItem_.active_ = targetPos_;
        }
        prevPhase_ = phase_;
    }
}

THUMB int BattleMenu_ARRAY_CHANGE::unkfunc_0216f37c()
{
    int result = MenuUpdate_Assist::menuSelect(menuItem_, memberNavigator_);
    if (result != 0) {
        redraw_ = 1;
        if (result == 1) {
            phase_ = 0;
        }
        if (result == 2) {
            phase_ = 1;
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
        }
        int chara = menuItem_.active_;
        btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
        return 1;
    }
    int chara = menuItem_.active_;
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
    return 0;
}

THUMB int BattleMenu_ARRAY_CHANGE::unkfunc_0216f3c4()
{
    int result = MenuUpdate_Assist::menuSelect(targetItem_, targetNavigator_);
    if (result != 0) {
        redraw_ = 1;
        targetPos_ = targetItem_.active_;
        if (result == 1) {
            phase_ = 1;
        }
        if (result == 2) {
            int order[4];
            dss::memset(order, 0, sizeof(order));
            for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
                order[i] = status::g_Party.getPlayerIndex(i);
                if (i == btl::BattleMenuPlayerControl::getSingleton()->activeChara_) {
                    order[i] = status::g_Party.getPlayerIndex(btl::BattleMenuPlayerControl::getSingleton()->targetChara_);
                }
            }
            status::g_Party.reorder(order[0], order[1], order[2], order[3]);
            targetItem_.result_ = 0;
            targetItem_.lastresult_ = 0;
            redraw_ = 1;
            dss::memset(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_, 0, sizeof(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_));
            close();
            gBattleMenu_ROOT.open();
            gBattleMenu_ROOT.menuItem_.active_ = 1;
        }
        return 1;
    }
    targetPos_ = targetItem_.active_;
    int active = targetItem_.active_;
    if (MenuUpdate_Assist::isPageFlipOne(pageItem_, targetNavigator_, active)) {
        targetItem_.active_ = active;
        targetPos_ = active;
        redraw_ = 1;
        return 1;
    }
    return 0;
}

THUMB int BattleMenu_ARRAY_CHANGE::unkfunc_0216f4cc()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (phase_ == 0) {
            close();
            gBattleMenu_ARRAYMENU.open();
            gBattleMenu_ARRAYMENU.cursorPos_ = 0;
        } else if (phase_ == 1) {
            phase_ = 0;
        }
        redraw_ = 1;
        return 1;
    }
    return 0;
}
