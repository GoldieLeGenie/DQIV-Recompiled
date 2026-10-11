#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/ExcelParam.hpp"

namespace param {
    extern const ExcelFile<unsigned char, 4020> EffectParam_array;
}

namespace status {
    struct ExcelParamBis {
    
        static void setupBattle(status::ExcelParam *setup);
        static void cleanupBattle(status::ExcelParam *clean);
        static void setupBattleInitialize(status::ExcelParam *clean);
        static void cleanupBattleInitialize(status::ExcelParam *clean);
    };
}
    
