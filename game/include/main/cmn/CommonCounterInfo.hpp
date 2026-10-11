#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/UnkMemory.hpp"

namespace cmn{
    struct CommonCounterInfo{
        int waitCounter_;
        int waitMax_;
        unsigned char dayCounter_[16];
        unsigned char freeCounter_[4];
        void initialize();
        bool isEndWaitCounter();
        void setWaitZero(int frame);
        int checkBottun();
        int setDayCounter(int index, unsigned char value);
        void setChangeDay();
        int isEndDayCounter(int index);
    };
    extern CommonCounterInfo g_CommonCounterInfo;
}


 
