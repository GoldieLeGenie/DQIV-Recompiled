#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/BattleActorExec.hpp"

namespace btl {
    struct BattleActor2 {
        status::CharacterStatus *characterStatus_;
        status::HaveBattleStatus *haveBattleStatus_;
        status::UseActionParam useActionParam_;
        BattleActorExec battleActorExec_;
        int agility_;
        BattleActor2();
        ~BattleActor2();
        void setup(status::CharacterStatus* chara);
        void setup();
        void reattack();
        void change();
        void tarot();
        void parupunte();
        void meganteRing();
        void megazaruRing();
        void execute();
        int isEnd();
        void setAction();
        void setTarget();
        int isActionEnable();
    };
    
}
