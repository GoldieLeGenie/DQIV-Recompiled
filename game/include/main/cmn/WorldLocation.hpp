#pragma once
#include "globaldefs.h"
#include "main/param/Param.hpp"
#include "main/dss/DssUtils.hpp"
#include "GameInfo.hpp"


namespace cmn
{
    struct WorldLocation
    {
        param::CLUTCode *pCLUTCode_;
        WorldLocation();
        ~WorldLocation();
        static WorldLocation* getSingleton();
        void initialize();
        void terminate();
        static void calcWorldPos(fx32* x, fx32* y);
        dss::Fix32Vector3 calcPaletteRate(int prev, int next, dss::Fix32 ratio);
        dss::Fix32Vector3 calcPaletteRate(int time);
        dss::Fix32Vector3 calcYamiPaletteRate();
        static void setCurrentTimeZone();
        static TIME_ZONE getCurrentTimeZone();
    };
    
}