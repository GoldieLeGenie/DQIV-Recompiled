#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/status/ExcelParam.hpp"

namespace status{
    struct StatusChangeOne{
        dss::BitFlaguint flag_;
        int damageType_;
        char count_;
        char turn_;
        char pattern_;
        char astron_;
        StatusChangeOne();
        ~StatusChangeOne();
        void setup(int damageType, int flag);
        void cleanup();
        void clear();
        int addTurn();
        int isCancel();
        void execStartOfTurn();
        void execEndOfTurn();
        void execEndOfRound();
        void setEnable(int enable);
        int isEnable();
        void setRelease(bool release);
        bool isRelease();
        int getExecMessage();
        int getResultMessage();
        int getResultPlayerMessage();
        int getResultMonsterMessage();
        int getAgainEnableMessage();
        int getAgainDisableMessage();
        int getReleaseMessage();
        static bool isValidOnAstron(int damageType);
        static bool isValidOnSpazz(int damageType);
        static bool isAfterOnSleep(int damageType);
        static bool isAfterOnPath1(int damageType);
        static bool isAfterOnConfuze(int damageType);
    };
}