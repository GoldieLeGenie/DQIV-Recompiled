#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/btl/BattleEffectTransform.hpp"

namespace btl {
    struct BattleTransform {
        int enable_;                        // 0x000
        int max_;                           // 0x004
        int transIndex_;                    // 0x008
        BattleEffectTransform trans_[3];    // 0x00C

        BattleTransform();
        ~BattleTransform();
        static BattleTransform* getSingleton();
        void setup(int index);
        void setTransform();
        void draw();
        void cleanup();
        int isEnd();
        int startNext();
        static int getDummyFromMonster(int index);
        static int getDummyFromTrans();
    };
}
