#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"

THUMB void BattleMenu_ATTACK::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    monsterMask_ = 1;
    menuItem_.active_ = 0;
    enemyMaxNum_ = BattleMenuJudge::getSingleton()->getMonsterTouchRect(touchRect_);
    for (int i = 0; i < enemyMaxNum_; i++) {
        if (touchRect_[i].group == btl::BattleMenuPlayerControl::getSingleton()->getTargetGroup()) {
            menuItem_.active_ = i;
            break;
        }
    }
    int group = touchRect_[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = group;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(touchRect_[menuItem_.active_].group);
}

THUMB void BattleMenu_ATTACK::menuExecute()
{
    BattleMonsterMask::getSingleton()->execute();
    enemyMaxNum_ = BattleMenuJudge::getSingleton()->getMonsterTouchRect(touchRect_);
    MenuTemplate_battle::BATTLE_ICON_ENEMY(&menuItem_, menuItem_.active_, touchRect_, enemyMaxNum_);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    if (monsterMask_ == 1) {
        BattleMonsterMask::getSingleton()->select(touchRect_[menuItem_.active_].group);
    } else {
        BattleMonsterMask::getSingleton()->select(-1);
    }
}

THUMB void BattleMenu_ATTACK::menuDraw()
{
    unkfunc_0216ba54(touchRect_[menuItem_.active_].group);
}

THUMB void BattleMenu_ATTACK::menuUpdate()
{
    int group = touchRect_[menuItem_.active_].group;
    g_monster.getGroupCount();
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        gBattleMenu_ACTIONMENU.open();
        gBattleMenu_ACTIONMENU.pageItem_.active_ = 0;
        BattleMonsterMask::getSingleton()->select(-1);
        monsterMask_ = 0;
        close();
        return;
    }
    int old = menuItem_.active_;
    menuItem_.execInput();
    int i = menuItem_.active_;
    switch (menuItem_.result_) {
    case 1:
        redraw_ = 1;
        if (group != touchRect_[menuItem_.active_].group) {
            break;
        }
        if (menuItem_.reason_ == 2) {
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            unkfunc_0216ea10();
            return;
        }
        if (old < i) {
            for (;;) {
                if (i == old) {
                    menuItem_.active_ = i;
                    int target = touchRect_[i].group;
                    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                    break;
                }
                if (i == enemyMaxNum_) {
                    i = 0;
                    continue;
                }
                if (group != touchRect_[i].group) {
                    menuItem_.active_ = i;
                    int target = touchRect_[i].group;
                    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                    break;
                }
                i++;
            }
        } else {
            for (;;) {
                if (i == old) {
                    menuItem_.active_ = i;
                    int target = touchRect_[i].group;
                    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                    break;
                }
                if (i == -1) {
                    i = enemyMaxNum_ - 1;
                    continue;
                }
                if (group != touchRect_[i].group) {
                    menuItem_.active_ = i;
                    int target = touchRect_[i].group;
                    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                    break;
                }
                i--;
            }
        }
        break;
    case 2:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        unkfunc_0216ea10();
        return;
    case 7:
        redraw_ = 1;
        for (i = enemyMaxNum_ - 1;;) {
            if (i == old) {
                menuItem_.active_ = i;
                int target = touchRect_[i].group;
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                break;
            }
            if (i == -1) {
                i = enemyMaxNum_ - 1;
                continue;
            }
            if (group != touchRect_[i].group) {
                menuItem_.active_ = i;
                int target = touchRect_[i].group;
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                break;
            }
            i--;
        }
        break;
    case 8:
        redraw_ = 1;
        for (i = 0;;) {
            if (i == old) {
                menuItem_.active_ = i;
                int target = touchRect_[i].group;
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                break;
            }
            if (i == enemyMaxNum_) {
                i = 0;
                continue;
            }
            if (group != touchRect_[i].group) {
                menuItem_.active_ = i;
                int target = touchRect_[i].group;
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                break;
            }
            i++;
        }
        break;
    }
    if (group != touchRect_[i].group) {
        BattleMonsterNamePlate::getSingleton().maxPriority(touchRect_[i].group);
        int target = touchRect_[i].group;
        btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
        btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(touchRect_[i].group);
    }
}

THUMB void BattleMenu_ATTACK::unkfunc_0216ea10()
{
    BattleMenuJudge::getSingleton()->setAttack(touchRect_[menuItem_.active_].group);
    BattleMenuJudge::getSingleton()->setNextPlayer();
    monsterMask_ = 0;
    BattleMonsterMask::getSingleton()->select(-1);
    redraw_ = 1;
    close();
}
