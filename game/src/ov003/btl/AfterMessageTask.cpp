#include "ov003/btl/AfterMessageTask.hpp"
#include "ov003/btl/BattleActorMacro.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/btl/BattleExecVictory.hpp"

ARM void btl::AfterMessageTask::setup(status::UseActionParam *useActionParam)
{
  useActionParam_ = useActionParam;
}

ARM void btl::AfterMessageTask::cleanup()
{
    int i;

    currentTarget_++;
    if (currentTarget_ >= targetCount_) {
        partTaskManager.setNextTask(7);
        useActionParam_->actorCharacterStatus_->clearMenuStatusFlag();
        for (i = 0; i < useActionParam_->targetCount_; i++) {
            if (useActionParam_->targetCharacterStatus_[i] != 0) {
                useActionParam_->targetCharacterStatus_[i]->clearMenuStatusFlag();
            }
        }
    }
    else {
        partTaskManager.setNextTask(4);
    }
}

ARM void btl::AfterMessageTask::initialize()
{
    btl::BattleActorMacro::setAddMacro(*useActionParam_, currentTarget_);
    message_ = btl::BattleMessage::setAfterMessage(useActionParam_, currentTarget_);

    if (useActionParam_->targetCharacterStatus_[currentTarget_] == 0) {
        return;
    }

    if (btl::BattleActorManager2::getSingleton()->eventType_ == 1) {
        BattleAutoFeed::setAfterMessage();
        int i;
        int drawCtrlId = useActionParam_->targetCharacterStatus_[0]->haveStatusInfo_.drawCtrlId_;
        if (useActionParam_->actorCharacterStatus_->characterType_ == 0) {
            for (i = 0; i < 4; i++) {
                if (i != btl::BattleExecEvent00::getRealVelorinman()) {
                    btl::BattleMonsterDraw2::getSingleton()->monsters_[i].startAnimation(0, 0x1f);
                }
            }
            if (drawCtrlId != btl::BattleExecEvent00::getRealVelorinman()) {
                int real = btl::BattleExecEvent00::getRealVelorinman();
                btl::BattleMonsterDraw2::getSingleton()->monsters_[real].startAnimation(0, 0x1b);
            }
        }
    }

    if (useActionParam_->actionIndex_ == 0x1d7) {
        SoundManager::stopSeWithIndex(0x454, 0x1e);
    }

    if (useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.isAddEffectDamage()) {
        status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
        if (actor->characterType_ == 1) {
            int drawCtrlId = actor->haveStatusInfo_.drawCtrlId_;
            if (actor->haveStatusInfo_.addDamage_ > 0) {
                if (actor->haveStatusInfo_.isDeath()) {
                    btl::BattleMonsterDraw2::getSingleton()->monsters_[drawCtrlId].startAnimation(0, 0x22);
                } else {
                    btl::BattleMonsterDraw2::getSingleton()->monsters_[drawCtrlId].startAnimation(0, 0x23);
                }
                SoundManager::playSe(0x192, 0);
            }
        } else if (actor->haveStatusInfo_.addDamage_ > 0) {
            SoundManager::playSe(0x193, 0);
            MenuAPI::shakeMessage();
        }
        useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.setAddEffectDamage(false);
    }

    if (useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.isAddEffectRecovery()) {
        SoundManager::playSe(0x1f5, 0);
        useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.setAddEffectRecovery(false);
    }

    if (useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.isTargetJouk()) {
        useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.setTargetJouk(false);
    }

    if (useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.isAddEffectMahotora()) {
        if (useActionParam_->actorCharacterStatus_->characterType_ != 1 && useActionParam_->actionIndex_ == 0x47) {
            int idx = btl::BattleEffectManager::getSingleton()->setupEffect(0x1e);
            if (idx < 0) {
                return;
            }
            status::UseActionParam* param = useActionParam_;
            btl::BattleEffectManager* mgr;

            mgr = btl::BattleEffectManager::getSingleton();
            mgr->unit_[idx].setTarget(*param, 0);
        }
        useActionParam_->targetCharacterStatus_[currentTarget_]->haveStatusInfo_.setAddEffectMahotora(false);
    }

    btl::BattleActorAnimation::setAfterAnimation(useActionParam_->actorCharacterStatus_, 0, targetCount_, currentTarget_);
}



ARM void btl::AfterMessageTask::terminate()
{
    btl::BattleActorAnimation::setAfterAnimation2(useActionParam_->actorCharacterStatus_, 0);

    if (useActionParam_->actorCharacterStatus_ == 0) {
        return;
    }
    if (!useActionParam_->actorCharacterStatus_->haveStatusInfo_.isFirstMosyas()) {
        return;
    }
    useActionParam_->actorCharacterStatus_->haveStatusInfo_.setFirstMosyas(false);
}


ARM void btl::AfterMessageTask::execute()
{
    if (useActionParam_->actorCharacterStatus_ != 0) {
        if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.isMonsterChange()) {
            if (btl::BattleEffectManager::getSingleton()->isAllEnd() == 0) {
                return;
            }
        }
    }

    if (message_ != 0) {
        if (BattleAutoFeed::isEndAfterMessage() == 0) {
            return;
        }
        cleanup();
    } else {
        cleanup();
    }
}

