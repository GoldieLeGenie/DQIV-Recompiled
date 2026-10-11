#include "ov003/btl/BattleActorEffect.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/param/MonsterAnim.hpp"
#include "main/status/ExcelParam.hpp"

int btl::BattleActorEffect::wait_;

THUMB void btl::BattleActorEffect::setExecEffect(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    wait_ = 0;

    if (checkCommonExecEffect(useActionParam) == 0) {
        return;
    }

    if (useActionParam->actorCharacterStatus_->characterType_ == PLAYER) {
        wait_ = setPlayerEffect(useActionParam);
    } else if (useActionParam->actorCharacterStatus_->characterType_ == MONSTER) {
        wait_ = setEnemyEffect(useActionParam);

        if (status::UseAction::getActionType(useActionParam->actionIndex_) == status::UseAction::ActionTypeMagic) {
            if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahoton)) {
                int w = wait_;
                btl::BattleEffectManager::getSingleton()->wait_ = w;
                return;
            }
        }

        wait_ += setResultEnemyEffect(useActionParam);
    }

    int w = wait_;
    btl::BattleEffectManager::getSingleton()->wait_ = w;
}

THUMB int btl::BattleActorEffect::checkCommonExecEffect(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    param::ActionParam* actionParam = status::excelParam.actionParam_;

    if (actor->haveStatusInfo_.isStatusChangeRelease()) {
        return 0;
    }

    if (actionParam[useActionParam->actionIndex_].useMP != 0) {
        if (actor->haveStatusInfo_.isMpFailure()) {
            return 0;
        }
    }

    if (actor->haveStatusInfo_.isDeath()) {
        if (actor->haveStatusInfo_.isSelfImmolation() == 1) {
            return 1;
        }
        if (actor->haveStatusInfo_.isDamageMyself() == 1) {
            return 1;
        }
        if (actor->haveStatusInfo_.isMahokantaCounter() == 1) {
            return 1;
        }
        if (useActionParam->actionIndex_ == 543) {
            return 1;
        }
    }

    if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
        if (useActionParam->actionIndex_ == 466) {
            return 1;
        }
        if (actor->haveStatusInfo_.isMahokantaCounter() == 1) {
            return 1;
        }
    }

    if (!actor->haveStatusInfo_.isAttackEnable()) {
        return 0;
    }
    return 1;
}
THUMB int btl::BattleActorEffect::setPlayerEffect(status::UseActionParam* useActionParam)
{
    param::ActionParam* actionParam = status::excelParam.actionParam_;
    int camera;
    int wait;
    int actionIndex = useActionParam->actionIndex_;

    if (!checkPlayerExecEffect(useActionParam)) {
        return 0;
    }

    int effectID = actionParam[actionIndex].effectFriend;
    camera = ((actionParam[actionIndex].byte_6 & 0xe0) << 19) >> 24;

    if (effectID == 300) {
        if (useActionParam->targetCharacterStatus_[0]->characterType_ == PLAYER) {
            return 0;
        }
    }

    if (effectID != 0) {
        if (effectID == 300) {
            if (useActionParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_.charaIndex_ == 138) {
                effectID = 345;
            } else {
                effectID = btl::BattleEffectManager::getSingleton()->getWeaponEffectID((status::PlayerStatus*)useActionParam->actorCharacterStatus_);
            }
        }

        param::EffectParam* effectParam = btl::BattleEffectManager::getSingleton()->getEffectParam(effectID);

        if (actionIndex == 484) {
            effectParam->byte_1 = effectParam->byte_1 & ~0x78;
            effectParam->byte_1 = effectParam->byte_1 + 0x18;
        }

        int unitIndex = btl::BattleEffectManager::getSingleton()->setupEffect(effectID);
        if (unitIndex < 0) {
            return 0;
        }

        if (camera == 4 && actionIndex != 471) {
            btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
            mgr->unit_[unitIndex].setTarget(*useActionParam, 1);
        } else {
            btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
            mgr->unit_[unitIndex].setTarget(*useActionParam, 0);
        }

        wait = BattleAutoFeed::getMessageSpeed();
        if (wait < 0) {
            wait = 24;
        }
        btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[unitIndex].setWaitTime(wait);

        if (useActionParam->actorCharacterStatus_->haveStatusInfo_.isFirstKaishin()
            || useActionParam->actorCharacterStatus_->damageSound_ == status::CharacterStatus::TsukonSe) {

            int scale = ((effectParam->byte_1 & 0x78) << 21) >> 24;
            unsigned short frame = effectParam->frame;
            if (scale < 3) {
                scale = 2;
            }
            btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
            param::EffectParam* kaishinParam = mgr->getEffectParam(344);
            kaishinParam->byte_1 = kaishinParam->byte_1 & ~0x78;
            kaishinParam->byte_1 = kaishinParam->byte_1 + (char)((scale << 3) & 0x78);
            kaishinParam->frame = frame;

            unitIndex = btl::BattleEffectManager::getSingleton()->setupEffect(344);
            if (unitIndex >= 0) {
                btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[unitIndex].setTarget(*useActionParam, 0);
                btl::BattleEffectManager::getSingleton()->unit_[unitIndex].setWaitTime(wait);
            }

            useActionParam->actorCharacterStatus_->haveStatusInfo_.setFirstKaishin(false);
        }

        if (camera != 4) {
            btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
            return wait + mgr->unit_[unitIndex].getHitFrame();
        }
    }

    return 0;
}

THUMB int btl::BattleActorEffect::checkPlayerExecEffect(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    if (useActionParam->targetCharacterStatus_[0] == 0) {
        return 0;
    }

    if (useActionParam->actionIndex_ == 0) {
        return 0;
    }

    if (status::UseAction::getActionType(useActionParam->actionIndex_) == status::UseAction::ActionTypeMagic) {
        if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFizzleZone)) {
            return 0;
        }
        if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahoton)) {
            return 0;
        }
    }

    if (useActionParam->actionIndex_ == 371) {
        if (useActionParam->result_ == 0) {
            return 0;
        }
    }

    return 1;
}

THUMB int btl::BattleActorEffect::setEnemyEffect(status::UseActionParam* useActionParam)
{
    int actionIndex = useActionParam->actionIndex_;
    int ctrlId = useActionParam->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
    int animIndex = useActionParam->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();
    int monsterNo = btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].monsterIndex_;

    if (!checkEnemyExecEffect(useActionParam)) {
        return 0;
    }

    if (actionIndex == 417 || actionIndex == 418 || actionIndex == 430) {
        actionIndex = 71;
    }

    int animDataIndex = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, monsterNo, actionIndex, animIndex);
    if (animDataIndex >= 0) {
        param::MonsterAnim* animData = &status::excelParam.monsterAnim_[animDataIndex];
        int effectID = animData->effect;

        if (effectID == 0) {
            return animData->startframe + animData->hitframe;
        }

        param::EffectParam* effectParam = btl::BattleEffectManager::getSingleton()->getEffectParam(effectID);
        effectParam->frame = animData->hitframe;

        char animBits = (animData->byte_1 & 0x3c) >> 2;
        char effectBits = effectParam->byte_1;
        effectParam->byte_1 = effectBits & ~0x78;
        effectParam->byte_1 += (char)((animBits << 3) & 0x78);
        effectParam->scale = animData->scale;

        int unitIndex = btl::BattleEffectManager::getSingleton()->setupEffect(effectID);
        if (unitIndex < 0) {
            return animData->startframe;
        }

        int animfile = animData->animfile;
        status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
        btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[unitIndex].setTarget(actor, animfile);

        int startframe = animData->startframe;
        mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[unitIndex].setWaitTime(startframe);

        return animData->startframe + animData->hitframe;
    }

    return 0;
}

THUMB int btl::BattleActorEffect::checkEnemyExecEffect(status::UseActionParam* useActionParam)
{
    if (useActionParam->targetCharacterStatus_[0] == 0) {
        return 0;
    }
    if (useActionParam->actionIndex_ == 0) {
        return 0;
    }
    return 1;
}

THUMB int btl::BattleActorEffect::setResultEnemyEffect(status::UseActionParam* useActionParam)
{
    param::ActionParam* actionParam = status::excelParam.actionParam_;

    if (!checkEnemyResultEffect(useActionParam)) {
        return 0;
    }

    int effectID = actionParam[useActionParam->actionIndex_].effectEnemy;
    if (effectID != 0) {
        if (effectID == 116) {
            return setMegazaruEffect(useActionParam);
        }

        btl::BattleEffectManager::getSingleton()->getEffectParam(effectID);

        int unitIndex = btl::BattleEffectManager::getSingleton()->setupEffect(effectID);
        if (unitIndex < 0) {
            return 0;
        }

        btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[unitIndex].setTarget(*useActionParam, 0);

        int wait = wait_;
        mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[unitIndex].setWaitTime(wait);

        mgr = btl::BattleEffectManager::getSingleton();
        return mgr->unit_[unitIndex].getHitFrame();
    }

    return 0;
}


THUMB int btl::BattleActorEffect::checkEnemyResultEffect(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    if (useActionParam->targetCharacterStatus_[0] == 0) {
        return 0;
    }
    if (useActionParam->actionIndex_ == 0) {
        return 0;
    }

    if (status::UseAction::getActionType(useActionParam->actionIndex_) == status::UseAction::ActionTypeMagic) {
        if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFizzleZone)) {
            return 0;
        }
    }

    return 1;
}

THUMB int btl::BattleActorEffect::setMegazaruEffect(status::UseActionParam* useActionParam)
{
    unsigned short effectNo;                                                     
    int ret;                                                                 
    int rebirthNum = 0;                                                      
    int normalNum = 0;
                                                  
    effectNo =                                            
    status::excelParam.actionParam_[useActionParam->actionIndex_].effectEnemy;
 
    if (useActionParam->targetCount_ > 6) {
        return 0;
    }
 
    for (int i = 0; i < useActionParam->targetCount_; i++) {                 
        if (useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.isMegazaruRebirth()) {
            rebirthNum++;
        } else {
            normalNum++;
        }
    }
    
    if (normalNum != 0) {

        btl::BattleEffectManager::getSingleton()->getEffectParam(effectNo);
        int index = btl::BattleEffectManager::getSingleton()->setupEffect(effectNo);    
        if (index < 0) {
            return 0;
        }
        for (int i = 0; i < useActionParam->targetCount_; i++) {
            if (useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.isMegazaruRebirth()) {
                btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[index].setFaildTarget(i, 0);
            } else {
                btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[index].setFaildTarget(i, 1);
            }
        }
        btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[index].setTarget(*useActionParam, 0);

        int w = wait_;
        mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[index].setWaitTime(w);
    }
 
    if (rebirthNum != 0) {
        btl::BattleEffectManager::getSingleton()->getEffectParam(effectNo + 1);
        int index = btl::BattleEffectManager::getSingleton()->setupEffect(effectNo + 1);
        if (index < 0) {
            return 0;
        }
        for (int i = 0; i < useActionParam->targetCount_; i++) {
            if (useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.isMegazaruRebirth()) {
                btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[index].setFaildTarget(i, 1);
            } else {
                btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
                mgr->unit_[index].setFaildTarget(i, 0);
            }
        }
        btl::BattleEffectManager* mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[index].setTarget(*useActionParam, 0);

        int w = wait_;
        mgr = btl::BattleEffectManager::getSingleton();
        mgr->unit_[index].setWaitTime(w);

        mgr = btl::BattleEffectManager::getSingleton();
        ret = mgr->unit_[index].getHitFrame();
    }

    for (int i = 0; i < useActionParam->targetCount_; i++) {
        if (useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.isMegazaruRebirth()) {
            useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.setMegazaruRebirth(false);
        }
    }

    return ret;
}