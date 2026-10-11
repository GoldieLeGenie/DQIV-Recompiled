#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

THUMB void BattleMenu_MAGIC2ENEMY::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_f8 = 1;
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
    activeMagic_ = btl::BattleMenuPlayerControl::getSingleton()->activeMagic_;
    activeMagicIndex_ = 0;
}

THUMB void BattleMenu_MAGIC2ENEMY::menuExecute()
{
    BattleMonsterMask::getSingleton()->execute();
    enemyNumMax_ = BattleMenuJudge::getSingleton()->getMonsterTouchRect(touchRect_);
    MenuTemplate_battle::BATTLE_ICON_ENEMY(&menuItem_, menuItem_.active_, touchRect_, enemyNumMax_);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    if (unk_f8 == 1) {
        BattleMonsterMask::getSingleton()->select(touchRect_[menuItem_.active_].group);
    } else {
        BattleMonsterMask::getSingleton()->select(-1);
    }
}

THUMB void BattleMenu_MAGIC2ENEMY::menuDraw()
{
    unkfunc_0216bbc0(touchRect_[menuItem_.active_].group);
}

THUMB void BattleMenu_MAGIC2ENEMY::menuUpdate()
{
    int group = touchRect_[menuItem_.active_].group;
    int old = menuItem_.active_;
    menuItem_.execInput();
    int i = menuItem_.active_;
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseAction, -1);
        gBattleMenu_MAGIC.open();
        gBattleMenu_MAGIC.setActiveMagicPos(activeMagicPos_);
        unk_f8 = 0;
        close();
        return;
    }
    switch (menuItem_.result_) {
    case 1:
        redraw_ = 1;
        if (group != touchRect_[menuItem_.active_].group) {
            break;
        }
        if (menuItem_.reason_ == 2) {
            unk_f8 = 0;
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            BattleMenuJudge::getSingleton()->setMagicEnemy(activeMagic_, touchRect_[menuItem_.active_].group);
            BattleMenuJudge::getSingleton()->setNextPlayer();
            close();
            break;
        }
        if (old < i) {
            for (;;) {
                if (i == old) {
                    break;
                }
                if (i == enemyNumMax_) {
                    i = 0;
                    continue;
                }
                if (group != touchRect_[i].group) {
                    menuItem_.active_ = i;
                    break;
                }
                i++;
            }
        } else {
            for (;;) {
                if (i == old) {
                    break;
                }
                if (i == -1) {
                    i = enemyNumMax_ - 1;
                    continue;
                }
                if (group != touchRect_[i].group) {
                    menuItem_.active_ = i;
                    break;
                }
                i--;
            }
        }
        break;
    case 2:
        redraw_ = 1;
        close();
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        if (activeMagicIndex_ == 0x13) {
            BattleMenuJudge::getSingleton()->minadeinFlag_ = 1;
        }
        BattleMenuJudge::getSingleton()->setMagicEnemy(activeMagic_, touchRect_[menuItem_.active_].group);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        unk_f8 = 0;
        break;
    case 5:
    case 6:
    case 7:
        redraw_ = 1;
        for (i = enemyNumMax_ - 1;;) {
            if (i == old) {
                break;
            }
            if (i == -1) {
                i = enemyNumMax_ - 1;
                continue;
            }
            if (group != touchRect_[i].group) {
                menuItem_.active_ = i;
                break;
            }
            i--;
        }
        break;
    case 8:
        redraw_ = 1;
        for (i = 0;;) {
            if (i == old) {
                break;
            }
            if (i == enemyNumMax_) {
                i = 0;
                continue;
            }
            if (group != touchRect_[i].group) {
                menuItem_.active_ = i;
                break;
            }
            i++;
        }
        break;
    }
    BattleMonsterNamePlate::getSingleton().maxPriority(touchRect_[menuItem_.active_].group);
    int target = touchRect_[menuItem_.active_].group;
    btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
    btl::BattleMenuPlayerControl::getSingleton()->setTargetGroup(touchRect_[menuItem_.active_].group);
}
