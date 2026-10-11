#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"

namespace btl {
    struct SetFlagInfo {
        short flagNo;
        char flagType;
        char victory;
    };

    struct SetFlagParam {
        int flagType;
        int flagNo;
        int value;
    };

    struct BattleScriptManager {
        int checkBattle_;
        int setDataCount_;
        SetFlagInfo flagInfo_[2];
        BattleScriptManager();
        void setScriptBattleResult(int flagType, int flagNo, int victory);
        void checkScriptBattleResult();
        static BattleScriptManager* getSingleton();
        void setEncountMap(int tile);
    };
}
