#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/CharacterStatus.hpp"
#include "main/status/MonsterStatus.hpp"

namespace status {
    struct BaseActionFlag {
        volatile int flag_;
        BaseActionFlag() { flag_ = 0; }
    };
    struct BaseActionStatus {
        enum MonsterDrop {
            GoldExp = 0x1,
            Gold    = 0x2,
        };
        static int baikirutoMessFlag_;
        static int path1MessFlag_;
        static int sleepMessFlag_;
        static int confusionMessFlag_;
        static int mahokantaMessFlag_;
        static int execCallFriend_;
        static int eventBattle_;
        static int work_;
        static BaseActionFlag flag_;
        static int monsterEffectValue_;
        static int playerEffectValue_;
        static int effectValue_;
        static int actionIndex_;
        static void setEffectValue(int effect, int player, int monster);
        static int getEffectValue(status::CharacterStatus *target);
        static int actionTypeDamage(status::CharacterStatus *target);
        static void actionTypeInstantDeath(status::CharacterStatus *target);
        static int actionTypeRecovery(status::CharacterStatus *target);
        static int actionTypeAddMP(status::CharacterStatus *target);
        static int actionTypeSubMP(status::CharacterStatus *actor,status::CharacterStatus *target);
        static void actionTypeBaikiruto(status::CharacterStatus *target);
        static void actionTypeMahokanta(status::CharacterStatus *target);
        static void actionTypeMahosute(status::CharacterStatus *target);
        static void actionTypeMahoton(status::CharacterStatus *target);
        static int actionTypeSleep(status::CharacterStatus *target);
        static void actionTypePoison(status::CharacterStatus *target);
        static int actionTypeSpazz(status::CharacterStatus *target);
        static int actionTypeDefenceChange(status::CharacterStatus *target);
        static int abs(int value);
        static int actionTypeStatusClear(status::CharacterStatus *target);
        static int actionTypeAgilityChange(status::CharacterStatus *target);
        static void actionTypeAstoron(status::CharacterStatus *target);
        static void actionTypeConfusion(status::CharacterStatus *target);
        static void actionTypeDragoram(status::CharacterStatus *target);
        static void actionTypeManusa(status::CharacterStatus *target);
        static void actionTypeFubaha(status::CharacterStatus *target);
        static void actionTypeFeather(status::CharacterStatus *target);
        static int actionTypeLight(status::CharacterStatus *target);
        static void actionTypeDefence(status::CharacterStatus *target);
        static void actionTypePowerSave(status::CharacterStatus *target);
        static int  actionTypeRebirth(status::CharacterStatus *target);
        static int actionTypeMosyas(status::CharacterStatus *actor,status::CharacterStatus *target);
        static int actionTypeNone(status::CharacterStatus *actor, status::CharacterStatus *target);
        static void characterClearOut(status::CharacterStatus *chara, MonsterDrop drop);
        static void setMonsterChange(int flag);
        static bool isMonsterChange();
         
    };
}

namespace btl {
    struct BattleMonster;
    struct BattleMonsterDraw2;
}
struct BattleMonsterDrawParam {
    int unk_00;        // +0
    int unk_04;        // +4
    fx32 vz_;     // +8
    fx32 vy_;     // +0xC
    fx32 vx_;     // +0x10
    int unk_14;        // +0x14
};
extern BattleMonsterDrawParam g_BattleMonsterDrawParam;

