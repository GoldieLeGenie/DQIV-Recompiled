#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/BattleSelectTargetParam.hpp"

namespace btl {
    struct BattleSelectTarget
    {
        static status::CharacterStatus * specialTarget_[15]; //data_ov015_0217ad84
        static void setTarget(status::UseActionParam* param, BattleSelectTargetParam::CallTargetSelect select);
        static int setTargetSet(status::UseActionParam* actionParam,btl::BattleSelectTargetParam::CallTargetSelect callTarget);
        static void printTarget(status::UseActionParam* param);
        static void setTargetPlayer(btl::BattleSelectTargetParam* param);
        static void setTargetPlayerWithDeath(btl::BattleSelectTargetParam* param);
        static void setTargetPlayerAll(btl::BattleSelectTargetParam* param);
        static void setTargetPlayerAllWithDeath(btl::BattleSelectTargetParam* param);
        static void setTargetMonster(btl::BattleSelectTargetParam* param);
        static void setTargetMonsterWithDeath(btl::BattleSelectTargetParam* param);
        static int setTargetBoth(int actionIndex, btl::BattleSelectTargetParam* param);
        static void setTargetStadiumEnemy(BattleSelectTargetParam* param);
        static int setTargetStadiumMine(BattleSelectTargetParam* param);
        static int setTargetNone(status::CharacterStatus* chara, BattleSelectTargetParam* param);
        static int setTargetMyself(status::CharacterStatus* chara, BattleSelectTargetParam* param);
        static int setTargetFriend(status::CharacterStatus* chara, int action, BattleSelectTargetParam* param);
        static int setTargetEnemy(status::CharacterStatus* chara, int action, BattleSelectTargetParam* param);
        static int setTargetOne(btl::BattleSelectTargetParam* param);
        static int setTargetGroup(btl::BattleSelectTargetParam* param);
        static int setTargetAll(btl::BattleSelectTargetParam* param);
        static int setTargetAllWithCarriage(btl::BattleSelectTargetParam* param);
        static void setTargetSpecial(status::UseActionParam* param);
        static void setTargetSpecialToPlayer(status::UseActionParam* param);
        static void setTargetSpecialToMonster(status::UseActionParam* param);
        static int setTargetSpecialToMonsterNoSpazz2(int targetCount);
        static int setTargetSpecialToMonsterNoConfusion2(int targetCount);
        static int setTargetSpecialToMonsterNearDeath2(int targetCount);
        static int setTargetSpecialToMonsterHpMin2(int targetCount);
        static void setTargetSpecialToParam2(status::UseActionParam* param);
        static void setTargetCrossFire(status::UseActionParam* param);
        static void setActorAction(status::UseActionParam* param,BattleSelectTargetParam::CallTargetSelect select);
    };
    
}

;
