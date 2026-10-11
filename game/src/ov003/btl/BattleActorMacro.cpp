#include "ov003/btl/BattleActorMacro.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "main/encount/Encount.hpp"
#include "ov003/status/MonsterStatus.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"

THUMB void btl::BattleActorMacro::setExecMacro(status::UseActionParam& useActionParam)
{
    int actionIndex = useActionParam.actionIndex_;
    status::CharacterStatus* actor = useActionParam.actorCharacterStatus_;
    status::CharacterStatus* target = useActionParam.targetCharacterStatus_[0];
    status::UseActionMacro::setExecMacro(actor, target, actionIndex);
    setMacroActor(actor, actionIndex);
    setMacroTarget(target, actionIndex, 0);
}

THUMB void btl::BattleActorMacro::setResultMacro(status::UseActionParam& useActionParam, int targetIndex)
{
    status::CharacterStatus* target = useActionParam.targetCharacterStatus_[targetIndex];
    status::CharacterStatus* actor = useActionParam.actorCharacterStatus_;
    int actionIndex = useActionParam.actionIndex_;
    if (target != 0) {
        status::UseActionMacro::setResultMacro(actor, target, actionIndex);
    }
    setMacroActor(actor, actionIndex);
    setMacroTarget(target, actionIndex, targetIndex);
}

THUMB void btl::BattleActorMacro::setMacroActor(status::CharacterStatus* actor, int actionIndex)
{
    if (actor == 0) {
        return;
    }

    if (actor->characterType_ == PLAYER) {
        TextAPI::setMACRO0(1, 0x50000000, actor->haveStatusInfo_.haveStatus_.playerIndex_);
    }
    if (actor->characterType_ == MONSTER) {
        status::MonsterStatus* monster = (status::MonsterStatus*)actor;
        int monsterIndex = monster->characterIndex_;
        if (g_monster.getMonsterCountDeadOrAlive(monsterIndex) == 1 && encount::Encount::getSingleton()->getMonsterCountName(monsterIndex) == 0) {
            TextAPI::setMACRO0(1, 0x60000000, monsterIndex);
        } else {
            TextAPI::setMACRO0(1, 0x60000000, monsterIndex, monster->sortIndex_);
        }
        if (!monster->haveStatusInfo_.isFirstMosyas() && monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
            TextAPI::setMACRO0(1, 0x50000000, monster->mosyasIndex_, 1, -1);
        }
    }
    if (actor->haveStatusInfo_.addDamage_ != 0) {
        TextAPI::setMACRO2(0x2b, 0xf0000000, actor->haveStatusInfo_.addDamage_);
    }
}

THUMB void btl::BattleActorMacro::setMacroTarget(status::CharacterStatus* target, int actionIndex, int targetIndex)
{
    if (target == 0) {
        return;
    }

    if (target->characterType_ == PLAYER) {
        TextAPI::setMACRO0(0x12, 0x50000000, target->haveStatusInfo_.haveStatus_.playerIndex_);
    }
    if (target->characterType_ == MONSTER) {
        status::MonsterStatus* monster = (status::MonsterStatus*)target;
        int monsterIndex = monster->characterIndex_;
        if (g_monster.getMonsterCountDeadOrAlive(monsterIndex) == 1 && encount::Encount::getSingleton()->getMonsterCountName(monsterIndex) == 0) {
            TextAPI::setMACRO0(0x12, 0x60000000, monsterIndex);
        } else {
            TextAPI::setMACRO0(0x12, 0x60000000, monsterIndex, monster->sortIndex_);
        }
        if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
            TextAPI::setMACRO0(0x12, 0x50000000, monster->mosyasIndex_, 1, -1);
        }
    }
    TextAPI::setMACRO0(0x2b, 0xf0000000, target->haveStatusInfo_.effectValue_);
    if (target->haveStatusInfo_.isMahokantaCounter()) {
        TextAPI::setMACRO0(0x2b, 0xf0000000, target->haveStatusInfo_.mahokantaEffectValue_[targetIndex]);
    }
}

THUMB void btl::BattleActorMacro::setAddMacro(status::UseActionParam& useActionParam, int targetIndex)
{
    status::UseActionMacro::setAddMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[targetIndex], useActionParam.actionIndex_);
    setMacroActor(useActionParam.actorCharacterStatus_, 0);
    setMacroTarget(useActionParam.targetCharacterStatus_[targetIndex], 0, 0);
}
