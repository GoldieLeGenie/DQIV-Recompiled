#include "ov003/btl/BattleMonster.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/BaseActionMessage.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/ActionExec.hpp"
#include "main/dss/Random.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownFurniture.hpp"

int status::BaseActionStatus::actionIndex_;
int status::BaseActionStatus::effectValue_;
int status::BaseActionStatus::playerEffectValue_;
int status::BaseActionStatus::monsterEffectValue_;
status::BaseActionFlag status::BaseActionStatus::flag_;
int status::BaseActionStatus::work_;
int status::BaseActionStatus::eventBattle_;
int status::BaseActionStatus::execCallFriend_;
int status::BaseActionStatus::mahokantaMessFlag_;
int status::BaseActionStatus::confusionMessFlag_;
int status::BaseActionStatus::sleepMessFlag_;
int status::BaseActionStatus::path1MessFlag_;
int status::BaseActionStatus::baikirutoMessFlag_;

#pragma explicit_zero_data on
BattleMonsterDrawParam g_BattleMonsterDrawParam = { 0 };
#pragma explicit_zero_data reset

THUMB void status::BaseActionStatus::setEffectValue(int effect, int player, int monster) {
    status::BaseActionStatus::effectValue_  = effect;
    status::BaseActionStatus::playerEffectValue_ = player;
    status::BaseActionStatus::monsterEffectValue_ = monster;
}


THUMB int status::BaseActionStatus::getEffectValue(status::CharacterStatus *target) {
    if (target->characterType_ == PLAYER) {
        return status::BaseActionStatus::playerEffectValue_;
    }
    if (target->characterType_ == MONSTER) {
        return status::BaseActionStatus::monsterEffectValue_;
    }
    return 0;
}

THUMB int status::BaseActionStatus::actionTypeDamage(status::CharacterStatus *target)
{
    if (status::BaseActionStatus::eventBattle_ && target->characterIndex_ == 170 && (target[1].haveStatusInfo_.hp_[0] & 1) == 0) //not sure need to be checked for target[1].haveStatusInfo_.hp_[0]
    {
        return 0;
    }

    int effect = getEffectValue(target);
    int ret;

    if (effect != 0)
    {
        if (status::BaseActionStatus::actionIndex_ == 0x15F && target->haveBattleStatus_.metal_)
        {
            target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -1);
            target->haveStatusInfo_.setDamage(true);
            target->haveStatusInfo_.setUseActionEffectValue(1);
            target->haveStatusInfo_.setDamage(false);
            target->setDamageAnimation();
            ret = 1;
        }
        else
        {
            target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -effect);
            target->haveStatusInfo_.setDamage(true);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
            target->haveStatusInfo_.setDamage(false);
            target->setDamageAnimation();
            ret = 1;
        }
    }
    else
    {
        target->haveStatusInfo_.setUseActionEffectValue(0);
        ret = 0;
    }

    // Dragoram
    if (status::BaseActionStatus::actionIndex_ == 482)
    {
        int hp = target->haveStatusInfo_.getHp();
        int damage = (dssrand::rand(0x9C) + 100) * hp / 128;
        target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -damage);
        target->haveStatusInfo_.setDamage(true);
        target->haveStatusInfo_.setUseActionEffectValue(damage);
        target->haveStatusInfo_.setDamage(false);
        if (!target->haveStatusInfo_.isDeath())
        {
            target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusDragoram, false);
            target->haveBattleStatus_.changeMonsterFromPlayer(196);
        }
        if (damage == 0)
        {
            return 0;
        }
        return 1;
    }

    return ret;
}

THUMB void status::BaseActionStatus::actionTypeInstantDeath(status::CharacterStatus *target)
{
    getEffectValue(target);

    if (status::BaseActionStatus::actionIndex_ == 0x1CF)
    {
        target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -4092);
        target->haveStatusInfo_.setUseActionEffectValue(0);
        target->haveStatusInfo_.haveStatus_.setExp(0);
        target->haveStatusInfo_.haveStatus_.gold_ = 0;
        target->setDeathAnimation();
        return;
    }

    target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -4092);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    target->setDeathAnimation();

    if (target->haveStatusInfo_.getHp() != 0)
    {
        target->clearDeathAnimation();
    }
}

THUMB int status::BaseActionStatus::actionTypeRecovery(status::CharacterStatus *target)
{
    int result = 0;

    if (target->haveStatusInfo_.getHp() < target->haveStatusInfo_.getHpMax())
    {
        int hp = getEffectValue(target);
        target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, hp);
        target->haveStatusInfo_.setUseActionEffectValue(hp);
        result = 1;
    }
    else
    {
        status::BaseActionStatus::playerEffectValue_ = 0;
    }

    return result;
}

THUMB int status::BaseActionStatus::actionTypeAddMP(status::CharacterStatus *target)
{
    int ret = 0;
    unsigned int Mp = target->haveStatusInfo_.getMp();
    if (Mp <  target->haveStatusInfo_.getMpMax())
    {
        int effect = getEffectValue(target);
        int value = dss::clamp<int>(
            effect, 0,
            target->haveStatusInfo_.getMpMax() - target->haveStatusInfo_.getMp());

        target->haveStatusInfo_.addMpInBattle(status::HaveStatusInfo::ResultAction, value);
        target->haveStatusInfo_.setUseActionEffectValue(value);
        ret = 1;
    }

    return ret;
}

THUMB int status::BaseActionStatus::actionTypeSubMP(status::CharacterStatus *actor,status::CharacterStatus *target)
{
    int result = 0;

    int effect = getEffectValue(target);
    if (target->haveStatusInfo_.getMp() < effect)
    {
        effect = target->haveStatusInfo_.getMp();
    }

    if (effect > 0)
    {
        target->haveStatusInfo_.addMpInBattle(status::HaveStatusInfo::ResultAction, -effect);
        target->haveStatusInfo_.setUseActionEffectValue(effect);
        result = 1;
    }
    else if (status::BaseActionStatus::actionIndex_ == 0x215)
    {
        actor->haveStatusInfo_.addMpInBattle(status::HaveStatusInfo::ResultAction, 0);
        actor->haveStatusInfo_.setUseActionEffectValue(0);
        result = 1;
    }

    if (status::BaseActionStatus::actionIndex_ == 0x22)
    {
        if (target->haveStatusInfo_.getMpMax() != 0)
        {
            actor->haveStatusInfo_.addMpInBattle(status::HaveStatusInfo::ResultAction, effect);
            actor->haveStatusInfo_.setUseActionEffectValue(effect);
            result = 1;
        }
        else
        {
            result = 0;
        }
    }

    return result;
}

THUMB int status::BaseActionStatus::actionTypeRebirth(status::CharacterStatus *target)
{
    int ret = 0;
    if (target->characterType_ == MONSTER)
    {
        if (btl::BattleMonsterDraw2::getSingleton()->isCallFriend(target->characterIndex_) == 0)
        {
            return 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x3D || status::BaseActionStatus::actionIndex_ == 0xC8)
    {
        if (dssrand::rand(2) != 0)
        {
            return 0;
        }

        if (target->haveStatusInfo_.isDeath())
        {
            target->haveStatusInfo_.setHp((unsigned short)(target->haveStatusInfo_.getHpMax() / 2));
            target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, 0);
            target->haveStatusInfo_.setUseActionEffectValue(0x3FF);
            target->haveStatusInfo_.statusChange_.clear();
            target->haveStatusInfo_.setStatusChangeRelease(true);
            target->haveStatusInfo_.setZaorikuRebirth(true);
            target->setRebirthAnimation();
            ret = 1;

            if (data_0210bb94.unkfunc_02058114(0xD) && target->characterType_ == MONSTER)
            {
                short log;
                for (int i = 0; i < g_monster.getCount(); i++)
                {
                    if (g_monster.getMonsterStatus(i) == (status::MonsterStatus*)target)
                    {
                        log = btl::BattleActorManager2::getSingleton()->deathLog_;
                        if (log & (1 << g_monster.getCtrlId(i)))
                        {
                            int val = log ^ (1 << g_monster.getCtrlId(i));
                            btl::BattleActorManager2::getSingleton()->deathLog_ = val;
                        }
                    }
                }
            }
        }
        return ret;
    }

    if (target->haveStatusInfo_.isDeath())
    {
        target->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, 0x3FF);
        target->haveStatusInfo_.setUseActionEffectValue(0x3FF);
        target->haveStatusInfo_.statusChange_.clear();
        target->haveStatusInfo_.rebirthFlag_ = 1;
        target->haveStatusInfo_.setStatusChangeRelease(true);
        target->haveStatusInfo_.setZaorikuRebirth(true);
        target->setRebirthAnimation();
        ret = 1;

        if (data_0210bb94.unkfunc_02058114(0xD) &&
            target->characterType_ == MONSTER)
        {
            short log;
            for (int i = 0; i < g_monster.getCount(); i++)
            {
                if (g_monster.getMonsterStatus(i) == (status::MonsterStatus*)target)
                {
                    log = btl::BattleActorManager2::getSingleton()->deathLog_;
                    if (log & (1 << g_monster.getCtrlId(i)))
                    {
                        int val = log ^ (1 << g_monster.getCtrlId(i));
                        btl::BattleActorManager2::getSingleton()->deathLog_ = val;
                    }
                }
            }
        }
    }

    if (target->characterType_ == MONSTER)
    {
        int idx = target->haveStatusInfo_.drawCtrlId_;
        int spacePos   = btl::BattleMonsterDraw2::getSingleton()->spacePos_;
        int spaceWidth = btl::BattleMonsterDraw2::getSingleton()->spaceWidth_;

        dss::Vector3int pos;
        pos.vx = g_BattleMonsterDrawParam.vx_;
        pos.vy = g_BattleMonsterDrawParam.vy_;
        pos.vz = g_BattleMonsterDrawParam.vz_;
        pos.vx = spacePos;

        btl::BattleMonsterDraw2* draw = btl::BattleMonsterDraw2::getSingleton();
        btl::BattleMonster* m = &draw->monsters_[idx];
        m->monsterDraw_.setPositionInt(pos);

        btl::BattleMonsterDraw2::getSingleton()->monsters_[idx].screenPosition_ = spacePos - spaceWidth / 2;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[idx].screenWidth_    = spaceWidth;
    }

    return ret;
}


THUMB void status::BaseActionStatus::actionTypeBaikiruto(status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusBaikiruto))
    {
        status::BaseActionStatus::baikirutoMessFlag_ = 1;
        status::BaseActionStatus::work_ =
            target->haveStatusInfo_.statusChange_.getAgainDisableMessage(status::StatusChange::StatusBaikiruto);
    }

    target->haveStatusInfo_.setAttackChange();
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
}

THUMB void status::BaseActionStatus::actionTypeMahokanta(status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahokanta))
    {
        status::BaseActionStatus::mahokantaMessFlag_ = 1;
    }

    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB void status::BaseActionStatus::actionTypeMahosute(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB void status::BaseActionStatus::actionTypeMahoton(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB int status::BaseActionStatus::actionTypeSleep(status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDefence))
    {
        target->haveStatusInfo_.statusChange_.release(status::StatusChange::StatusDefence);
    }

    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep))
    {
        status::BaseActionStatus::sleepMessFlag_ = 1;
        status::BaseActionStatus::work_ =
            target->haveStatusInfo_.statusChange_.getAgainEnableMessage(status::StatusChange::StatusSleep);
    }

    if (!target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true))
    {
        return 0;
    }

    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    return 1;
}


THUMB void status::BaseActionStatus::actionTypePoison(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB int status::BaseActionStatus::actionTypeSpazz(status::CharacterStatus *target)
{
    if (!target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true))
    {
        return 0;
    }

    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    return 1;
}

THUMB int status::BaseActionStatus::actionTypeDefenceChange(status::CharacterStatus *target)
{
    if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDefenceChange))
    {
        target->haveStatusInfo_.clearDefenceChange();
    }

    int value = target->haveStatusInfo_.setDefenceChange(status::BaseActionStatus::actionIndex_);
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);

    if (value != 0)
    {
        target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
        target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(abs(value));
        return 1;
    }

    return 0;
}

THUMB int status::BaseActionStatus::abs(int value)
{
    if (value < 0)
    {
        value = (short)-value;
    }
    return value;
}

THUMB int status::BaseActionStatus::actionTypeStatusClear(status::CharacterStatus *target)
{
    if (status::BaseActionStatus::actionIndex_ == 0xA2 || status::BaseActionStatus::actionIndex_ == 0x15E)
    {
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusPoison))
        {
            return 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xA5 || status::BaseActionStatus::actionIndex_ == 0x161)
    {
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz))
        {
            target->haveStatusInfo_.setStatusChangeRelease(true);
            return 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x40 || status::BaseActionStatus::actionIndex_ == 0xCA)
    {
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusPoison))
        {
            target->haveStatusInfo_.setStatusChangeRelease(true);
            return 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x41 || status::BaseActionStatus::actionIndex_ == 0x41)
    {
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz))
        {
            return 0;
        }
        if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz))
        {
            target->haveStatusInfo_.setStatusChangeRelease(true);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x46 || status::BaseActionStatus::actionIndex_ == 0x46)
    {
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep))
        {
            return 0;
        }
        if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep))
        {
            target->haveStatusInfo_.setStatusChangeRelease(true);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x6A || status::BaseActionStatus::actionIndex_ == 0x154)
    {
        if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu))
        {
            target->haveStatusInfo_.statusChange_.release(status::StatusChange::StatusMosyasu);
            target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
            target->setMosyasRelease();
            target->haveStatusInfo_.setStatusChangeRelease(true);
        }

        if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDragoram))
        {
            target->haveStatusInfo_.statusChange_.release(status::StatusChange::StatusDragoram);
            target->haveBattleStatus_.changeMonsterReverse();
            target->haveStatusInfo_.setStatusChangeRelease(true);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x15B)
    {
        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x5A))
        {
            return 0;
        }
        if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion))
        {
            return 0;
        }
    }

    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    return 1;
}

THUMB int status::BaseActionStatus::actionTypeAgilityChange(status::CharacterStatus *target)
{
    if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusAgilityChange))
    {
        target->haveStatusInfo_.clearAgilityChange();
    }

    int value = target->haveStatusInfo_.setAgilityChange();
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);

    if (value != 0)
    {
        target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
        target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(value);
        return 1;
    }

    return 0;
}

THUMB void status::BaseActionStatus::actionTypeAstoron(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);

    if (target->characterType_ == PLAYER)
    {
        target->haveStatusInfo_.statusChange_.setTurn(status::StatusChange::StatusAstoron, 3);
    }

    if (target->characterType_ == MONSTER)
    {
        target->haveStatusInfo_.statusChange_.setTurn(status::StatusChange::StatusAstoron, 4);
    }

    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    target->haveStatusInfo_.setAstoron(true);
}

THUMB void status::BaseActionStatus::actionTypeConfusion(status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion))
    {
        status::BaseActionStatus::confusionMessFlag_ = 1;
        status::BaseActionStatus::work_ =
            target->haveStatusInfo_.statusChange_.getAgainEnableMessage(status::StatusChange::StatusConfusion);
    }

    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB void status::BaseActionStatus::actionTypeDragoram(status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusDragoram))
    {
        target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
        target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(0);
        return;
    }

    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
    target->haveBattleStatus_.changeMonsterFromPlayer(0xC3);
}

THUMB void status::BaseActionStatus::actionTypeManusa(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB void status::BaseActionStatus::actionTypeFubaha(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB void status::BaseActionStatus::actionTypeFeather(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
    target->haveStatusInfo_.setUseActionEffectValue(0);
}

THUMB int status::BaseActionStatus::actionTypeLight(status::CharacterStatus *target)
{
    if (target->characterType_ == PLAYER)
    {
        if (dssrand::rand(8) < 5)
        {
            target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
            target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
            target->haveStatusInfo_.setUseActionEffectValue(0);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
        target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(0);
    }

    return 1;
}

THUMB void status::BaseActionStatus::actionTypeDefence(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
}


THUMB void status::BaseActionStatus::actionTypePowerSave(status::CharacterStatus *target)
{
    target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
}


THUMB int status::BaseActionStatus::actionTypeMosyas(status::CharacterStatus *actor,
                                               status::CharacterStatus *target)
{
    if (target->haveStatusInfo_.isMosyasTarget())
    {
        return 0;
    }
    target->haveStatusInfo_.setMosyasTarget(true);

    int monsterIndex = 0;
    int mosyasIndex  = 0;
    unsigned int index = target->haveStatusInfo_.haveStatus_.playerIndex_;
    switch (index)
    {
        case 1:  monsterIndex = 0xFA;  mosyasIndex = 0xD7; break;
        case 2:  monsterIndex = 0xFB;  mosyasIndex = 0xD8; break;
        case 3:  monsterIndex = 0xFC;  mosyasIndex = 0xD9; break;
        case 4:  monsterIndex = 0xFD;  mosyasIndex = 0xDA; break;
        case 5:  monsterIndex = 0xFE;  mosyasIndex = 0xDB; break;
        case 6:  monsterIndex = 0xFF;  mosyasIndex = 0xDC; break;
        case 7:  monsterIndex = 0x100; mosyasIndex = 0xDD; break;
        case 8:  monsterIndex = 0x101; mosyasIndex = 0xDE; break;
        case 9:  monsterIndex = 0x102; mosyasIndex = 0xDF; break;
        case 10: case 11: case 12: case 13: case 14:
        case 15: case 16: case 17: case 18: case 19:
        case 20: case 21: case 22: case 23: case 24: break;
        case 25: monsterIndex = 0x103; mosyasIndex = 0xE0; break;
    }

    actor->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
    actor->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);

    int hp    = actor->haveStatusInfo_.getHp();
    int hpMax = actor->haveStatusInfo_.getHpMax();
    int mp    = actor->haveStatusInfo_.getMp();
    int mpMax = actor->haveStatusInfo_.getMpMax();

    actor->setMosyasChange(monsterIndex);

    actor->haveStatusInfo_.setHpMax(hpMax);
    actor->haveStatusInfo_.setHp(hp);
    actor->haveStatusInfo_.setMpMax(mpMax);
    actor->haveStatusInfo_.setMp(mp);

    ((status::MonsterStatus *)actor)->mosyasIndex_  = target->haveStatusInfo_.haveStatus_.playerIndex_;
    ((status::MonsterStatus *)actor)->mosyasTarget_ = target;

    int agl = target->haveStatusInfo_.getAgility(0);
    actor->haveStatusInfo_.setAgility(agl);
    int protection = target->haveStatusInfo_.getProtection(0);
    actor->haveStatusInfo_.setProtection(protection);
    actor->haveStatusInfo_.setAttack(target->haveStatusInfo_.getAttack(0));
    actor->haveStatusInfo_.setDefence(target->haveStatusInfo_.getDefence(0));

    int count = dss::clamp<int>(target->haveStatusInfo_.haveAction_.getCount(), 0, 14);
    actor->haveBattleStatus_.clearMosyasAction();

    int i = 0;
    if (count > 0)
    {
        status::HaveStatusInfo *info = &target->haveStatusInfo_;
        if (count > 0)
        {
            status::HaveAction *act = &info->haveAction_;
            if (count > 0)
            {
                do
                {
                    unsigned short action = act->getAction(i);
                    if (status::UseAction::isBattleUse(action) &&
                        status::UseAction::isMosyasAction(action) &&
                        action != 0x8C && action != 0x8E)
                    {
                        actor->haveBattleStatus_.setMosyasAction(action);
                    }
                    i++;
                } while (i < count);
            }
        }
    }
    actor->haveBattleStatus_.setMosyasAction(0x47);

    actor->haveStatusInfo_.setMosyasExec(true);
    status::setMosyasIndex(mosyasIndex);
    return 1;
}


THUMB int status::BaseActionStatus::actionTypeNone(status::CharacterStatus *actor, status::CharacterStatus *target)
{
    int ret = 1;
    char effect = status::BaseActionStatus::playerEffectValue_;
    int monsterNo;
    int count;
    int count2;
    int spacePos;
    int spaceWidth;
    unsigned short* pi;

    if (status::BaseActionStatus::actionIndex_ == 0x1A || status::BaseActionStatus::actionIndex_ == 0x14A
        || status::BaseActionStatus::actionIndex_ == 0x158 || status::BaseActionStatus::actionIndex_ == 0x220) {
        characterClearOut(target, (MonsterDrop)0);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1B) {
        if (dssrand::rand(2) == 0) {
            target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, -0x3FF);
            target->haveStatusInfo_.setUseActionEffectValue(0);
        } else {
            int hp = target->haveStatusInfo_.getHp();
            short keep = (short)dss::clamp<int>((short)(hp * 5 / 100 - 1), 1, 0x3FF);
            hp = hp - keep;
            target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, -hp);
            target->haveStatusInfo_.setDamage(true);
            target->haveStatusInfo_.setUseActionEffectValue(hp);
            target->haveStatusInfo_.setDamage(false);
        }
        if (target->characterType_ == MONSTER) {
            target->haveStatusInfo_.haveStatus_.setExp(0);
            target->haveStatusInfo_.haveStatus_.gold_ = 0;
            target->setDamageAnimation();
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x3F || status::BaseActionStatus::actionIndex_ == 0x202) {
        int ok = 1;
        int wasZero = 0;
        if (target->characterType_ == MONSTER) {
            if (btl::BattleMonsterDraw2::getSingleton()->isCallFriend(target->characterIndex_) != 0) {
                ok = 1;
            } else {
                ok = 0;
                ret = 0;
            }
        }
        if (ok != 0) {
            status::BaseActionStatus::flag_.flag_ |= 4;
            if (target->haveStatusInfo_.isDeath() != 0) {
                status::BaseActionStatus::flag_.flag_ |= 1;
                target->haveStatusInfo_.setStatusChangeRelease(true);
            } else {
                status::BaseActionStatus::flag_.flag_ |= 2;
                target->haveStatusInfo_.setMegazaruRecovery(true);
            }
            if (status::BaseActionStatus::actionIndex_ == 0x3F) {
                actor->haveStatusInfo_.setSelfImmolation(true);
            }
            if (target->haveStatusInfo_.getHp() == 0) {
                wasZero = 1;
            }
            target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, 0x3FF);
            target->haveStatusInfo_.rebirthFlag_ = 1;
            if (actor != target) {
                target->setRebirthAnimation();
            }
            if (actor->characterType_ == MONSTER) {
                target->haveStatusInfo_.haveStatus_.setExp(0);
                target->haveStatusInfo_.haveStatus_.gold_ = 0;
            }
            ret = 1;
            if (target->characterType_ == MONSTER && actor != target && wasZero != 0) {
                target->haveStatusInfo_.setMegazaruRebirth(ret);
                spacePos   = btl::BattleMonsterDraw2::getSingleton()->spacePos_;
                spaceWidth = btl::BattleMonsterDraw2::getSingleton()->spaceWidth_;
                dss::Vector3int pos;
                pos.vx = g_BattleMonsterDrawParam.unk_04;   // ldr [r0, #0x4]
                pos.vy = g_BattleMonsterDrawParam.unk_00;   // ldr [r0, #0x0]
                pos.vz = g_BattleMonsterDrawParam.unk_14;   // ldr [r0, #0x14]
                pos.vx = spacePos;               
                int idx = target->haveStatusInfo_.drawCtrlId_;
                btl::BattleMonsterDraw2* draw = btl::BattleMonsterDraw2::getSingleton();          
                draw->monsters_[idx].monsterDraw_.setPositionInt(pos);
                btl::BattleMonsterDraw2::getSingleton()->monsters_[idx].screenPosition_ = spacePos - spaceWidth / 2;
                btl::BattleMonsterDraw2::getSingleton()->monsters_[idx].screenWidth_ = spaceWidth;
            }
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x61) {
        target->haveStatusInfo_.statusChange_.clear();
        target->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusFizzleZone, 0);
        target->haveStatusInfo_.setStatusChangeInBattle(HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(0);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x10A) {
        if (target->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusPath1) != 0) {
            status::BaseActionStatus::path1MessFlag_ = 1;
            status::BaseActionStatus::work_ = target->haveStatusInfo_.statusChange_.getAgainEnableMessage(StatusChange::StatusPath1);
        }
        target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
        target->haveStatusInfo_.setStatusChangeInBattle(HaveStatusInfo::ResultAction);
        target->haveStatusInfo_.setUseActionEffectValue(0);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x155) {
        if (dssrand::rand(8) == 0) {
            target->haveStatusInfo_.statusChange_.setup(status::BaseActionStatus::actionIndex_, true);
            target->haveStatusInfo_.setStatusChangeInBattle(HaveStatusInfo::ResultAction);
            target->haveStatusInfo_.setUseActionEffectValue(0);
            ret = 1;
        } else {
            ret = 0;
        }
    }

    int action = status::BaseActionStatus::actionIndex_;
    if (action == 0xA3 || action == 0xCF) {
        encount::Encount::getSingleton()->disableEncount(action);
    }
    action = status::BaseActionStatus::actionIndex_;
    if (action == 0xA6) {
        encount::Encount::getSingleton()->easyEncount(action);
    }
    action = status::BaseActionStatus::actionIndex_;
    if (action == 0xD7) {
        encount::Encount::getSingleton()->differentEncount(action);
    }

    if (status::BaseActionStatus::actionIndex_ == 0xD4) {
        if (data_0210bb94.unkfunc_02058114(0xc) == 0) {
            ret = 0;
        } else {
            if (TownFurnitureManager::getSingleton()->searchFloorItem() == 0) {
                ret = 0;
            }
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xD8) {
        if (g_Stage.isEncount() != 0 && encount::Encount::getSingleton()->enable_ != 0) {
            if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
                if (TownPlayerManager::getSingleton()->isSaveAndBattleOK() == 0) {
                    ret = 0;
                } else {
                    encount::Encount::getSingleton()->forceEncount();
                    ret = 1;
                }
            } else {
                encount::Encount::getSingleton()->forceEncount();
                ret = 1;
            }
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x16A) {
        if (target->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusMosyasu) != 0) {
            target->haveStatusInfo_.statusChange_.release(StatusChange::StatusMosyasu);
            target->haveStatusInfo_.setStatusChangeInBattle(HaveStatusInfo::ResultAction);
            target->setMosyasRelease();
            target->haveStatusInfo_.setStatusChangeRelease(true);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x9F || status::BaseActionStatus::actionIndex_ == 0x147) {
        ret = 0;
    }

    if (status::BaseActionStatus::actionIndex_ == 0xA7 || status::BaseActionStatus::actionIndex_ == 0x162) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.baseStatus_.strength_ != 0xFF) {
            target->haveStatusInfo_.haveStatus_.addStrength(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xB9 || status::BaseActionStatus::actionIndex_ == 0x163) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.getAgility() != 0xFF) {
            target->haveStatusInfo_.haveStatus_.AddAgility(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xBA || status::BaseActionStatus::actionIndex_ == 0x164) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.getWisdom() != 0xFF) {
            target->haveStatusInfo_.haveStatus_.addWisdom(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xBB || status::BaseActionStatus::actionIndex_ == 0x165) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.baseStatus_.protection_ != 0xFF) {
            target->haveStatusInfo_.haveStatus_.addProtection(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xBC || status::BaseActionStatus::actionIndex_ == 0x166) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.getHpMax() != 9999) {
            target->haveStatusInfo_.addHpMax(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xA8 || status::BaseActionStatus::actionIndex_ == 0x167) {
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
            ret = 0;
        } else if (target->haveStatusInfo_.haveStatus_.getMpMax() != 999
                   && target->haveStatusInfo_.haveStatus_.getMpMax() != 0) {
            target->haveStatusInfo_.haveStatus_.addMpMax(effect);
            target->haveStatusInfo_.setUseActionEffectValue(effect);
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x104 && actor->characterType_ == MONSTER) {
        characterClearOut(actor, (MonsterDrop)0);
        if (actor->haveStatusInfo_.isDeath() != 0) {
            actor->haveStatusInfo_.setDisappearFlag(false);
            actor->haveStatusInfo_.setEscapeFlag(true);
            actor->haveBattleStatus_.escape_ = 1;
            actor->setEscapeAnimation();
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0xCB || status::BaseActionStatus::actionIndex_ == 0xA4
        || status::BaseActionStatus::actionIndex_ == 0xAF) {
        ret = execRula();
    }
    if (status::BaseActionStatus::actionIndex_ == 0xCC) {
        ret = execRiremito();
    }
    if (status::BaseActionStatus::actionIndex_ == 0xCD) {
        ret = execImpas();
    }
    if (status::BaseActionStatus::actionIndex_ == 0xD0) {
        ret = execLanaruta();
    }

    if (status::BaseActionStatus::actionIndex_ == 0xB2) {
        ret = 0;
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            if (g_Stage.getTimeZone() == TIME_ZONE_DAYTIME || g_Stage.getTimeZone() == TIME_ZONE_EVENING) {
                ret = execLanaruta();
            } else {
                ret = 0;
            }
        } else if (data_0210bb94.unkfunc_02058114(0xe) != 0) {
            if (g_Stage.getWorldTime() < 0x840) {
                ret = execLanaruta();
            } else {
                ret = 0;
            }
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x148) {
        if (actor->haveStatusInfo_.haveStatus_.playerIndex_ == 8) {
            ret = 1;
            actor->haveStatusInfo_.setSilverTarot(ret);
        } else {
            ret = 0;
            actor->haveStatusInfo_.setSilverTarot(ret);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x10E) {
        int monsterNo = actor->characterIndex_;   
        int group = actor->characterGroup_;

        initCallMonster();
        ret = callDifferentMonster(group, monsterNo);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x10F) {
        int monsterNo = actor->characterIndex_;
        int group = actor->characterGroup_;
        initCallMonster();
        ret = callDifferentMonster(group, monsterNo);
        if (ret != 0) {
            status::BaseActionStatus::execCallFriend_ = 1;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x110) {
        monsterNo = actor->characterIndex_;       
        int group = actor->characterGroup_;
        initCallMonster();
        int n = 0;
        do {
            if (callDifferentMonster(group, monsterNo) == 0) {
                break;
            }
            n++;
        } while (true);
        if (n != 0) {
            ret = 1;
        } else {
            ret = 0;
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x111) { initCallMonster(); ret = callDifferentMonster(-1, 0xC); }
    if (status::BaseActionStatus::actionIndex_ == 0x112) { initCallMonster(); ret = callDifferentMonster(-1, 0x27); }
    if (status::BaseActionStatus::actionIndex_ == 0x113) { initCallMonster(); ret = callDifferentMonster(-1, 0x4C); }
    if (status::BaseActionStatus::actionIndex_ == 0x114) { initCallMonster(); ret = callDifferentMonster(-1, 0x31); }
    if (status::BaseActionStatus::actionIndex_ == 0x115) { initCallMonster(); ret = callDifferentMonster(-1, 0x47); }
    if (status::BaseActionStatus::actionIndex_ == 0x116) { initCallMonster(); ret = callDifferentMonster(-1, 0x69); }
    if (status::BaseActionStatus::actionIndex_ == 0x117) { initCallMonster(); ret = callDifferentMonster(-1, 0x46); }
    if (status::BaseActionStatus::actionIndex_ == 0x118) { initCallMonster(); ret = callDifferentMonster(-1, 0x86); }
    if (status::BaseActionStatus::actionIndex_ == 0x119) { initCallMonster(); ret = callDifferentMonster(-1, 0x4F); }
    if (status::BaseActionStatus::actionIndex_ == 0x11A) { initCallMonster(); ret = callDifferentMonster(-1, 0xEB); }

    if (status::BaseActionStatus::actionIndex_ == 0x1D2) {
        target->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusSleep, 1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1D4) {
        target->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusPath1, 1);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1D3) {
        g_Party.setBattleMode();
        int cnt = g_Party.getCount();
        for (int i = 0; i < cnt; i++) {
            if (actor != (status::CharacterStatus*)g_Party.getPlayerStatus(i)) {
                ((status::CharacterStatus*)g_Party.getPlayerStatus(i))->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusTimeStop, 1);
            }
        }
        int cnt2 = g_monster.getCount();
        for (int i = 0; i < cnt2; i++) {
            ((status::MonsterStatus*)g_monster.getMonsterStatus(i))->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusTimeStop, 1);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1E3) {
        if (target->haveStatusInfo_.isDeath() != 0) {
            target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, 0x3FF);
            target->haveStatusInfo_.rebirthFlag_ = 1;
            target->setRebirthAnimation();
            target->haveStatusInfo_.setStatusChangeRelease(true);
        } else {
            target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, 0x3FF);
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1DD) {
        characterClearOut(target, (MonsterDrop)0);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1D0) {
        StatusChange::setupFizzleZone();
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1CB && target->characterType_ == MONSTER) {
        characterClearOut(target, (MonsterDrop)0);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1D1) {
        target->haveStatusInfo_.setAllKaishin(true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1A2) {
        target->haveStatusInfo_.setDamageMyself(true);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1AA) {
        int order[4] = { 0, 0, 0, 0 };
        g_Party.setBattleMode();
        count = g_Party.getCount();
        if (count > 4) {
            count = 4;
        }
        int i = 0;
        if (0 < count) {
            status::CharacterStatus* p = actor;
            pi = &p->haveStatusInfo_.haveStatus_.playerIndex_;
            if (0 < count) {
                int* dst = order;
                do {
                    int pidx = g_Party.getPlayerIndex(i);
                    if (pidx != *pi) {
                        *dst = pidx;
                        dst++;
                    }
                    i++;
                } while (i < count);
            }
        }
        g_Party.reorder(order[0], order[1], order[2], order[3]);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1BC) {
        status::BaseActionStatus::work_ = actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_ARMOR);
        actor->haveStatusInfo_.resetEquipment2(ITEM_ARMOR);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1B8) {
        target->haveStatusInfo_.statusChange_.setup(0x1B8, true);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1C2) {
        int order[4] = { 0, 0, 0, 0 };
        g_Party.setBattleMode();
        count2 = g_Party.getCount();
        if (count2 > 4) {
            count2 = 4;
        }
        int i = 0;
        if (0 < count2) {
            int* dst = order;
            do {
                if (g_Party.isInsideCarriage(i) == 0) {
                    int pidx = ((status::CharacterStatus*)g_Party.getPlayerStatus(i))->haveStatusInfo_.haveStatus_.playerIndex_;
                    if (pidx != actor->haveStatusInfo_.haveStatus_.playerIndex_) {
                        *dst = pidx;
                        dst++;
                    }
                }
                i++;
            } while (i < count2);
        }
        g_Party.reorder(actor->haveStatusInfo_.haveStatus_.playerIndex_, order[0], order[1], order[2]);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x1C8) {
        target->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusSpazz, 1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1C9) {
        characterClearOut(actor, (MonsterDrop)0);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x1CA) {
        characterClearOut(actor, (MonsterDrop)0);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x11E) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0xD4);
        actor->haveStatusInfo_.setMonsterChange(true);
        actor->haveStatusInfo_.setDisableTextureCache(true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x11F) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0xD5);
        actor->haveStatusInfo_.setMonsterChange(true);
        actor->haveStatusInfo_.setDisableTextureCache(true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x120) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0xD6);
        actor->haveStatusInfo_.setMonsterChange(true);
        actor->haveStatusInfo_.setDisableTextureCache(true);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x210) {
        characterClearOut(target, Gold);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x211) {
        ret = 0;
    }

    if (status::BaseActionStatus::actionIndex_ == 0x213) {
        actor->haveStatusInfo_.statusChange_.setup(0x212, true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x213) {
        target->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusPowerSave, 0);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x214) {
        int item = ((status::MonsterStatus*)target)->getHaveDropItem();
        if (item != 0) {
            if (actor->haveStatusInfo_.haveItem_.getCount() != 0xC) {
                actor->haveStatusInfo_.haveItem_.add(item);
            } else {
                g_Party.haveItemSack_.add(item);
            }
            int charaNo = target->characterIndex_;
            int found = -1;
            int i = 0;
            param::BookData* p = excelParam.bookData_;
            do {
                if (charaNo == p->name) {
                    found = i;
                    break;
                }
                i++;
                p++;
            } while (i < 0xD2);
            if (found != -1) {
                g_BattleResult.addItemCount(found);
            }
        }
    }

    if (status::BaseActionStatus::actionIndex_ == 0x216) {
        target->haveStatusInfo_.addHpInBattle(HaveStatusInfo::ResultAction, -effect);
        target->haveStatusInfo_.setDamage(true);
        target->haveStatusInfo_.setUseActionEffectValue(effect);
        target->haveStatusInfo_.setDamage(false);
        target->setDamageAnimation();
    }

    if (status::BaseActionStatus::actionIndex_ == 0x218) {
        target->haveStatusInfo_.statusChange_.setup(0x218, true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x219) {
        target->haveStatusInfo_.statusChange_.setup(0x219, true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x21A) {
        target->haveStatusInfo_.statusChange_.setup(0x21A, true);
    }

    if (status::BaseActionStatus::actionIndex_ == 0x22B) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x12C);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x22C) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x12D);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x22D) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x12E);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x22E) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x12F);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x22F) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x132);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
        actor->haveStatusInfo_.setEvilPriest(true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x230) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x133);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
        actor->haveStatusInfo_.setEvilPriest(true);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x231) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x134);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }
    if (status::BaseActionStatus::actionIndex_ == 0x232) {
        actor->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x135);
        actor->haveStatusInfo_.setMonsterChange(true);
        setMonsterChange(1);
    }

    return ret;
}





THUMB void status::BaseActionStatus::characterClearOut(status::CharacterStatus *chara, status::BaseActionStatus::MonsterDrop drop)
{
    if (drop == GoldExp)
    {
        chara->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -1023);
        chara->haveStatusInfo_.setUseActionEffectValue(0);
    }
    else if (drop == Gold )
    {
        chara->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -1023);
        chara->haveStatusInfo_.haveStatus_.setExp(0);
        chara->haveStatusInfo_.setUseActionEffectValue(0);
    }
    else
    {
        chara->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::ResultAction, -1023);
        chara->haveStatusInfo_.haveStatus_.setExp(0);
        chara->haveStatusInfo_.haveStatus_.gold_ = 0;
        chara->haveStatusInfo_.setUseActionEffectValue(0);
    }

    if (chara->haveStatusInfo_.isDeath())
    {
        chara->haveStatusInfo_.setDisappearFlag(true);
        chara->setDisappearAnimation();
    }
}

THUMB void status::BaseActionStatus::setMonsterChange(int flag)
{
    if (flag)
    {
        status::BaseActionStatus::flag_.flag_ |= 0x10;
    }
    else
    {
        status::BaseActionStatus::flag_.flag_ &= ~0x10;
    }
}

THUMB bool status::BaseActionStatus::isMonsterChange()
{
    if (status::BaseActionStatus::flag_.flag_ & 0x10)
    {
        return true;
    }
    return false;
}


