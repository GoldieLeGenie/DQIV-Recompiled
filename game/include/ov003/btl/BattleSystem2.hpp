#pragma once
#include <globaldefs.h>
#include "main/dss/Render.hpp"

namespace btl {
    struct BattleSystem2 {
        Render render_;                 // 0x000

        BattleSystem2();
        ~BattleSystem2();
        static BattleSystem2* getSingleton();
        void initialize();
        void terminate();
        void execute();
        void draw();
    };
}


