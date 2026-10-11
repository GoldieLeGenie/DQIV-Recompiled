#include "ov003/btl/ResultMessageTask.hpp"
#include "ov003/btl/BattleActorMacro.hpp"
#include "main/task/PartTaskManager.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/btl/BattleExecVictory.hpp"


ARM void btl::ResultMessageTask::setup(status::UseActionParam *useActionParam)
{
  useActionParam_ = useActionParam;
}

ARM void btl::ResultMessageTask::cleanup()
{
  partTaskManager.setNextTask(6);
}

ARM void btl::ResultMessageTask::initialize()
{
    status::CharacterStatus* target;
    int id;

    if (status::BaseAction::multiFlag_ != 0) {
        target = useActionParam_->targetCharacterStatus_[currentTarget_];
        if (target != 0) {
            if (currentTarget_ == 0) {
                target->haveStatusInfo_.setUseActionEffectValue(
                    target->haveStatusInfo_.effectValueMulti_[0]);
            }
            if (currentTarget_ == 1) {
                useActionParam_->targetCharacterStatus_[currentTarget_]
                    ->haveStatusInfo_.setUseActionEffectValue(
                        useActionParam_->targetCharacterStatus_[currentTarget_]
                            ->haveStatusInfo_.effectValueMulti_[1]);
            }
        }
    }

    target = useActionParam_->targetCharacterStatus_[currentTarget_];
    if (target != 0 && target->haveStatusInfo_.isMosyasRelease() != 0) {
        btl::BattleActorAnimation::setMosyasReverse(useActionParam_->targetCharacterStatus_[currentTarget_]);
    }

    btl::BattleActorMacro::setResultMacro(*useActionParam_, currentTarget_);

    if (btl::BattleActorManager2::getSingleton()->eventType_ != BattleActorManager2::Velorinman) {
        message_ = btl::BattleMessage::setResultMessage(useActionParam_, currentTarget_);
    }
    else if (useActionParam_->actorCharacterStatus_->characterType_ == PLAYER &&
             (id = useActionParam_->targetCharacterStatus_[0]
                       ->haveStatusInfo_.drawCtrlId_,
              id != btl::BattleExecEvent00::getRealVelorinman())) {
        message_ = 0;
    }
    else {
        message_ = btl::BattleMessage::setResultMessage(useActionParam_, currentTarget_);
    }

    BattleAutoFeed::setResultMessage();
    btl::BattleMessage::setShakeMessage(useActionParam_, currentTarget_);
    btl::BattleActorAnimation::setResultAnimation(useActionParam_, currentTarget_);

    target = useActionParam_->targetCharacterStatus_[currentTarget_];
    if (target != 0) {
        target->setMenuStatusFlag((status::HaveStatusInfo::DiffStatus)2);
    }
}



ARM void btl::ResultMessageTask::terminate()
{
  return;
}


ARM void btl::ResultMessageTask::execute()
{
    if (BattleAutoFeed::isEndResultMessage() || message_ == 0) {
        cleanup();
    }
}
