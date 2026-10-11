#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov003/btl/BattleMonster.hpp"


namespace status{
    struct MonsterPartyWithDraw : MonsterParty
    {
        virtual int add(int monsterGroup, int monsterIndex, int flag); 
        virtual void del(int ctrl);
    };
    
}

extern status::MonsterPartyWithDraw g_monster;

extern dss::Vector3int g_monsterDrawPos;
