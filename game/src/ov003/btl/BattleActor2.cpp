#include "ov003/btl/BattleActor2.hpp"
#include "main/dss/DssUtils.hpp"


#pragma profile on
THUMB btl::BattleActor2::BattleActor2()
{
    
}

THUMB btl::BattleActor2::~BattleActor2()
{

}

THUMB void btl::BattleActor2::setup(status::CharacterStatus* chara)
{
    characterStatus_ = chara;
    haveBattleStatus_ = &chara->haveBattleStatus_;

    useActionParam_.clear();
    useActionParam_.actorCharacterStatus_ = chara;

    agility_ = chara->haveStatusInfo_.getAgility(0);
    agility_ = agility_ * 10;
    agility_ = dss::getRandomVariation(agility_, 0x32, 0);
}

THUMB void btl::BattleActor2::setup()
{
    battleActorExec_.setup(&useActionParam_);
}

THUMB void btl::BattleActor2::reattack()
{
    useActionParam_.clear();
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setActorAction(&useActionParam_, btl::BattleSelectTargetParam::StartTurn);
    btl::BattleSelectTarget::setTarget(&useActionParam_,btl::BattleSelectTargetParam::StartRound);
}

THUMB void btl::BattleActor2::change()
{
    useActionParam_.clear();
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setActorAction(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}

THUMB void btl::BattleActor2::tarot()
{
    int actionIndex = characterStatus_->haveBattleStatus_.actionIndex_;
    useActionParam_.clear();
    useActionParam_.actionIndex_ = actionIndex;
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}


THUMB void btl::BattleActor2::parupunte()
{
    int actionIndex = characterStatus_->haveBattleStatus_.actionIndex_;
    useActionParam_.clear();
    useActionParam_.actionIndex_ = actionIndex;
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}

THUMB void btl::BattleActor2::meganteRing()
{
    characterStatus_->haveBattleStatus_.setupSpecialAction(0x201);

    int actionIndex = characterStatus_->haveBattleStatus_.actionIndex_;
    useActionParam_.clear();
    useActionParam_.actionIndex_ = actionIndex;
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}

THUMB void btl::BattleActor2::megazaruRing()
{
    characterStatus_->haveBattleStatus_.setupSpecialAction(0x202);
    int actionIndex = characterStatus_->haveBattleStatus_.actionIndex_;
    useActionParam_.clear();
    useActionParam_.actionIndex_ = actionIndex;
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}

THUMB void btl::BattleActor2::execute()
{
    battleActorExec_.execute();
}


THUMB int btl::BattleActor2::isEnd()
{
    if (battleActorExec_.isEnd())
        return true;

    return false;
}

THUMB void btl::BattleActor2::setAction()
{
    haveBattleStatus_->setActionSelect(status::HaveBattleStatus::StartRound);
    useActionParam_.actionIndex_ = haveBattleStatus_->actionIndex_;
    useActionParam_.actorCharacterStatus_ = characterStatus_;
    useActionParam_.itemSortIndex_ = haveBattleStatus_->sortIndex_;
}

THUMB void btl::BattleActor2::setTarget()
{
    btl::BattleSelectTarget::setTarget(&useActionParam_, btl::BattleSelectTargetParam::StartRound);
}


THUMB int btl::BattleActor2::isActionEnable()
{
    if (useActionParam_.actorCharacterStatus_->haveStatusInfo_.isDeath())
        return false;

    if (useActionParam_.actorCharacterStatus_->haveBattleStatus_.escape_ != 0)
        return false;

    if (useActionParam_.actorCharacterStatus_->haveStatusInfo_.isStatusChangeRelease())
        return false;

    return true;
}

