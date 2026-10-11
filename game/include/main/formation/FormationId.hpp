#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/dss/Random.hpp"

namespace formation {

struct FormationId {
    int formationId_;
    int chapter_;
    int partyCount_;
    unsigned char thirdGroup_;
    unsigned char forthGroup_;
    short typeRate_[14];
    unsigned char countMin_[14];
    unsigned char countMax_[14];
    FormationId();
    ~FormationId();
    void setup(int id);
    EncountType select();
    static int isRange(int value, int min, int max);
    static short getMaxValue(short* rates, int count);
    EncountType selectA_E();
    int getMonsterCount(EncountType type);
    int getMonsterCountA_E();
    static unsigned int getEncountFormNumCount();
    int getMonsterCountF_J(EncountType type);
    int getMonsterCountK_L();
    int getMonsterCountM_N();
    int setMonsterCountLimit(int count);
    int isThirdGroup();
    int isForthGroup();
    void setTimeZone(int time);
    void setDaytime();
    void setNight();
    void setClear(EncountType type);
    void setChapter(int chp);
    void setPartyCount(int cnt);
};

}  // namespace formation

struct EncountFormNumHeader {
    unsigned int count;                     // 0x00
    unsigned int unk_4;                      // 0x04
    unsigned int unk_8;                      // 0x08
};
extern const EncountFormNumHeader encountFormNumHeader; //data_0208ca04
