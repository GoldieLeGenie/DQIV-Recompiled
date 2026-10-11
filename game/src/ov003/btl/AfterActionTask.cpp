#include "ov003/btl/AfterActionTask.hpp"
#include "ov003/btl/BattleActorMacro.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"


ARM void btl::AfterActionTask::cleanup()
{
    partTaskManager.setNextTask(8);
}


ARM bool btl::AfterActionTask::isStatusChangeEnable()   
{
    int actionIndex = useActionParam_->actionIndex_;

    if (actionIndex == 0x42)  return false;
    if (actionIndex == 0x148) return false;
    if (actionIndex == 0x2f)  return false;
    if (actionIndex == 0x1e2) return false;
    if (actionIndex == 0x1d9) return false;
    if (actionIndex == 0x11e) return false;
    if (actionIndex == 0x11f) return false;
    if (actionIndex == 0x120) return false;

    return true;
}


ARM void btl::AfterActionTask::initialize()
{
    if (isStatusChangeEnable() == 0)
        return;

    status::StatusChange* statusChange = &useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;
    statusChange->print();
    statusChange->execEndOfTurn();

    mess_  = isMessageStatusChangeRelease();
}

ARM void btl::AfterActionTask::terminate()
{
  return;
}


ARM void btl::AfterActionTask::execute()
{
    if (BattleCamera::getSingleton()->isCameraAnimation() == 0) {
        if (this->mess_ == 0) {
            cleanup();
        }
        else if (BattleAutoFeed::isEndMessage() != 0) {
            this->mess_ = isMessageStatusChangeRelease();
            if (this->mess_ == 0) {
                cleanup();
            }
        }
    }

    if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.isAddEffectSleep() == 0)
        return;

    if (useActionParam_->actorCharacterStatus_->characterType_ != MONSTER)
        return;

    int drawCtrlId = useActionParam_->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
    if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.addDamage_ > 0) {
        btl::BattleMonsterDraw2::getSingleton()->monsters_[drawCtrlId].startAnimation(0, 0x23);
    }
}


ARM int btl::AfterActionTask::isMessageStatusChangeRelease()
{
    status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
    int result = 0;

    int releaseStatus = actor->haveStatusInfo_.statusChange_.isRelease();
    int death = useActionParam_->actorCharacterStatus_->haveStatusInfo_.isDeath();
    int battleCount = g_monster.getBattleCount();

    if (releaseStatus != 0 && death == 0 && battleCount != 0) {
        btl::BattleActorMacro::setMacroActor(useActionParam_->actorCharacterStatus_, result);
        status::UseActionMacro::setStatusChangeMacro(useActionParam_->actorCharacterStatus_);

        if (releaseStatus == status::StatusChange::StatusDragoram) {
            btl::BattleActorMacro::setMacroTarget(useActionParam_->actorCharacterStatus_, result, result);
            useActionParam_->actorCharacterStatus_->haveBattleStatus_.changeMonsterReverse();
        }

        if (releaseStatus == status::StatusChange::StatusMosyasu) {
            useActionParam_->actorCharacterStatus_->setMosyasRelease();
            btl::BattleActorMacro::setMacroTarget(useActionParam_->actorCharacterStatus_, 0, 0);
            btl::BattleActorAnimation::setMosyasReverse(useActionParam_->actorCharacterStatus_);
        }

        if (releaseStatus == status::StatusChange::StatusTimeStop) {
            int count;
            int i;

            status::g_Party.setBattleMode();

            count = status::g_Party.getCount();
            for (i = 0; i < count; i++) {
                status::PlayerStatus* chara = status::g_Party.getPlayerStatus(i);
                if (chara->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusTimeStop)) {
                    chara->haveStatusInfo_.statusChange_.release(status::StatusChange::StatusTimeStop);
                }
            }

            int j;
            int monsterCount = g_monster.getCount();
            for (j = 0; j < monsterCount; j++) {
                status::MonsterStatus* monster = g_monster.getMonsterStatus(j);
                if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusTimeStop)) {
                    monster->haveStatusInfo_.statusChange_.release(status::StatusChange::StatusTimeStop);
                }
            }
        }

        int message = actor->haveStatusInfo_.statusChange_.getReleaseMessage();
        btl::BattleMessage::setMessage(message, 0, 0, 0);
        if (message != 0)
            result = 1;
    }

    return result;
}