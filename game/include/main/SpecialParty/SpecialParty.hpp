#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/Random.hpp"
#include "main/status/ExcelParam.hpp"


struct SpecialParty
{
    unsigned short id_;
    unsigned short monsterIndex_[4];
    unsigned short monsterCount_[4];
    unsigned short countMin_[4];
    unsigned short countMax_[4];
    SpecialParty();
    ~SpecialParty();
    void setup(int index);
    int getMonsterIndex(int grp);
    int getMonsterCount(int grp);
};
