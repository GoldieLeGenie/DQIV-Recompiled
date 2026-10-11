#pragma ipa file
#include "ov003/btl/AttackAutoActionParam.hpp"
#include "ov003/btl/RecoveryAutoActionParam.hpp"
#include "ov003/btl/DefenceAutoActionParam.hpp"
#include "ov003/btl/AutoAction.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseStatus.hpp"
#include "ov015/btl/BattleSelectTarget.hpp"

#pragma profile on

THUMB btl::AutoActionParam::AutoActionParam()
{
    clear();
}

THUMB btl::AutoActionParam::~AutoActionParam()
{
}

THUMB void btl::AutoActionParam::setup(int actionIndex, int itemIndex, status::CharacterStatus* player)
{
    actionIndex_ = actionIndex;
    itemIndex_ = itemIndex;
    player_ = player;

    setupTarget();
    calcEffectValue();
    calcEffectValueException();
    calcActionDefence();
    calcTargetEffectValue();
    calcCorrectEffectVelue();
    calcCorrectEffectVelueException();
    calcGroupValue();
    calcAllValue();
    calcTargetCount();
    calcSelectGroup();
    calcSelectIndex();
}

THUMB void btl::AutoActionParam::clear()
{
    actionIndex_ = 0;
    itemIndex_ = 0;

    for (int i = 0; i < 12; i++) {
        targetActionDefence_[i] = 0;
        targetEffectValue_[i] = 0;
        lastEffectValue_[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
        targetGroupEffectValue_[i] = 0;
    }
}

THUMB void btl::AutoActionParam::setupTarget()
{
    battleSelectTargetParam_.clear();

    if (player_->characterType_ == MONSTER) {
        btl::BattleSelectTarget::setTargetMonster(&battleSelectTargetParam_);
    }
    if (player_->characterType_ == PLAYER) {
        btl::BattleSelectTarget::setTargetPlayer(&battleSelectTargetParam_);
    }
}

THUMB void btl::AutoActionParam::calcEffectValue()
{
    if (player_->characterType_ == PLAYER) {
        effectValue_ = status::UseAction::getEffectValueToMonsterMin(actionIndex_);
    }

    for (int i = 0; i < 12; i++) {
        targetEffectValue_[i] = effectValue_;
    }
}

THUMB void btl::AutoActionParam::calcEffectValueException()
{
    if (actionIndex_ != 71) {
        return;
    }

    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        targetEffectValue_[i] = status::getUsuallyAttackEffectValue(
            player_, battleSelectTargetParam_.getSourceCharacterStatus(i));
    }
}

THUMB void btl::AutoActionParam::calcActionDefence()
{
    ActionDefenceKind kind;
    ActionDefenceType type;

    kind = status::UseAction::getActionDefenceKind(actionIndex_);
    type = status::UseAction::getActionDefenceType(actionIndex_);

    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        targetActionDefence_[i] = battleSelectTargetParam_.getSourceCharacterStatus(i)
            ->haveStatusInfo_.actionDefence_.execAI(kind,type);
    }
}

THUMB void btl::AutoActionParam::calcTargetEffectValue()
{
    for (int i = 0; i < targetCount_; i++) {
        lastEffectValue_[i] = targetActionDefence_[i] * targetEffectValue_[i] / 1000;
    }
}

THUMB void btl::AutoActionParam::calcCorrectEffectVelue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        lastEffectValue_[i] = status::HaveStatusInfo::getClampValue(
            lastEffectValue_[i], 0,
            battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHp());
    }
}

THUMB void btl::AutoActionParam::calcCorrectEffectVelueException(){
    return;
}


THUMB void btl::AutoActionParam::calcGroupValue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        int group = battleSelectTargetParam_.getSourceCharacterStatus(i)->characterGroup_;
        targetGroupEffectValue_[group] = targetGroupEffectValue_[group] + lastEffectValue_[i];
    }
}

THUMB void btl::AutoActionParam::calcAllValue(){
    return;
}

THUMB void btl::AutoActionParam::calcTargetCount()
{
    int count;
    short* total;
    switch (status::UseAction::getUseArea(actionIndex_)) {
    case 1:
        for (int i = 0; i < 4; i++) {
            count = battleSelectTargetParam_.getSourceCountForGroup(i);
            if (count != 0) {
                targetGroupEffectValue_[i] = targetGroupEffectValue_[i] / count;
            }
        }
        break;
    case 3: {
        int i;
        for (i = 0;  i < 4; i++) {
            total = &targetGroupEffectValue_[4];
            *total += targetGroupEffectValue_[i];
        }
        for (int j = 0; j < 4; j++) {
            targetGroupEffectValue_[j] = 0;
        }
        break;
    }
    case 0:
    case 2:
    case 4:
    case 5:
    case 6:
        break;
    }
}

THUMB void btl::AutoActionParam::calcSelectGroup()
{
    int max = -1;
    maxGroup_ = -1;
    for (int i = 0; i < 5; i++) {
        if (max < targetGroupEffectValue_[i]) {
            max = targetGroupEffectValue_[i];
            maxGroup_ = i;
        }
    }
}

THUMB void btl::AutoActionParam::calcSelectIndex()
{
    int max = -1;
    maxIndex_ = -1;
    for (int i = 0; i < 12; i++) {
        if (max < lastEffectValue_[i]) {
            max = lastEffectValue_[i];
            maxIndex_ = i;
        }
    }
}


// ============================================================================
// AttackAutoActionParam
// ============================================================================

THUMB void btl::AttackAutoActionParam::setupTarget()
{
    battleSelectTargetParam_.clear();

    if (player_->characterType_ == MONSTER) {
        btl::BattleSelectTarget::setTargetPlayer(&battleSelectTargetParam_);
    }
    if (player_->characterType_ == PLAYER) {
        btl::BattleSelectTarget::setTargetMonster(&battleSelectTargetParam_);
    }
}

THUMB void btl::AttackAutoActionParam::calcEffectValue()
{
    if (player_->characterType_ == PLAYER) {
        effectValue_ = status::UseAction::getEffectValueToMonsterMin(actionIndex_);
    }

    for (int i = 0; i < 12; i++) {
        targetEffectValue_[i] = effectValue_;
    }
}

THUMB void btl::AttackAutoActionParam::calcEffectValueException()
{
    if (actionIndex_ == 71) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            targetEffectValue_[i] = status::getUsuallyAttackEffectValue(
                player_, battleSelectTargetParam_.getSourceCharacterStatus(i));
        }
    }

    if (actionIndex_ == 20 || actionIndex_ == 22 || actionIndex_ == 24) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            battleSelectTargetParam_.getSourceCharacterStatus(i);
            targetEffectValue_[i] = 1024;
        }
    }

    if (actionIndex_ == 137) {
        int count = battleSelectTargetParam_.sourceCount_;
        targetCount_ = count;
        for (int i = 0; i < targetCount_; i++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);
            status::getUsuallyAttackEffectValue(player_, target);
            targetEffectValue_[i] = (short)((short)status::getUsuallyAttackEffectValue(player_, target) * 3) / (count + 1);
        }
    }

    if (actionIndex_ == 109) {
        int value = status::getRandomVariation(
            dss::clamp<int>(player_->haveStatusInfo_.haveStatus_.level_ * 2 + 30, 0, 95), 10, 10);
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            battleSelectTargetParam_.getSourceCharacterStatus(i);
            targetEffectValue_[i] = value;
        }
    }

     if (actionIndex_ == 140) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);
            int value = status::getUsuallyAttackEffectValue(player_, target);
            if (target->type_.flag_  & 1) {
                value = value * 3 / 2;
            }
            targetEffectValue_[i] = value;
        }
    }

    if (actionIndex_ == 142) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);
            int value = status::getUsuallyAttackEffectValue(player_, target);
            if (target->type_.flag_  & 2) {
                value = value * 3 / 2 + 1;
            }
            targetEffectValue_[i] = value;
        }
    }
}

THUMB void btl::AttackAutoActionParam::calcCorrectEffectVelue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        lastEffectValue_[i] = status::HaveStatusInfo::getClampValue(
            lastEffectValue_[i], 0,
            battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHp());
    }
}

THUMB void btl::AttackAutoActionParam::calcCorrectEffectVelueException()
{
    calcCorrectEffectVelueMahokanta();
    calcCorrectEffectVelueMahosute();
}

THUMB void btl::AttackAutoActionParam::calcCorrectEffectVelueMahokanta()
{
    if (!status::UseAction::isMahokanta(actionIndex_)) {
        return;
    }

    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        if (battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahokanta)) {
            lastEffectValue_[i] = 0;
        }
    }
}

THUMB void btl::AttackAutoActionParam::calcCorrectEffectVelueMahosute()
{
    if (!status::UseAction::isMahosute(actionIndex_)) {
        return;
    }

    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        if (battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahosute)) {
            lastEffectValue_[i] = 0;
        }
    }
}


// ============================================================================
// RecoveryAutoActionParam
// ============================================================================

THUMB void btl::RecoveryAutoActionParam::setupTarget()
{
    battleSelectTargetParam_.clear();

    if (player_->characterType_ == PLAYER) {
        btl::BattleSelectTarget::setTargetPlayerWithDeath(&battleSelectTargetParam_);

        if (status::UseAction::getUseType(actionIndex_) == 0) {
            battleSelectTargetParam_.sourceCount_ = 1;
            battleSelectTargetParam_.setSourceCharacterStatus(0, player_);
        }
    }

    if (player_->characterType_ == MONSTER) {
        btl::BattleSelectTarget::setTargetMonster(&battleSelectTargetParam_);
    }
}

THUMB void btl::RecoveryAutoActionParam::calcEffectValue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        if (battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.isDeath()) {
            targetEffectValue_[i] = 0;
        } else {
            targetEffectValue_[i] = status::UseAction::getEffectValueToPlayerMin(actionIndex_);
        }
    }
}

THUMB void btl::RecoveryAutoActionParam::calcEffectValueException()
{
    if (actionIndex_ == 61) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);
            if (target->haveStatusInfo_.isDeath()) {
                int hpMax = target->haveStatusInfo_.getHpMax();
                targetEffectValue_[i] = (hpMax - target->haveStatusInfo_.getHp()) / 2;
            }
        }
    }

    if (actionIndex_ == 62) {
        targetCount_ = battleSelectTargetParam_.sourceCount_;
        for (int i = 0; i < targetCount_; i++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);
            if (target->haveStatusInfo_.isDeath()) {
                int hpMax = target->haveStatusInfo_.getHpMax();
                targetEffectValue_[i] = hpMax - target->haveStatusInfo_.getHp();
            }
        }
    }
}

THUMB void btl::RecoveryAutoActionParam::calcCorrectEffectVelue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        int hpMax = battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHpMax();
        lastEffectValue_[i] = status::HaveStatusInfo::getClampValue(
            lastEffectValue_[i], 0,
            (short)(hpMax - battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHp()));
    }
}

THUMB void btl::RecoveryAutoActionParam::calcCorrectEffectVelueException()
{
    int minIndex = -1;
    int minRate = 100;

    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        int hpMax = battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHpMax();
        int rate = battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHp() * 100 / hpMax;
        if (minRate > rate  ) {
            minRate = rate;
            minIndex = i;
        }
    }

    if (minIndex != -1) {
        lastEffectValue_[minIndex] = lastEffectValue_[minIndex] + 1;
    }
}

THUMB int btl::RecoveryAutoActionParam::isMemberHpDown(int percent)
{
    int i = 0;
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (; i < targetCount_; i++) {
        if (!battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.isDeath()) {
            int hpMax = battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHpMax();
            int hp = battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.getHp();
            if (hp < hpMax * (100 - percent) / 100) {
                return 1;
            }
        }
    }

    return 0;
}
THUMB int btl::RecoveryAutoActionParam::isMemberDeath()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        if (battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.isDeath()) {
            return 1;
        }
    }

    return 0;
}

THUMB int btl::RecoveryAutoActionParam::getMemberSpazz()
{
    int count = 0;
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++) {
        if (battleSelectTargetParam_.getSourceCharacterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz)) {
            count++;
        }
    }

    return count;
}


// ============================================================================
// DefenceAutoActionParam
// ============================================================================

THUMB void btl::DefenceAutoActionParam::setupTarget()
{
    unsigned int UseType;
    battleSelectTargetParam_.clear();

    UseType = status::UseAction::getUseType(actionIndex_);

    if (UseType == status::UseAction::Enemy)
        BattleSelectTarget::setTargetMonster(&battleSelectTargetParam_);

    if (UseType <= status::UseAction::Friend)
        BattleSelectTarget::setTargetPlayer(&battleSelectTargetParam_);
}


THUMB void btl::DefenceAutoActionParam::calcEffectValue()
{
    if (player_->characterType_ == PLAYER) {
        effectValue_ = 1000;
        if (actionIndex_ == 155 ){
            effectValue_ = 100;
        }
    }
    for (int i = 0; i < 12; ++i) {
        targetEffectValue_[i] = effectValue_;
    }
}


THUMB void btl::DefenceAutoActionParam::calcEffectValueException()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; ++i)
    {
        status::CharacterStatus* src = battleSelectTargetParam_.getSourceCharacterStatus(i);
        status::HaveBattleStatus* battle = &src->haveBattleStatus_;

        if (actionIndex_ == 30)
        {
            int count = 0;
            for (int j = 0; j < 6; ++j)
            {
                if (status::UseAction::isMahoton(battle->getActionIndex(j)))
                    ++count;
            }
            if (count == 0)
                targetEffectValue_[i] = 0;
        }

        if (actionIndex_ == 31)
        {
            int count = 0;
            for (int j = 0; j < 6; ++j)
            {
                if (status::UseAction::isManusa(battle->getActionIndex(j)))
                    ++count;
            }
            if (count == 0)
                targetEffectValue_[i] = 0;
        }

        if (actionIndex_ == 35)
        {
            int count = 0;
            for (int j = 0; j < 6; ++j)
            {
                if (status::UseAction::isMahokanta(battle->getActionIndex(j)))
                    ++count;
            }
            if (count == 0)
                targetEffectValue_[i] = 0;
        }

        if (actionIndex_ == 43)
        {
            int count = 0;
            for (int j = 0; j < 6; ++j)
            {
                if (status::UseAction::isFubaha(battle->getActionIndex(j)))
                    ++count;
            }
            if (count == 0)
                targetEffectValue_[i] = 0;
        }
    }
}

THUMB void btl::DefenceAutoActionParam::calcCorrectEffectVelue()
{
    targetCount_ = battleSelectTargetParam_.sourceCount_;

    for (int i = 0; i < targetCount_; i++)
    {
        status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(i);

        if (actionIndex_ == 0x1C || actionIndex_ == 0x1D) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
                lastEffectValue_[i] = 0;
            }
        }
        if (actionIndex_ == 0x1E) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahoton)) {
                lastEffectValue_[i] = 0;
            }
        }
        if (actionIndex_ == 0x1F) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusManusa)) {
                lastEffectValue_[i] = 0;
            }
        }
        if (actionIndex_ == 0x20) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion)) {
                lastEffectValue_[i] = 0;
            }
        }
        if (actionIndex_ == 0x23) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahokanta)) {
                lastEffectValue_[i] = 0;
            }
        }
        if (actionIndex_ == 0x2B) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFubaha)) {
                lastEffectValue_[i] = 0;
            }
        }

        if (actionIndex_ == 0x25 || actionIndex_ == 0x27) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDefenceChange)) {
                int defence       = target->haveStatusInfo_.getDefence(0);
                int baseDefence   = target->haveStatusInfo_.getBaseDefence();
                int defenceChange = target->haveStatusInfo_.getDefenceChange();

                if (actionIndex_ == 0x25) {
                    if (defenceChange < 0) {
                        int characterGroup = target->characterGroup_;
                        for (int j = 0; j < targetCount_; j++) {
                            if (characterGroup == battleSelectTargetParam_.getSourceCharacterStatus(j)->characterGroup_) {
                                lastEffectValue_[j] = 0;
                            }
                        }
                    }
                    else if (defence == 0) {
                        lastEffectValue_[i] = 0;
                    }
                }
                if (actionIndex_ == 0x27) {
                    if (defence == 999) {
                        lastEffectValue_[i] = 0;
                    }
                    else if (defenceChange > 0) {
                        if (dssrand::rand((defence / baseDefence) * 4) != 0) {
                            lastEffectValue_[i] = 0;
                        }
                    }
                }
            }
        }

        if (actionIndex_ == 0x2C) {
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusBaikiruto)) {
                lastEffectValue_[i] = 0;
            }
            else {
                switch (target->haveStatusInfo_.haveStatus_.playerIndex_) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 7:
                    break;
                default:
                    lastEffectValue_[i] = 0;
                    break;
                }
            }
        }
    }

    if (actionIndex_ == 0x26 || actionIndex_ == 0x2A) {
        int count = 0;
        for (int j = 0; j < targetCount_; j++) {
            status::CharacterStatus* target = battleSelectTargetParam_.getSourceCharacterStatus(j);
            if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDefenceChange)) {
                int defence       = target->haveStatusInfo_.getDefence(0);
                int defenceChange = target->haveStatusInfo_.getDefenceChange();

                if (actionIndex_ == 0x26 && defenceChange < 0) {
                    count++;
                    if (defence == 0) {
                        lastEffectValue_[j] = 0;
                    }
                }
                if (actionIndex_ == 0x2A && defenceChange > 0) {
                    count++;
                    if (defence == 999) {
                        lastEffectValue_[j] = 0;
                    }
                }
            }
        }
        if (count * 100 / targetCount_ > 30) {
            for (int j = 0; j < targetCount_; j++) {
                lastEffectValue_[j] = 0;
            }
        }
    }
}
