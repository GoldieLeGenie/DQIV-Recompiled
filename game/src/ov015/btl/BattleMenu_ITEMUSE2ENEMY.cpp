#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "main/status/PartyStatus.hpp"

THUMB void BattleMenu_ITEMUSE2ENEMY::menuSetup()
{
    status::g_Party.setBattleMode();
    unk_148 = 0;
    unk_14c = 0;
    unk_150 = 0;
    unk_154 = 0;
    unk_158 = 0;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    unk_150 = BattleMenuJudge::getSingleton()->getMonsterTouchRect(unk_160);
    for (int i = 0; i < unk_150; i++) {
        if (unk_160[i].group == btl::BattleMenuPlayerControl::getSingleton()->getTargetGroup()) {
            menuItem_.active_ = i;
            break;
        }
    }
    int group = unk_160[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = group;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(unk_160[menuItem_.active_].group);
    unk_15c = 1;
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuExecute()
{
    BattleMonsterMask::getSingleton()->execute();
    unk_150 = BattleMenuJudge::getSingleton()->getMonsterTouchRect(unk_160);
    MenuTemplate_battle::BATTLE_ICON_ENEMY(&menuItem_, menuItem_.active_, unk_160, unk_150);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    if (unk_15c == 1) {
        BattleMonsterMask::getSingleton()->select(unk_160[menuItem_.active_].group);
    } else {
        BattleMonsterMask::getSingleton()->select(-1);
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuDraw()
{
    unkfunc_0216baf4(unk_160[menuItem_.active_].group);
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuUpdate()
{
    unk_154 = unk_160[menuItem_.active_].group;
    unk_158 = menuItem_.active_;
    if (unkfunc_0216cee0()) {
        unkfunc_0216ce00();
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cd44()
{
    unk_15c = 0;
    menuItem_.result_ = 0;
    menuItem_.lastresult_ = 0;
    BattleMenuJudge::getSingleton()->setItemEnemy(unk_14c, unk_160[menuItem_.active_].group);
    BattleMenuJudge::getSingleton()->setNextPlayer();
    close();
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cd80(int index)
{
    int old = unk_158;
    for (;;) {
        if (index == old) {
            return;
        }
        if (index == unk_150) {
            index = 0;
            continue;
        }
        if (unk_154 != unk_160[index].group) {
            menuItem_.active_ = index;
            return;
        }
        index++;
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cdc0(int index)
{
    int old = unk_158;
    for (;;) {
        if (index == old) {
            return;
        }
        if (index == -1) {
            index = unk_150 - 1;
            continue;
        }
        if (unk_154 != unk_160[index].group) {
            menuItem_.active_ = index;
            return;
        }
        index--;
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216ce00()
{
    menuItem_.execInput();
    switch (menuItem_.result_) {
    case 1:
        redraw_ = 1;
        if (unk_154 == unk_160[menuItem_.active_].group) {
            if (menuItem_.reason_ == 2) {
                unkfunc_0216cd44();
            } else if (unk_158 < menuItem_.active_) {
                unkfunc_0216cd80(menuItem_.active_);
            } else {
                unkfunc_0216cdc0(menuItem_.active_);
            }
        }
        break;
    case 2:
        redraw_ = 1;
        unkfunc_0216cd44();
        break;
    case 5:
    case 6:
    case 7:
        redraw_ = 1;
        unkfunc_0216cdc0(unk_150 - 1);
        break;
    case 8:
        redraw_ = 1;
        unkfunc_0216cd80(0);
        break;
    }
    BattleMonsterNamePlate::getSingleton().maxPriority(unk_160[menuItem_.active_].group);
    int target = unk_160[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(unk_160[menuItem_.active_].group);
}

THUMB int BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cee0()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        unk_15c = 0;
        close();
        gBattleMenu_ITEM.open();
        return 0;
    }
    return 1;
}
