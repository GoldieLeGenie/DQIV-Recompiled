#pragma once

struct TownFurnitureManager;
#include <globaldefs.h>
#include "main/global/GlobalDQ4.hpp"
#include "GameInfo.hpp"
#include "main/status/CharacterStatus.hpp"
#include "ov003/status/MonsterParty.hpp"

namespace status {
    struct CallMonsterInfo {
        char pad_0x00[0x08];              // 0x00-0x07 
        int metalSlimeBeforeIndex_;       // 0x08
        int metalSlimeCount_;             // 0x0c
        int callMonsterFlag_;             // 0x10 — isCallMonster, setCallMonster
        int mosyasIndex_;                 // 0x14
        int callMonsterCount_;            // 0x18 — addCallMonster, initCallMonster
        int callMonsterIndex_;   // 0x1c — callDifferentMonster, getCallDifferentMonsterIndex
    };
    int getUsuallyAttackEffectValue(status::CharacterStatus* actor, status::CharacterStatus* target);
    int getKaishinAttackEffectValue(status::CharacterStatus* actor, status::CharacterStatus* target);
    int getTsukon1AttackEffectValue(status::CharacterStatus* actor);
    int getTsukon2AttackEffectValue(status::CharacterStatus* actor);
    int getRandomVariation(int value, int under, int over);
    int execRula();
    int execRiremito();
    bool execImpas();
    int execLanaruta();
    void initCallMonster();
    void addCallMonster(CharacterStatus* status);
    int getCallMonsterCount();
    status::CharacterStatus* getCallMonsterStatus(int index);
    void parupunteMetalSlime3(int actionIndex);
    void setMosyasIndex(int monsterIndex);
    int getMosyasIndex();
    void setCallMonster(int flag);
    int isCallMonster();
    int callDifferentMonster(int index, int monsterIndex);
    int getCallDifferentMonsterIndex();
    int getParupunteMetalSlimeBeforeIndex();
    void parupunteMetalSlime1();
    void parupunteMetalSlime2(int actionIndex);

}

extern status::CallMonsterInfo CallMonster; //data_020d06c8
extern status::CharacterStatus* callMonsterStatus_[12];  //data_020d06e8

struct TownPlayerManager;
namespace btl { struct BattleMonsterDraw2; }
