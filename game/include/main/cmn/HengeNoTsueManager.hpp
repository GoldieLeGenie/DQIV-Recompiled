#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "GameInfo.hpp"

namespace cmn
{
    struct HengeNoTsueManager {         
        int index_;
        int charNo_;
        int counter_;
        int change_;
        int locked_;
        int endLess_;
        int nextAction_;
        int getCharNo() { return charNo_; }
        int isChange() { return change_; }
        int getChangeCharaNo();
        void execute();
        int isEnd();
        int getChangeTable(int no);
        bool isMonster();
        void setCounter();
        int getMessage(int charaNo);
        void setNextAction(int action);
        int getNextAction();
    };
}

extern cmn::HengeNoTsueManager g_HengeNoTsue; // data_020efc6c
