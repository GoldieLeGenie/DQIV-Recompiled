#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/btl/BattleTurn.hpp"
#include "ov003/btl/BattleActor2.hpp"

namespace btl {
    struct BattleRound {
        BattleTurn battleTurn_[16];
        int currentBattleTurn_;         // 0x100
        int currentBattleTurnID_;       // 0x104
        int countBattleTurn_;           // 0x108
        int turnEndFlag_;               // 0x10C

        BattleRound();
        ~BattleRound();
        void initialize();
        void terminate();
        void execute();
        int isEnd();
        BattleActor2* add(status::CharacterStatus* chara);
        int isMegazaruRingEnable();
        int execMegazaruRing();
        int execMeganteRing();
        BattleTurn* getCurrentTurn() { return &battleTurn_[currentBattleTurn_]; }
    };
}
