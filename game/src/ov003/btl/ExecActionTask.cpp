#include "ov003/btl/ExecActionTask.hpp"
#include "ov003/btl/BattleActorManager2.hpp"

int actionAnimation;//data_ov003_0216a97c
ARM void btl::ExecActionTask::initialize()
{
    if ((unsigned int)(useActionParam_->actionIndex_ - 0x201) <= 1 && useActionParam_->actorCharacterStatus_ != 0) {
        int hp = useActionParam_->actorCharacterStatus_->haveStatusInfo_.getHp();
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(hp);
    }

    btl::BattleSelectTarget::setActorAction(useActionParam_, btl::BattleSelectTargetParam::StartTurn);
    btl::BattleSelectTarget::setTarget(useActionParam_, btl::BattleSelectTargetParam::StartTurn);

    if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion)) {
        status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
        useActionParam_->clear();
        actor->haveBattleStatus_.setupConfuseAction();
        useActionParam_->actionIndex_ = actor->haveBattleStatus_.actionIndex_;
        useActionParam_->actorCharacterStatus_ = actor;
        btl::BattleSelectTarget::setTarget(useActionParam_, btl::BattleSelectTargetParam::StartRound);
    }

    if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDragoram)) {
        status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
        useActionParam_->clear();
        actor->haveBattleStatus_.setActionSelect(status::HaveBattleStatus::StartRound);
        useActionParam_->actionIndex_ = actor->haveBattleStatus_.actionIndex_;
        useActionParam_->actorCharacterStatus_ = actor;
        btl::BattleSelectTarget::setTarget(useActionParam_, btl::BattleSelectTargetParam::StartRound);
    }

    setupTorunekoAction();
    btl::BattleActorManager2::getSingleton()->setMegazaruRing(useActionParam_);

    useActionParam_->actorCharacterStatus_->printAction();
    btl::BattleSelectTarget::printTarget(useActionParam_);

    if (useActionParam_->actorCharacterStatus_->haveBattleStatus_.getSelectCommand() == status::HaveBattleStatus::UseItem) {
        status::UseItem::execBattleUse(useActionParam_);
    }
    else {
        status::UseAction::execUse(useActionParam_);
    }

    actionAnimation = useActionParam_->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();
    useActionParam_->actorCharacterStatus_->setMenuStatusFlag(status::HaveStatusInfo::BeforeAction);

    for (int i = 0; i < useActionParam_->targetCount_; i++) {
        if (useActionParam_->targetCharacterStatus_[i] != 0) {
            useActionParam_->targetCharacterStatus_[i]->setMenuStatusFlag(status::HaveStatusInfo::BeforeAction);
        }
    }

    if ((unsigned int)(useActionParam_->actionIndex_ - 0x201) <= 1 && useActionParam_->actorCharacterStatus_ != 0) {
        int hp = useActionParam_->actorCharacterStatus_->haveStatusInfo_.getHp();
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
        useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(hp);
    }

    useActionParam_->actorCharacterStatus_->haveBattleStatus_.print();

    param::ActionParam* actionParam = status::excelParam.actionParam_;
    if (checkCommonExec(useActionParam_) != 0) {
        if (useActionParam_->actorCharacterStatus_->characterType_ == MONSTER) {
            int actionIndex = useActionParam_->actionIndex_;
            if (actionParam[actionIndex].type == 1) {
                SoundManager::playSe(306, 0);
            }
            else if (status::UseAction::getDamageType(actionIndex) == status::UseAction::DamageTypeDamage) {
                unsigned char type = actionParam[useActionParam_->actionIndex_].type;
                if (type != 5 && type != 6) {
                    SoundManager::playSe(0x191, 0);
                }
            }
        }
        else {
            int actionIndex = useActionParam_->actionIndex_;
            if (actionParam[actionIndex].type == 1) {
                SoundManager::playSe(306, 0);
            }
            else if (status::UseAction::getDamageType(actionIndex) == status::UseAction::DamageTypeDamage) {
                unsigned char type = actionParam[useActionParam_->actionIndex_].type;
                if (type != 5 && type != 6) {
                    SoundManager::playSe(0x190, 0);
                }
            }
        }

        if ((unsigned int)(useActionParam_->actionIndex_ - 0x201) <= 1 && useActionParam_->actorCharacterStatus_ != 0) {
            int hp = useActionParam_->actorCharacterStatus_->haveStatusInfo_.getHp();
            useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
            useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
            useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(hp);
        }
    }

}

ARM bool btl::ExecActionTask::checkCommonExec(status::UseActionParam* param)
{
    status::CharacterStatus* actor = param->actorCharacterStatus_;

    if (actor->haveStatusInfo_.isStatusChangeRelease())
        return false;

    if (actor->haveStatusInfo_.isDeath()) {
        if (actor->haveStatusInfo_.isSelfImmolation() == 1)
            return true;
        if (actor->haveStatusInfo_.isDamageMyself() == 1)
            return true;
        if (actor->haveStatusInfo_.isMahokantaCounter() == 1)
            return true;
    }

    if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
        if (actor->haveStatusInfo_.isMahokantaCounter() == 1)
            return true;
    }

    if (actor->haveStatusInfo_.isAttackEnable() == 0)
        return false;

    return actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDragoram) != 1;
}

ARM void btl::ExecActionTask::terminate()
{
    if ((unsigned int)(useActionParam_->actionIndex_ - 0x201) > 1)
        return;

    if (useActionParam_->actorCharacterStatus_ == 0)
        return;

    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
}


ARM void btl::ExecActionTask::execute()
{
    partTaskManager.setNextTask(2);
}

ARM void btl::ExecActionTask::setupTorunekoAction()
{
    int actionIndex = useActionParam_->actionIndex_;

    if (actionIndex == 0x201)
        return;
    if (actionIndex == 0x202)
        return;

    if (useActionParam_->actorCharacterStatus_->haveBattleStatus_.setupTorunekoAction() == 0)
        return;

    status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
    useActionParam_->clear();
    useActionParam_->actionIndex_ = actor->haveBattleStatus_.actionIndex_;
    useActionParam_->actorCharacterStatus_ = actor;
    btl::BattleSelectTarget::setTarget(useActionParam_, btl::BattleSelectTargetParam::StartRound);
}