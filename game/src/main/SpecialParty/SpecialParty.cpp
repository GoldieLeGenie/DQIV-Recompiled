#include "main/SpecialParty/SpecialParty.hpp"



THUMB SpecialParty::SpecialParty()

{
  return;
}


THUMB SpecialParty::~SpecialParty()

{
  return;
}


THUMB void SpecialParty::setup(int index)
{
    param::EncountSpecial *tbl = status::excelParam.encountSpecial_;
    int min;
    int max;

    min = tbl[index].group1_min;
    max = tbl[index].group1_max;
    monsterIndex_[0] = tbl[index].monsterID1;
    monsterCount_[0] = min + dssrand::rand(max - min + 1);

    min = tbl[index].group2_min;
    max = tbl[index].group2_max;
    monsterIndex_[1] = tbl[index].monsterID2;
    monsterCount_[1] = min + dssrand::rand(max - min + 1);

    min = tbl[index].group3_min;
    max = tbl[index].group3_max;
    monsterIndex_[2] = tbl[index].monsterID3;
    monsterCount_[2] = min + dssrand::rand(max - min + 1);

    min = tbl[index].group4_min;
    max = tbl[index].group4_max;
    monsterIndex_[3] = tbl[index].monsterID4;
    monsterCount_[3] = min + dssrand::rand(max - min + 1);
}

THUMB int SpecialParty::getMonsterIndex(int grp)
{
  return monsterIndex_[grp];
}

THUMB int SpecialParty::getMonsterCount(int grp)
{
  return monsterCount_[grp];
}

