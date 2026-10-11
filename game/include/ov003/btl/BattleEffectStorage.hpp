#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/btl/BattleEffectGroup.hpp"

namespace btl {
    struct BattleEffectStorage {
        BattleEffectGroup group[12];
        int effectCounter_;             // 0xB5B0

        BattleEffectStorage();
        ~BattleEffectStorage();
        void initialize();
        void terminate();
        BattleEffectGroup* getContainer();
        void restoreContainer();
        int getContainerStock();
    };
}
