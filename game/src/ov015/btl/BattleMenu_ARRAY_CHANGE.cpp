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
    unk_8c.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    unk_f0.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_20 = 0;
    unk_1c = 0;
    unk_24 = 0;
    unk_1b8.setupBase();
    unk_1c4.setupBase();
}

THUMB void BattleMenu_ARRAY_CHANGE::menuExecute()
{
    status::g_Party.setMemberShiftMode();
    if (unk_1c == 0) {
        unk_1b8.setup(4, 1, status::g_Party.getCarriageOutCount());
        int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
        MenuTemplate_battle::BATTLE_ARRAYCHANGE_5x2(&menuItem_, chara, status::g_Party.getCarriageOutCount());
    } else {
        int count = status::g_Party.getCount() - status::g_Party.getCarriageOutCount();
        unk_1c4.setup(4, 1, count);
        int num = count - unk_1c4.getMaxCountInPage() * unk_1c4.getPageNo();
        int page;
        int last = unk_1c4.getPageMaxCount() - 1;
        page = unk_1c4.getPageNo();
        if (page != last) {
            num = unk_1c4.getMaxCountInPage();
        }
        MenuTemplate_battle::BATTLE_ARRAYCHANGE_TO_5x2(&unk_8c, unk_24, num);
        int max = unk_1c4.getPageMaxCount() - 1;
        MenuTemplate_Common::TOWN_PAGE_1x1(&unk_f0, unk_f0.active_, max, 0xc4, 0x88);
    }
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_ARRAY_CHANGE::menuDraw()
{
    status::g_Party.setMemberShiftMode();
    if (unk_1c == 0) {
        unkfunc_0216bdd4();
    } else {
        int list[10];
        dss::memset(list, -1, sizeof(list));
        int out = status::g_Party.getCarriageOutCount();
        int num = 0;
        for (int i = out; i < status::g_Party.getCount(); i++) {
            list[num] = i;
            num++;
        }
        int target = out + unk_1c4.getIndex(unk_24);
        btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
        unkfunc_0216be58(list, num, unk_1c4.getPageNo());
    }
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    if (unk_1c != 0) {
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
    unkfunc_0216c524(actions, num, chara);
    menuItem_.drawActive();
    unk_8c.drawActive();
}

THUMB void BattleMenu_ARRAY_CHANGE::menuUpdate()
{
    if (!unkfunc_0216f4cc()) {
        if (unk_1c == 0) {
            unkfunc_0216f37c();
        } else {
            unkfunc_0216f3c4();
        }
    }
    if (unk_20 != unk_1c) {
        if (unk_1c == 0) {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            unk_8c.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            menuItem_.active_ = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
            unk_8c.active_ = 0;
        } else {
            menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            unk_8c.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            menuItem_.active_ = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
            unk_8c.active_ = unk_24;
        }
        unk_20 = unk_1c;
    }
}

THUMB int BattleMenu_ARRAY_CHANGE::unkfunc_0216f37c()
{
    int result = MenuUpdate_Assist::menuSelect(menuItem_, unk_1b8);
    if (result != 0) {
        redraw_ = 1;
        if (result == 1) {
            unk_1c = 0;
        }
        if (result == 2) {
            unk_1c = 1;
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
    int result = MenuUpdate_Assist::menuSelect(unk_8c, unk_1c4);
    if (result != 0) {
        redraw_ = 1;
        unk_24 = unk_8c.active_;
        if (result == 1) {
            unk_1c = 1;
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
            unk_8c.result_ = 0;
            unk_8c.lastresult_ = 0;
            redraw_ = 1;
            dss::memset(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_, 0, sizeof(btl::BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_));
            close();
            gBattleMenu_ROOT.open();
            gBattleMenu_ROOT.menuItem_.active_ = 1;
        }
        return 1;
    }
    unk_24 = unk_8c.active_;
    int active = unk_8c.active_;
    if (MenuUpdate_Assist::isPageFlipOne(unk_f0, unk_1c4, active)) {
        unk_8c.active_ = active;
        unk_24 = active;
        redraw_ = 1;
        return 1;
    }
    return 0;
}

THUMB int BattleMenu_ARRAY_CHANGE::unkfunc_0216f4cc()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (unk_1c == 0) {
            close();
            gBattleMenu_ARRAYMENU.open();
            gBattleMenu_ARRAYMENU.unk_1c = 0;
        } else if (unk_1c == 1) {
            unk_1c = 0;
        }
        redraw_ = 1;
        return 1;
    }
    return 0;
}
