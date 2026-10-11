#include "ov003/btl/BattleMonster.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionMessage.hpp"
#include "main/status/MonsterStatus.hpp"
#include "main/dss/Random.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownFurniture.hpp"

status::CallMonsterInfo CallMonster;//data_020d06c8
status::CharacterStatus* callMonsterStatus_[12];  //data_020d06e8


THUMB int status::getUsuallyAttackEffectValue(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    int attack = actor->haveStatusInfo_.getAttack(0);   
    int defence = target->haveStatusInfo_.getDefence(0);
    int result = 0;
    int damage = (attack - defence / 2) / 2;

    if (damage <= 0) {
        result = dssrand::rand(2);
    } else {
        if (target->characterType_ == PLAYER) {
            int x = attack / 12;
            if (damage <= x) {
                result = dssrand::rand(x);
            } else if (damage < 8) {
                result = damage - 1 + dssrand::rand(3);
            } else {
                result = (damage * 7) / 8 + (dssrand::rand(256) * (damage / 4 + 1)) / 256;
            }
        }
        if (target->characterType_ == MONSTER) {
            if (damage < 8) {
                result = damage - 1 + dssrand::rand(3);
            } else {
                result = (damage * 7) / 8 + ((damage / 4 + 1) * dssrand::rand(256)) / 256;
            }
        }
    }

     if (status::BaseAction::kaishinFlag_ != 0) {
        result = getKaishinAttackEffectValue(actor, target);
    }
    if (status::BaseAction::tsukonFlag_ != 0) {
        result = getTsukon1AttackEffectValue(actor);
    }
    if (status::BaseAction::tsukon2Flag_ != 0) {
        result = getTsukon2AttackEffectValue(actor);
    }

    return result;
}


THUMB int status::getKaishinAttackEffectValue(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    int attack = actor->haveStatusInfo_.getAttack(0);
    target->haveStatusInfo_.getDefence(0);
    return ((dssrand::rand(11) + 64) * attack) / 64;
}


THUMB int status::getTsukon1AttackEffectValue(status::CharacterStatus* actor)
{
    return getRandomVariation(actor->haveStatusInfo_.getAttack(0), 5, 5);
}

THUMB int status::getTsukon2AttackEffectValue(status::CharacterStatus* actor)
{
    return actor->haveStatusInfo_.getAttack(0) * 5;
}

THUMB int status::getRandomVariation(int value, int under, int over)
{
    return (dssrand::rand(under + over + 1) + (100 - under)) * value / 100;
}

THUMB int status::execRula()
{
    if (g_Stage.isRula() == 0) {
        return 0;
    }
    if (g_Stage.isRulaDisable() == 0) {
        return 1;
    }
    return 0;
}

THUMB int status::execRiremito()
{
    if (g_Stage.isRiremito() == 0) {
        return 0;
    }
    if (g_Stage.isRiremitoDisable() == 0) {
        return 1;
    }
    return 0;
}

THUMB bool status::execImpas()
{
    int mapObj;
    int r;
    if (data_0210bb94.unkfunc_02058114(12) == 1) {
        mapObj = TownPlayerManager::getSingleton()->getInpasMapObj();
        r = TownFurnitureManager::getSingleton()->checkCoffer(mapObj);
        if (mapObj == 0 || r == 0) {
            return 0;
        }
    } else {
        return 1;
    }
    return 1;
}


THUMB int status::execLanaruta()
{
    if (!g_Stage.isLanaruta()) {
        return 0;
    }
    if (g_Stage.isLanarutaDisable()) {
        return 0;
    }
    if (g_Stage.timestop_) {
        return 0;
    }
    if (data_0210bb94.unkfunc_02058114(0xc) == 1 && g_Stage.lastFldSurface_ == -1) {
        return 0;
    }
    return 1;
}

 

THUMB void status::initCallMonster()
{
    CallMonster.callMonsterIndex_ = 0;   
    CallMonster.callMonsterCount_ = 0;            
    for (int i = 0; i < 12; i++) {
        callMonsterStatus_[i] = 0;
    }
}


THUMB void status::addCallMonster(status::CharacterStatus* status)
{
    callMonsterStatus_[CallMonster.callMonsterCount_] = status;
    CallMonster.callMonsterCount_ = CallMonster.callMonsterCount_ + 1;
    status->haveStatusInfo_.setCallFriend(1);
    return;
}


THUMB int status::getCallMonsterCount() {
    return CallMonster.callMonsterCount_;
}


THUMB status::CharacterStatus* status::getCallMonsterStatus(int index) {
    return callMonsterStatus_[index];
}


THUMB void status::setCallMonster(int flag) {
    CallMonster.callMonsterFlag_ = flag;
}

THUMB int status::isCallMonster() {
    return CallMonster.callMonsterFlag_;
}

THUMB int status::callDifferentMonster(int index, int monsterIndex)
{
    CallMonster.callMonsterIndex_ = monsterIndex;
    if (btl::BattleMonsterDraw2::getSingleton()->isCallFriend(monsterIndex) != 0
        && g_monster.getCount() < 0xC) {
        if (index != -1) {
            if (g_monster.getMonsterCountInGroup(index) == 8) {
                return 0;
            }
            if (g_monster.getMonsterCountInGroup(index) == 8) {
                return 0;
            }
            addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(index, monsterIndex, 0)));
            setCallMonster(1);
            return 1;
        } else {
            int found = g_monster.getMonsterGroupForMonsterIndex(monsterIndex);
            if (found != -1) {
                if (g_monster.getMonsterCountInGroup(found) == 8) {
                    return 0;
                }
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(found, monsterIndex, 0)));
                setCallMonster(1);
                return 1;
            }
            if (g_monster.getMonsterCountInGroupExist(0) == 0) {
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(0, monsterIndex, 0)));
                setCallMonster(1);
                return 1;
            }
            if (g_monster.getMonsterCountInGroupExist(1) == 0) {
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(1, monsterIndex, 0)));
                setCallMonster(1);
                return 1;
            }
            if (g_monster.getMonsterCountInGroupExist(2) == 0) {
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(2, monsterIndex, 0)));
                setCallMonster(1);
                return 1;
            }
            if (g_monster.getMonsterCountInGroupExist(3) == 0) {
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(3, monsterIndex, 0)));
                setCallMonster(1);
                return 1;
            }
        }
    }
    return 0;
}

THUMB int status::getCallDifferentMonsterIndex() {
    return CallMonster.callMonsterIndex_;
}


THUMB int status::getParupunteMetalSlimeBeforeIndex() {
    return CallMonster.metalSlimeBeforeIndex_;
}


THUMB void status::parupunteMetalSlime1()
{
    CallMonster.metalSlimeBeforeIndex_ = g_monster.getMonsterCallIndex();
    int count = g_monster.getCount();
    int n = 0;
    for (int i = 0; i < count; i++) {
        n++;
    }
    CallMonster.metalSlimeCount_ = n;
}

THUMB void status::parupunteMetalSlime2(int actionIndex)
{
    if (actionIndex == 0x1DB) {
        int collected[12];
        int count = g_monster.getCount();
        int found = 0;
        int i = 0;
        int* p = collected;
        do {
            i = i + 1;
            *p = -1;
            p = p + 1;
        } while (i < 0xC);
        i = 0;
        if (0 < count) {
            int* dst = collected;
            do {
                status::MonsterStatus* monster = (status::MonsterStatus*)g_monster.getMonsterStatus(i);
                if (monster->haveStatusInfo_.isDeath() == 0) {
                    *dst = g_monster.getCtrlId(i);
                    found = found + 1;
                    dst = dst + 1;
                }
                i = i + 1;
            } while (i < count);
        }
        count = 0;
        if (0 < found) {
            int* src = collected;
            int v;
            do {
                v = *src;
                if (v != -1) {
                    ((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::del(v);
                }
                src = src + 1;
                count = count + 1;
            } while (count < found);
        }
        g_monster.getCount();
    }
}

THUMB void status::parupunteMetalSlime3(int actionIndex)
{
    if (actionIndex == 0x1DB) {
        initCallMonster();
        int i = 0;
        if (0 < CallMonster.metalSlimeCount_) {
            do {
                addCallMonster((status::CharacterStatus*)g_monster.getMonsterStatus(((status::MonsterPartyWithDraw*)&g_monster)->MonsterPartyWithDraw::add(0, 0x44, 0)));
                i = i + 1;
            } while (i < CallMonster.metalSlimeCount_);
        }
        g_monster.getCount();
    }
}

THUMB void status::setMosyasIndex(int monsterIndex) {
    CallMonster.mosyasIndex_ = monsterIndex;
}


THUMB int status::getMosyasIndex() {
    return CallMonster.mosyasIndex_;
}