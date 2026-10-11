#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/HaveEquipment.hpp"
#include "ov003/btl/BattleExecVictory.hpp"
#include "main/dss/ScreenPosition.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

int monsterChangeCount;

THUMB int btl::BattleActorAnimation::setExecAnimation(status::UseActionParam* useActionParam)
{
    int actionIndex = useActionParam->actionIndex_;
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    if (!checkExecAnimation(useActionParam)) {
        return 0;
    }

    if (checkNormalAnimation(useActionParam)) {
        int animIndex;
        int ctrlId = useActionParam->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
        animIndex = useActionParam->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();

        switch (actionIndex) {
        case 417:
        case 418:
        case 430:
        case 489:
            actionIndex = 71;
            animIndex = 0;
            break;
        }

        if (actionIndex == 231 || actionIndex == 232) {
            if (useActionParam->actorCharacterStatus_->damageSound_ != status::CharacterStatus::TsukonSe) {
                actionIndex = 71;
                animIndex = 0;
            }
        }

        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(actionIndex, animIndex);
        return BattleAutoFeed::getMessageSpeed();
    }

    int action = useActionParam->actionIndex_;

    if (useActionParam->result_ != 0) {
        if (actor->isEscapeAnimation()) {
            useActionParam->actorCharacterStatus_->clearEscapeAnimation();
            int ctrlId = useActionParam->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
            int animIndex = useActionParam->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();
            btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(action, animIndex);
            SoundManager::playSe(408, 0);
        }
    }

    if (action == 324) {
        int ctrlId = useActionParam->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
        useActionParam->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(action, 9);
    }

    if (actor->isDisappearAnimation()) {
        actor->clearDisappearAnimation();
        int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 31);
    }

        if (actor->haveStatusInfo_.isKillMyself()) {
        int ctrlId = useActionParam->actorCharacterStatus_->haveStatusInfo_.drawCtrlId_;
        int animIndex = useActionParam->actorCharacterStatus_->haveBattleStatus_.getActionAnimation();
        BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
        mgr->monsters_[ctrlId].startAnimation(action, animIndex);
        mgr = btl::BattleMonsterDraw2::getSingleton();
        mgr->monsters_[ctrlId].setTransOfEnd();
    }

    return 0;
}

THUMB void btl::BattleActorAnimation::setExecAnimationMonster(status::CharacterStatus* actor)
{
    if (actor != 0 && actor != 0) {
        setMonsterChangeSetup(actor);
    }
}

THUMB int btl::BattleActorAnimation::checkExecAnimation(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    if (actor->haveStatusInfo_.isStatusChangeRelease()) {
        return 0;
    }
    if (actor->characterType_ == MONSTER) {
        return 1;
    }
    return 0;
}

THUMB int btl::BattleActorAnimation::checkNormalAnimation(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    if (actor->haveStatusInfo_.isDeath()) {
        if (actor->haveStatusInfo_.isSelfImmolation()) {
            return 1;
        }
        if (actor->haveStatusInfo_.isDamageMyself()) {
            return 1;
        }
        if (actor->haveStatusInfo_.isMahokantaCounter()) {
            return 1;
        }
        if (actor->haveStatusInfo_.isExecuteMeganteRing()) {
            return 1;
        }
    }

    if (actor->haveStatusInfo_.isAttackEnable() == 1) {
        return 1;
    }

    if (actor->haveStatusInfo_.isAstoron()) {
        for (int i = 0; i < useActionParam->targetCount_; i++) {
            useActionParam->targetCharacterStatus_[i]->haveStatusInfo_.setAstoron(false);
        }
        return 1;
    }

    return 0;
}

THUMB void btl::BattleActorAnimation::setResultAnimation(status::UseActionParam* useActionParam, int currentTarget)
{
    int partyIndex;
    short* tai;
    int i;
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[currentTarget];

    if (!checkResultAnimation(useActionParam, currentTarget)) {
        return;
    }

    if (!checkNormalAnimation(useActionParam)) {
        if (actor->haveStatusInfo_.isSleepAttack()) {
            if (target->haveStatusInfo_.effectValue_ > 0) {
                SoundManager::playSe(403, 0);
                return;
            }
            SoundManager::playSe(404, 0);
            return;
        }
        return;
    }

    if (target->characterType_ == PLAYER) {
        setPlayerSE(useActionParam, currentTarget);
    } else {
        int ctrlId = target->haveStatusInfo_.drawCtrlId_;

        if (btl::BattleActorManager2::getSingleton()->eventType_ == 1) {
            if (ctrlId != btl::BattleExecEvent00::getRealVelorinman()) {
                return;
            }
        }

        if (status::UseAction::getDamageType(useActionParam->actionIndex_) == 1) {
            if (target->isDamageAnimation()) {
                int effectValue = target->haveStatusInfo_.effectValue_;
                if (effectValue > 0) {
                    tai = MonsterTaiData[g_monster.getMonsterStatusWithCtrlId(ctrlId)->characterIndex_];

                    for (i = 0; i < g_monster.getCount(); i++) {
                        if (ctrlId == g_monster.getMonsterStatus(i)->haveStatusInfo_.drawCtrlId_) {
                            partyIndex = i;
                            break;
                        }
                    }

                    int* ctrl = (int*)unkfunc_02057128(partyIndex);
                    int a = ctrl[0];
                    int b = ctrl[1];
                    int value = b - status::HaveEquipment::getAbsoluteValue(tai[1] - tai[3]);
                    if (value <= 32) {
                        value = 32;
                    }
                    unkfunc_02050e88(a, value, effectValue, 0);
                    SoundManager::playSe(402, 0);
                }
            }
        }
    }

    setCommonSE(useActionParam, currentTarget);
    setResultAnimationMonster(actor, target, currentTarget);
    setMosyasChange(actor);
    setMonstersDisappear(useActionParam);
}

THUMB void btl::BattleActorAnimation::setPlayerSE(status::UseActionParam* useActionParam, int currentTarget)
{
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[currentTarget];

    switch (status::UseAction::getDamageType(useActionParam->actionIndex_)) {
    case status::UseAction::DamageTypeDamage:
        if (useActionParam->targetResult_[currentTarget] != 0) {
            if (target->haveStatusInfo_.effectValue_ > 0) {
                SoundManager::playSe(403, 0);
                return;
            }
            SoundManager::playSe(404, 0);
            return;
        }
        break;

    case status::UseAction::DamageTypeRecovery:
        SoundManager::playSe(501, 0);
        break;

    case status::UseAction::DamageTypeAgilityChange:
        if (target->haveStatusInfo_.effectValue_ > 0) {
            SoundManager::playSe(506, 0);
        }
        break;

    case status::UseAction::DamageTypeRebirth:
        SoundManager::playSe(553, 0);
        break;

    case status::UseAction::DamageTypeFubaha:
        if (currentTarget == 0) {
            SoundManager::playSe(543, 0);
        }
        break;

    case status::UseAction::DamageTypeBaikiruto:
        SoundManager::playSe(544, 0);
        break;

    case status::UseAction::DamageTypeMahokanta:
        SoundManager::playSe(541, 0);
        break;

    case status::UseAction::DamageTypeDefenceChange:
        switch (useActionParam->actionIndex_) {
        case 39:
        case 40:
        case 41:
        case 42:
        case 479:
            if (target->haveStatusInfo_.effectValue_ > 0) {
                SoundManager::playSe(502, 0);
            }
            break;
        case 37:
        case 38:
            if (target->haveStatusInfo_.effectValue_ > 0) {
                SoundManager::playSe(503, 0);
            }
            break;
        }
        break;

    case status::UseAction::DamageTypeAstoron:
        SoundManager::playSe(547, 0);
        break;

    case status::UseAction::DamageTypeMahosute:
        SoundManager::playSe(559, 0);
        break;

    case status::UseAction::DamageTypePowerSave:
        SoundManager::playSe(618, 0);
        break;

    default:
        if (useActionParam->actionIndex_ == 63) {
            SoundManager::playSe(501, 0);
        }
        break;
    }
}

THUMB void btl::BattleActorAnimation::setCommonSE(status::UseActionParam* useActionParam, int currentTarget)
{
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[currentTarget];
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    param::ActionParam* actionParam = status::excelParam.actionParam_;

    if (useActionParam->actionIndex_ == 371) {
        return;
    }

    switch (status::UseAction::getDamageType(useActionParam->actionIndex_)) {
    case status::UseAction::DamageTypeDamage:
        if (target->haveStatusInfo_.isImmidiateDeath()) {
            return;
        }
        if (actionParam[useActionParam->actionIndex_].magictype != 23) {
            return;
        }
        if (target->haveStatusInfo_.effectValue_ == 0 || target->haveStatusInfo_.isTargetJouk()) {
            if (actor->characterType_ == PLAYER) {
                SoundManager::playSe(405, 0);
                return;
            }
            SoundManager::playSe(404, 0);
            return;
        }
        break;

    case status::UseAction::DamageTypeSubMp:
        if (useActionParam->actionIndex_ != 34) {
            SoundManager::playSe(547, 0);
        }
        break;

    case status::UseAction::DamageTypeNone:
        setCommonSEFromAction(useActionParam);
        break;
    }
}

THUMB void btl::BattleActorAnimation::setCommonSEFromAction(status::UseActionParam* useActionParam)
{
    switch (useActionParam->actionIndex_) {
    case 465:
        SoundManager::playSe(504, 0);
        break;
    case 420:
        SoundManager::playSe(459, 0);
        break;
    case 424:
        SoundManager::playSe(457, 0);
        break;
    case 425:
        SoundManager::playSe(1102, 0);
        break;
    case 432:
        SoundManager::playSe(456, 0);
        break;
    case 440:
        SoundManager::playSe(460, 0);
        break;
    case 444:
        SoundManager::playSe(461, 0);
        break;
    case 453:
        SoundManager::playSe(646, 0);
        break;
    case 457:
        SoundManager::playSe(641, 0);
        break;
    case 456:
        SoundManager::playSe(462, 0);
        break;
    case 483:
        SoundManager::playSe(501, 0);
        break;
    }
}

THUMB int btl::BattleActorAnimation::checkResultAnimation(status::UseActionParam* useActionParam, int currentTarget)
{
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    param::ActionParam* actionParam = status::excelParam.actionParam_;

    if (useActionParam->targetCharacterStatus_[currentTarget] == 0) {
        return false;
    }

    if (actor->haveStatusInfo_.isStatusChangeRelease()) {
        return false;
    }

    if (status::UseAction::getActionType(useActionParam->actionIndex_) == status::UseAction::ActionTypeMagic) {
        if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFizzleZone)) {
            return false;
        }
        if (actor->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahoton)) {
            return false;
        }
    }

    if (actionParam[useActionParam->actionIndex_].useMP != 0) {
        if (actor->haveStatusInfo_.isMpFailure()) {
            actor->haveStatusInfo_.setMpFailure(false);
            return false;
        }
    }

    return true;
}

THUMB void btl::BattleActorAnimation::setResultAnimationMonster(status::CharacterStatus* actor, status::CharacterStatus* target, int currentTarget)
{
    setCallFriend(actor);
    setCallFriend(target);
    setCallFriend();

    if (!actor->haveStatusInfo_.isAttackEnable()
        && !actor->haveStatusInfo_.isSelfImmolation()
        && !actor->haveStatusInfo_.isDamageMyself()
        && !actor->haveStatusInfo_.isMahokantaCounter()
        && !actor->haveStatusInfo_.isExecuteMeganteRing()) {
        return;
    }

    if (target->characterType_ == MONSTER) {
        int ctrlId = target->haveStatusInfo_.drawCtrlId_;

        if (currentTarget == 0 && target->isMultiDamageAnimation()) {
            if (!target->isMultiDamageAnimation1()) {
                return;
            }
            target->clearMultiDamageAnimation1();
            if (target->haveStatusInfo_.effectValueMulti_[0] == 0) {
                return;
            }
            if (target->haveStatusInfo_.isDeath() && !target->isMultiDamageAnimation2nd()) {
                BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].startAnimation(0, 34);
                if (!target->isDeathDisable()) {
                    return;
                }
                mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].startAnimation(0, 35);
                return;
            }
            btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 35);
            return;
        }
        else if (currentTarget == 1 && target->isMultiDamageAnimation()) {
            target->clearMultiDamageAnimation();
            if (target->isMultiDamageAnimation2()) {
                target->clearMultiDamageAnimation2();
                if (target->haveStatusInfo_.effectValueMulti_[1] != 0) {
                    if (target->haveStatusInfo_.isDeath()) {
                        BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
                        mgr->monsters_[ctrlId].startAnimation(0, 34);
                        if (target->isDeathDisable()) {
                            mgr = btl::BattleMonsterDraw2::getSingleton();
                            mgr->monsters_[ctrlId].startAnimation(0, 35);
                        }
                    } else {
                        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 35);
                    }
                }
            }
            target->clearDamageAnimation();
            return;
        }
        else if (target->isDamageAnimation()) {
            target->clearDamageAnimation();
            if (target->haveStatusInfo_.isDeath()) {
                BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].startAnimation(0, 34);
                if (target->isDeathDisable()) {
                    mgr = btl::BattleMonsterDraw2::getSingleton();
                    mgr->monsters_[ctrlId].startAnimation(0, 35);
                }
            } else {
                btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 35);
            }
        }

        if (target->isDeathAnimation()) {
            target->clearDeathAnimation();
            if (target->haveStatusInfo_.isImmidiateDeath()) {
                target->haveStatusInfo_.setImmidiateDeath(false);
                SoundManager::playSe(402, 0);
            }
            BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
            mgr->monsters_[ctrlId].startAnimation(0, 34);
            if (target->isDeathDisable()) {
                mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].startAnimation(0, 35);
            }
        }

        if (target->isDisappearAnimation()) {
            target->clearDisappearAnimation();
            btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 31);
        }

        if (target->isRebirthAnimation()) {
            target->clearRebirthAnimation();
            btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 32);
        }

        if (target->isEscapeAnimation()) {
            SoundManager::playSe(408, 0);
        }
    }
}

THUMB void btl::BattleActorAnimation::setAfterAnimation(status::CharacterStatus* actor, status::CharacterStatus* target, int targetCount, int currentIndex)
{
    if (actor == 0) {
        return;
    }
    if (currentIndex != targetCount - 1) {
        return;
    }
    if (!actor->haveStatusInfo_.isSelfImmolation()) {
        return;
    }
    actor->haveStatusInfo_.setSelfImmolation(false);
    if (actor->characterType_ == MONSTER) {
        int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 31);
    }
}

THUMB void btl::BattleActorAnimation::setAfterAnimation2(status::CharacterStatus* actor, status::CharacterStatus* target){
    setMonsterChange(actor);
}

THUMB void btl::BattleActorAnimation::setMosyasChange(status::CharacterStatus* actor)
{
    if (actor == 0) {
        return;
    }
    if (actor->characterType_ != MONSTER) {
        return;
    }
    if (!actor->haveStatusInfo_.isMosyasExec()) {
        return;
    }
    actor->haveStatusInfo_.setMosyasExec(false);

    int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
    int group = actor->characterGroup_;
    dss::Fix32Vector3 pos = *btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].monsterDraw_.getPosition();

    btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);

    BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
    ctrlId = mgr->setup(group, status::getMosyasIndex());

    btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(pos);

    actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
    actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
}

THUMB void btl::BattleActorAnimation::setMosyasReverse(status::CharacterStatus* actor)
{
    if (actor->haveStatusInfo_.isMosyasRelease()) {
        int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
        int group = actor->characterGroup_;
        dss::Fix32Vector3 pos = *btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].monsterDraw_.getPosition();

        btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);

        ctrlId = btl::BattleMonsterDraw2::getSingleton()->setup(group, 122);

        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(pos);

        actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
        actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
    }
}


THUMB void btl::BattleActorAnimation::setCallFriend(status::CharacterStatus* chara)
{
    if (chara == 0) {
        return;
    }
    if (!chara->haveStatusInfo_.isCallFriend()) {
        return;
    }
    chara->haveStatusInfo_.setCallFriend(false);
    btl::BattleMonsterDraw2::getSingleton()->startAnimationWithLoop(chara->haveStatusInfo_.drawCtrlId_, 32, 1);
}


THUMB void btl::BattleActorAnimation::setCallFriend()
{
    int count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        status::MonsterStatus* monster = g_monster.getMonsterStatus(i);
        if (monster->haveStatusInfo_.isCallFriend()) {
            monster->haveStatusInfo_.setCallFriend(false);
            btl::BattleMonsterDraw2::getSingleton()->startAnimationWithLoop(monster->haveStatusInfo_.drawCtrlId_, 32, 1);
        }
    }
}


THUMB void btl::BattleActorAnimation::gattaiSlimeStart(status::CharacterStatus* actor, int actionIndex)
{
    if (actor == 0) {
        return;
    }
    if (!actor->haveStatusInfo_.isAttackEnable()) {
        return;
    }
    if (actionIndex != 229) {
        return;
    }

    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            int ctrlId = g_monster.getMonsterStatus(i)->haveStatusInfo_.drawCtrlId_;
            if (actor == g_monster.getMonsterStatus(i)) {
                actor->haveStatusInfo_.setMonsterChange(true);
                BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].startGattai();
                dss::Fix32Vector3 pos(0, 0, 0);
                mgr = btl::BattleMonsterDraw2::getSingleton();
                mgr->monsters_[ctrlId].setPosition(pos);
                SoundManager::playSe(705, 0);
                GameMonster::setupTexture(107);
            } else {
                btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].disappearGattaiSlime();
                actor->haveStatusInfo_.setMonsterChange(true);
            }
        }
    }
}

THUMB void btl::BattleActorAnimation::gattaiSlime(status::CharacterStatus* actor, int actionIndex)
{
    if (actor == 0) {
        return;
    }
    if (!actor->haveStatusInfo_.isAttackEnable()) {
        return;
    }
    if (actionIndex != 229) {
        return;
    }
    if (!actor->haveStatusInfo_.isMonsterChange()) {
        return;
    }

    actor->haveBattleStatus_.newBaseChangeMonster(107);
    actor->haveStatusInfo_.setMonsterChange(false);

    int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
    int group = actor->characterGroup_;
    dss::Fix32Vector3 pos = *btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].monsterDraw_.getPosition();

    btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);

    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            if (g_monster.getMonsterStatus(i) != actor) {
                g_monster.getMonsterStatus(i)->haveStatusInfo_.setDisappearFlag(true);
                g_monster.getMonsterStatus(i)->haveStatusInfo_.setDeath();
            }
        }
    }

    actor->characterIndex_ = 107;
    ctrlId = btl::BattleMonsterDraw2::getSingleton()->setup(group, actor->haveBattleStatus_.index_);

    btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(pos);

    actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
    actor->haveStatusInfo_.drawCtrlId_ = ctrlId;
    GameMonster::cleanupTexture(107);
}

THUMB void btl::BattleActorAnimation::setMonsterChangeSetup(status::CharacterStatus* actor)
{
    monsterChangeCount = 0;

    if (actor->haveStatusInfo_.isMonsterChange()) {
        if (!actor->haveStatusInfo_.isDisableTextureCache()) {
            GameMonster::setupTexture(actor->haveBattleStatus_.index_);
        }
        monsterChangeCount = 1;
    }
}

THUMB int btl::BattleActorAnimation::isMonsterChangeSetupEnd()
{
    if (monsterChangeCount == 0) {
        return 1;
    }
    monsterChangeCount = monsterChangeCount + 1;
    if (monsterChangeCount >= 6) {
        return 1;
    }
    return 0;
}


THUMB void btl::BattleActorAnimation::setMonsterChange(status::CharacterStatus* actor)
{
    if (!actor->haveStatusInfo_.isMonsterChange()) {
        return;
    }
    actor->haveStatusInfo_.setMonsterChange(false);

    int ctrlId = actor->haveStatusInfo_.drawCtrlId_;
    BattleMonsterDraw2* mgr = btl::BattleMonsterDraw2::getSingleton();
    int a = mgr->monsters_[ctrlId].screenPosition_;
    int b = btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].screenWidth_;
    int group = actor->characterGroup_;
    dss::Fix32Vector3 pos = *btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].monsterDraw_.getPosition();

    btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);

    int newCtrlId = btl::BattleMonsterDraw2::getSingleton()->setup(group, actor->haveBattleStatus_.index_);

    mgr = btl::BattleMonsterDraw2::getSingleton();
    mgr->monsters_[newCtrlId].setPosition(pos);

    actor->haveStatusInfo_.drawCtrlId_ = newCtrlId;
    actor->haveStatusInfo_.drawCtrlId_ = newCtrlId;

    int index = actor->haveBattleStatus_.index_;
    if (!actor->haveStatusInfo_.isDisableTextureCache()) {
        GameMonster::cleanupTexture(index);
    }
    actor->haveStatusInfo_.setDisableTextureCache(false);

    btl::BattleMonsterDraw2::getSingleton()->monsters_[newCtrlId].screenPosition_ = a;
    btl::BattleMonsterDraw2::getSingleton()->monsters_[newCtrlId].screenWidth_ = b;
}

THUMB void btl::BattleActorAnimation::setMonstersDisappear(status::UseActionParam* useActionParam)
{
    if (useActionParam->actionIndex_ == 477 || useActionParam->actionIndex_ == 528) {
        int count = useActionParam->targetCount_;
        for (int i = 0; i < count; i++) {
            status::CharacterStatus* target = useActionParam->targetCharacterStatus_[i];
            if (target != 0 && target->isDisappearAnimation()) {
                target->clearDisappearAnimation();
                int ctrlId = target->haveStatusInfo_.drawCtrlId_;
                btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 31);
            }
        }
    }
}