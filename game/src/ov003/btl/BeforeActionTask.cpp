#include "ov003/btl/BeforeActionTask.hpp"
#include "ov003/btl/selectAI.hpp"

ARM void btl::BeforeActionTask::initialize()
{
    btl::selectAI(useActionParam_);
    useActionParam_->clearMessage();

    unsigned int actionIndex = useActionParam_->actionIndex_;
    if ((actionIndex - 0x201) > 1)
        return;

    status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
    if (actor == 0)
        return;

    int hp = actor->haveStatusInfo_.getHp();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(hp);
}

ARM void btl::BeforeActionTask::terminate()
{
    unsigned int actionIndex = useActionParam_->actionIndex_;
    if ((actionIndex - 0x201) > 1)
        return;

    status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
    if (actor == 0)
        return;

    int hp = actor->haveStatusInfo_.getHp();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(hp);
}

ARM void btl::BeforeActionTask::execute()
{
    if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.isStatusChangeRelease()) {
        partTaskManager.setNextTask(9);
    }
    else {
        partTaskManager.setNextTask(1);
    }
}