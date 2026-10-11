#include "ov003/btl/SpecialMessageTask.hpp"
#include "ov003/btl/BattleActorMacro.hpp"
#include "main/status/BaseAction.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleExecVictory.hpp"




ARM void btl::SpecialMessageTask::setup(status::UseActionParam* useActionParam)
{
    useActionParam_ = useActionParam;
    currentTarget_ = 0;
}


ARM void btl::SpecialMessageTask::initialize()
{
    char v;
    int id;
    int idx;
    param::EffectParam* effectParam;

    targetCount_ = useActionParam_->targetCount_;

    if (status::BaseAction::multiFlag_ != 0 &&
        useActionParam_->targetCharacterStatus_[currentTarget_] != 0 &&
        currentTarget_ == 1 &&
        useActionParam_->actorCharacterStatus_->characterType_ == PLAYER) {

        if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.haveStatus_.charaIndex_ == 0x8a) {
            id = 0x159;
        }
        else {
            id = btl::BattleEffectManager::getSingleton()->getWeaponEffectID((status::PlayerStatus*)useActionParam_->actorCharacterStatus_);
        }

        effectParam = btl::BattleEffectManager::getSingleton()->getEffectParam(id);

        if (useActionParam_->actorCharacterStatus_->haveStatusInfo_.isSecondKaishin() ||
            useActionParam_->actorCharacterStatus_->damageSound_ == status::CharacterStatus::TsukonSe) {

            v = (effectParam->byte_1 & 0x78) >> 3;
            if (v < 3) {
                v = 2;
            }

            effectParam = btl::BattleEffectManager::getSingleton()->getEffectParam(0x158);
            effectParam->byte_1 &= ~0x78;
            effectParam->byte_1 += (char)((v << 3) & 0x78);
            effectParam->frame = 0;

            if ((idx = btl::BattleEffectManager::getSingleton()->setupEffect(0x158)) >= 0) {
                status::UseActionParam* uap = useActionParam_;
                btl::BattleEffectManager* mgr;

                mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[idx].setTarget(*uap, 0);

                mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[idx].setWaitTime(0);
            }

            useActionParam_->actorCharacterStatus_->haveStatusInfo_.setSecondKaishin(false);
        }
    }

    if (status::BaseAction::tsukonFlag_ != 0 || status::BaseAction::tsukon2Flag_ != 0) {
        btl::BattleActorMacro::setExecMacro(*useActionParam_);
    }

    message_ = btl::BattleMessage::setSpecialMessage(useActionParam_, currentTarget_);
    counter_ = 0;
}


ARM void btl::SpecialMessageTask::terminate()
{
  return;
}

ARM void btl::SpecialMessageTask::execute()
{
    int id;

    counter_++;
    if (counter_ == 4) {
        if (btl::BattleActorManager2::getSingleton()->eventType_ != BattleActorManager2::Velorinman ||
            (id = useActionParam_->targetCharacterStatus_[0]
                      ->haveStatusInfo_.drawCtrlId_,
             id == btl::BattleExecEvent00::getRealVelorinman())) {

            if (useActionParam_->actorCharacterStatus_->damageSound_ ==
                    status::CharacterStatus::KaishinSe) {
                SoundManager::playSe(0x197, 0);
            }
            else if (useActionParam_->actorCharacterStatus_->damageSound_ ==
                     status::CharacterStatus::TsukonSe) {
                if (useActionParam_->actorCharacterStatus_->characterType_ == PLAYER) {
                    SoundManager::playSe(0x197, 0);
                }
                else {
                    SoundManager::playSe(0x196, 0);
                }
            }
        }
    }

    if (BattleAutoFeed::isEndExecuteMessage() || message_ == 0) {
        partTaskManager.setNextTask(5);
    }
}