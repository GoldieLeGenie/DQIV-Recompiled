#include "ov003/btl/BeforeMessageTask.hpp"
#include "ov003/btl/BattleActorMacro.hpp"
#include "main/task/PartTaskManager.hpp"
#include "ov003/btl/BattleMessage.hpp"

ARM void btl::BeforeMessageTask::setup(status::UseActionParam *useActionParam)
{
  useActionParam_ = useActionParam;
}


ARM void btl::BeforeMessageTask::initialize()
{
    int hp;

    btl::BattleActorMacro::setMacroActor(useActionParam_->actorCharacterStatus_, 0);
    btl::BattleActorMacro::setMacroTarget(useActionParam_->targetCharacterStatus_[0], 0, 0);
    status::UseActionMacro::setBeforeMacro(useActionParam_->actorCharacterStatus_, useActionParam_->actionIndex_);
    message_ = btl::BattleMessage::setBeforeMessage(useActionParam_);
    BattleAutoFeed::setAfterMessage();

    if ((unsigned int)(useActionParam_->actionIndex_ - 0x201) > 1) {
        return;
    }
    if (useActionParam_->actorCharacterStatus_ == 0) {
        return;
    }

    hp = useActionParam_->actorCharacterStatus_->haveStatusInfo_.getHp();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp((unsigned short)hp);
}

ARM void btl::BeforeMessageTask::terminate()
{
  return;
}

ARM void btl::BeforeMessageTask::execute()
{
    if (message_ != 0) {
        if (BattleAutoFeed::isEndAfterMessage() != 0) {
            partTaskManager.setNextTask(3);
        }
    }
    else {
        partTaskManager.setNextTask(3);
    }
}


