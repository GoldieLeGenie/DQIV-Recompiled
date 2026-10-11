#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov003/btl/BattleActor2.hpp"

namespace btl {
    struct BattleTurn
    {
        CharacterType characterType_;
        int actorIndex_;
        btl::BattleActor2 *battleActor_;
        dss::Flag flag_;
        BattleTurn();
        ~BattleTurn();
        void setBattleActor2(btl::BattleActor2* battleActor);
        void execute();
        int isEnd();
        int checkEnd();
        void setup();
        void cleanup();
        void exec();
        int isBattleActor2End();
        void reattack();
        void change();
        void tarot();
        void parupunte();
    };
    
}