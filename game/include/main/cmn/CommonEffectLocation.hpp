#pragma once
#include "globaldefs.h"
#include "main/global/GlobalDQ4.hpp"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/fld/FLDObject.hpp"
#include "ov003/btl/BattleStage.hpp"

namespace cmn{
    struct CommonEffectLocation
    {
        dss::Fix32Vector3 prev_;
        dss::Fix32Vector3 next_;
        int frame_;
        int counter_;
        int extend_;
        int enable_;
        int index_;
        CommonEffectLocation();
        ~CommonEffectLocation();
        static CommonEffectLocation* getSingleton();
        void initialize();
        void terminate();
        void execute();
        void start(int index, int extend);
        int setPaletteRate(int index);
        int calcPaletteRate();
        dss::Fix32Vector3 getPaletteRate();
    };
}

struct TownStageManager;
