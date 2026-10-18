#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "main/status/PartyStatus.hpp"

THUMB void BattleMenu_ITEMUSE2ENEMY::menuSetup()
{
    status::g_Party.setBattleMode();
    activeChara_ = 0;
    activeItem_ = 0;
    enemyNumMax_ = 0;
    oldGroup_ = 0;
    oldActive_ = 0;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    enemyNumMax_ = BattleMenuJudge::getSingleton()->getMonsterTouchRect(touchRect_);
    for (int i = 0; i < enemyNumMax_; i++) {
        if (touchRect_[i].group == btl::BattleMenuPlayerControl::getSingleton()->getTargetGroup()) {
            menuItem_.active_ = i;
            break;
        }
    }
    int group = touchRect_[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = group;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(touchRect_[menuItem_.active_].group);
    monsterMask_ = 1;
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuExecute()
{
    BattleMonsterMask::getSingleton()->execute();
    enemyNumMax_ = BattleMenuJudge::getSingleton()->getMonsterTouchRect(touchRect_);
    MenuTemplate_battle::BATTLE_ICON_ENEMY(&menuItem_, menuItem_.active_, touchRect_, enemyNumMax_);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    if (monsterMask_ == 1) {
        BattleMonsterMask::getSingleton()->select(touchRect_[menuItem_.active_].group);
    } else {
        BattleMonsterMask::getSingleton()->select(-1);
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuDraw()
{
    drawItemUseOnEnemyMenu(touchRect_[menuItem_.active_].group);
}

THUMB void BattleMenu_ITEMUSE2ENEMY::menuUpdate()
{
    oldGroup_ = touchRect_[menuItem_.active_].group;
    oldActive_ = menuItem_.active_;
    if (unkfunc_0216cee0()) {
        unkfunc_0216ce00();
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cd44()
{
    monsterMask_ = 0;
    menuItem_.result_ = 0;
    menuItem_.lastresult_ = 0;
    BattleMenuJudge::getSingleton()->setItemEnemy(activeItem_, touchRect_[menuItem_.active_].group);
    BattleMenuJudge::getSingleton()->setNextPlayer();
    close();
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cd80(int index)
{
    int old = oldActive_;
    for (;;) {
        if (index == old) {
            return;
        }
        if (index == enemyNumMax_) {
            index = 0;
            continue;
        }
        if (oldGroup_ != touchRect_[index].group) {
            menuItem_.active_ = index;
            return;
        }
        index++;
    }
}

THUMB void BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cdc0(int index)
{
    int old = oldActive_;
    for (;;) {
        if (index == old) {
            return;
        }
        if (index == -1) {
            index = enemyNumMax_ - 1;
            continue;
        }
        if (oldGroup_ != touchRect_[index].group) {
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
        if (oldGroup_ == touchRect_[menuItem_.active_].group) {
            if (menuItem_.reason_ == 2) {
                unkfunc_0216cd44();
            } else if (oldActive_ < menuItem_.active_) {
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
        unkfunc_0216cdc0(enemyNumMax_ - 1);
        break;
    case 8:
        redraw_ = 1;
        unkfunc_0216cd80(0);
        break;
    }
    BattleMonsterNamePlate::getSingleton().maxPriority(touchRect_[menuItem_.active_].group);
    int target = touchRect_[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(touchRect_[menuItem_.active_].group);
}

THUMB int BattleMenu_ITEMUSE2ENEMY::unkfunc_0216cee0()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        monsterMask_ = 0;
        close();
        gBattleMenu_ITEM.open();
        return 0;
    }
    return 1;
}
