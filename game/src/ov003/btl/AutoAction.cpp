#include "ov003/btl/AutoAction.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseStatus.hpp"
#include "ov015/btl/BattleSelectTarget.hpp"

#pragma profile on

THUMB btl::AutoAction::AutoAction(){
    clear();
}

THUMB btl::AttackAutoActionParam::AttackAutoActionParam() {}
THUMB btl::AttackAutoActionParam::~AttackAutoActionParam() {}
THUMB btl::RecoveryAutoActionParam::RecoveryAutoActionParam() {}
THUMB btl::RecoveryAutoActionParam::~RecoveryAutoActionParam() {}
THUMB btl::DefenceAutoActionParam::DefenceAutoActionParam() {}
THUMB btl::DefenceAutoActionParam::~DefenceAutoActionParam() {}

THUMB btl::AutoAction::~AutoAction(){

}

THUMB void btl::AutoAction::clear()
{
    attackCount_ = 0;
    defenceCount_ = 0;
    assistCount_ = 0;
    recoveryCount_ = 0;

    for (int i = 0; i < 32; i++)
    {
        attackActionParam_[i].clear();
        defenceActionParam_[i].clear();
        assistActionParam_[i].clear();
        recoveryActionParam_[i].clear();
    }
}


THUMB void btl::AutoAction::setup(status::PlayerStatus* player)
{
    player_ = player;
    if (player_->haveBattleStatus_.getSelectCommand() != status::HaveBattleStatus::NoSelect) {
        if (player_->haveStatusInfo_.battleCommand_ != COMMAND_MEIREISASERO) {
            setLevelDifference();
            selectAction();
            selectMaxAction();
            setSelectAction();
        }
    }
}
THUMB void btl::AutoAction::selectAction()
{
    add(0x47, 0);
    add(0x9B, 0);

    status::HaveAction* haveAction = &player_->haveStatusInfo_.haveAction_;
    int count = haveAction->getCount();
    for (int i = 0; i < count; i++) {
        if (player_->haveStatusInfo_.battleCommand_ == COMMAND_JYUMONTUKAUNA) {
            if (status::UseAction::getActionType(haveAction->getAction(i)) == status::UseAction::ActionTypeMagic) {
                continue;
            }
        }
        add(haveAction->getAction(i), 0);
    }

    int count2;
    status::HaveItem* haveItem = &player_->haveStatusInfo_.haveItem_;
    count2 = haveItem->getCount();
    for (int i = 0; i < count2; i++) {
        int itemIndex = haveItem->getItem(i);
        if (!status::UseItem::isLost(itemIndex)) {
            add(status::UseItem::getBattleUseAction(itemIndex), itemIndex);
        }
    }
}

THUMB void btl::AutoAction::selectMaxAction()
{
    int max = -1;
    attackMaxIndex_ = -1;
    for (int i = 0; i < attackCount_; i++) {
        if (max < attackActionParam_[i].targetGroupEffectValue_[attackActionParam_[i].maxGroup_]) {
            max = attackActionParam_[i].targetGroupEffectValue_[attackActionParam_[i].maxGroup_];
            attackMaxIndex_ = i;
        }
    }

    max = -1;
    recoveryMaxIndex_ = -1;
    for (int i = 0; i < recoveryCount_; i++) {
        if (max < recoveryActionParam_[i].targetGroupEffectValue_[recoveryActionParam_[i].maxGroup_]) {
            max = recoveryActionParam_[i].targetGroupEffectValue_[recoveryActionParam_[i].maxGroup_];
            recoveryMaxIndex_ = i;
        }
    }

    max = -1;
    defenceMaxIndex_ = -1;
    for (int i = 0; i < defenceCount_; i++) {
        if (max < defenceActionParam_[i].targetGroupEffectValue_[defenceActionParam_[i].maxGroup_]) {
            max = defenceActionParam_[i].targetGroupEffectValue_[defenceActionParam_[i].maxGroup_];
            defenceMaxIndex_ = i;
        }
    }
}


THUMB void btl::AutoAction::setLevelDifference()
{
    int count = status::g_Party.getCount();
    int total = 0;

    for (int i = 0; i < count; i++) {
        total += status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_;
    }

    playerLevel_ = total / count;

    count = g_monster.getCount();
    total = 0;

    for (int i = 0; i < count; i++) {
        total += g_monster.getMonsterStatus(i)->haveStatusInfo_.haveStatus_.level_;
    }

    monsterLevel_ = total / count;
}


THUMB void btl::AutoAction::setSelectAction()
{
    setSelectAttackAction();
    setSelectDefenceAction();
    setSelectRecoveryAction();
}

THUMB void btl::AutoAction::add(int actionIndex, int itemIndex)
{
    if (validAction(actionIndex)) {
                switch (status::UseAction::getAIType(actionIndex)) {
        case status::UseAction::Attack:
            attackActionParam_[attackCount_].setup(actionIndex, itemIndex, player_);
            attackCount_++;
            break;
        case status::UseAction::Defence:
            defenceActionParam_[defenceCount_].setup(actionIndex, itemIndex, player_);
            defenceCount_++;
            break;
        case status::UseAction::Assist:
            assistActionParam_[assistCount_].setup(actionIndex, itemIndex, player_);
            assistCount_++;
            break;
        case status::UseAction::Recovery:
            recoveryActionParam_[recoveryCount_].setup(actionIndex, itemIndex, player_);
            recoveryCount_++;
            break;
        case status::UseAction::Invalid:
            break;
        }
    }
}


THUMB int btl::AutoAction::validAction(int actionIndex)
{
    if (actionIndex == 0x3F) {
        return 0;
    }
    if (status::UseAction::getUseMp(actionIndex) != 0) {
        status::HaveStatusInfo* info = &player_->haveStatusInfo_;
        if (status::UseAction::getUseMp(actionIndex) >= info->getMp()) {
            return 0;
        }
    }
    if (status::UseAction::isMahoton(actionIndex)) {
        status::StatusChange* statusChange = &player_->haveStatusInfo_.statusChange_;
        if (statusChange->isEnable(status::StatusChange::StatusMahoton)) {
            return 0;
        }
        if (statusChange->isEnable(status::StatusChange::StatusFizzleZone)) {
            return 0;
        }
    }
    return 1;
}


THUMB void btl::AutoAction::setSelectAttackAction()
{
    if (attackMaxIndex_ != -1) {
        if (player_->haveStatusInfo_.battleCommand_ != COMMAND_ORENIMAKASERO) {
            player_->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseAction,
                                                        attackActionParam_[attackMaxIndex_].actionIndex_);
            player_->haveBattleStatus_.selectedGroup_ = attackActionParam_[attackMaxIndex_].maxGroup_;
            player_->haveBattleStatus_.setActionIndex(attackActionParam_[attackMaxIndex_].actionIndex_);
        }
    }
}

THUMB void btl::AutoAction::setSelectRecoveryAction()
{
    setRecoveryAction();
    setAntiSpazzAction();
    setRebirthAction();
}

THUMB void btl::AutoAction::setSelectDefenceAction()
{
    setDefenceActionOM();
    setDefenceActionBG();

    if (player_->haveStatusInfo_.battleCommand_ == COMMAND_INOCHIDAIZINI) {
        if (recoveryMaxIndex_ != -1) {
            if (recoveryActionParam_[recoveryMaxIndex_].isMemberHpDown(0x32)) {
                setAction(0x9B, 0, -1, -1);
            }
        }
    }
}

THUMB void btl::AutoAction::setDefenceActionOM()
{
    if (player_->haveStatusInfo_.battleCommand_ == COMMAND_ORENIMAKASERO) {
        if (defenceMaxIndex_ != -1) {
            btl::DefenceAutoActionParam* param = &defenceActionParam_[defenceMaxIndex_];
            int itemIndex   = param->itemIndex_;
            int actionIndex = param->actionIndex_;
            int group       = param->maxGroup_;
            int target      = param->maxIndex_;

            if (status::UseAction::getUseArea(actionIndex) != status::UseItem::One) {
                target = -1;
            }
            setAction(actionIndex, itemIndex, group, target);
        }
    }
}


THUMB void btl::AutoAction::setDefenceActionBG()
{
    if (player_->haveStatusInfo_.battleCommand_ == COMMAND_BACCHIRIGANBARE) {
        if (monsterLevel_ - playerLevel_ >= 3) {
            if (defenceMaxIndex_ != -1) {
                btl::DefenceAutoActionParam* param = &defenceActionParam_[defenceMaxIndex_];
                int actionIndex = param->actionIndex_;
                int itemIndex   = param->itemIndex_;
                int group       = param->maxGroup_;
                int target      = param->maxIndex_;

                if (actionIndex != 0x9B) {
                    if (status::UseAction::getUseArea(actionIndex) != status::UseItem::One) {
                        target = -1;
                    }
                    setAction(actionIndex, itemIndex, group, target);
                }
            }
        }
    }
}

THUMB void btl::AutoAction::setRecoveryAction()
{
    if (recoveryMaxIndex_ != -1) {
        int percent = 30;
        CommandType command = player_->haveStatusInfo_.battleCommand_;
        if (command == COMMAND_GANGANIKOUZE) percent = 30;
        if (command == COMMAND_INOCHIDAIZINI) percent = 70;
        if (command == COMMAND_BACCHIRIGANBARE) percent = 50;
        if (command == COMMAND_JYUMONTUKAUNA) percent = 50;
        if (command == COMMAND_ORENIMAKASERO) percent = 50;

        if (recoveryActionParam_[recoveryMaxIndex_].isMemberHpDown(100 - percent)) {
            btl::RecoveryAutoActionParam* param = &recoveryActionParam_[recoveryMaxIndex_];
            int actionIndex = param->actionIndex_;
            int itemIndex   = param->itemIndex_;
            int group       = param->maxGroup_;
            int target      = param->maxIndex_;

            if (status::UseAction::getUseArea(actionIndex) != status::UseItem::One) {
                target = -1;
            }

            setAction(actionIndex, itemIndex, group, target);
        }
    }
}

THUMB void btl::AutoAction::setRebirthAction()
{
    if (recoveryMaxIndex_ != -1) {
        if (recoveryActionParam_[recoveryMaxIndex_].isMemberDeath()) {
            int index = -1;
            for (int i = 0; i < recoveryCount_; i++) {
                if (recoveryActionParam_[i].actionIndex_ == 0x3D) {
                    index = i;
                    break;
                }
            }
            for (int i = 0; i < recoveryCount_; i++) {
                if (recoveryActionParam_[i].actionIndex_ == 0x3E) {
                    index = i;
                    break;
                }
            }
            if (index != -1) {
                setAction(recoveryActionParam_[index].actionIndex_,
                          0,
                          recoveryActionParam_[index].maxGroup_,
                          recoveryActionParam_[index].maxIndex_);
            }
        }
    }
}

THUMB void btl::AutoAction::setAntiSpazzAction()
{
    if (recoveryMaxIndex_ != -1) {
        if (recoveryActionParam_[recoveryMaxIndex_].getMemberSpazz()) {
            int index = -1;
            for (int i = 0; i < recoveryCount_; i++) {
                if (recoveryActionParam_[i].actionIndex_ == 0x41) {
                    index = i;
                    break;
                }
            }
            if (index != -1) {
                setAction(recoveryActionParam_[index].actionIndex_, 0, 0, -1);
            }
        }
    }
}

THUMB void btl::AutoAction::setAction(int actionIndex, int itemIndex, int groupIndex, int targetIndex)
{
    if (actionIndex == 0x47) {
        player_->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::Attack, actionIndex);
    }
    else if (actionIndex == 0x9B) {
        player_->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::Defence, actionIndex);
    }
    else if (actionIndex >= 0x147 && actionIndex <= 0x173) {
        int count;
        status::HaveItem* haveItem = &player_->haveStatusInfo_.haveItem_;
        count = haveItem->getCount();
        for (int i = 0; i < count; i++) {
            if (itemIndex == haveItem->getItem(i)) {
                player_->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseItem, i);
                break;
            }
        }
    }
    else {
        player_->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseAction, actionIndex);
    }

    player_->haveBattleStatus_.setActionIndex(actionIndex);
    player_->haveBattleStatus_.selectedGroup_ = groupIndex;
    player_->haveBattleStatus_.selectedTarget_ = targetIndex;
}
