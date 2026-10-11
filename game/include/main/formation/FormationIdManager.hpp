#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/formation/FormationId.hpp"
#include "main/dss/Random.hpp"

namespace formation {

struct EncountGroup {
    EncountType type;
    int count;
};

struct FormationIdManager {
    int id_;
    int chapter_;
    int partyCount_;
    TIME_ZONE timeZone_;
    FormationId formationId_;
    EncountGroup group_[4];
    int none_group_[14];
    FormationIdManager();
    ~FormationIdManager();
    void clear();
    void select();
    void selectA_E();
    void selectF_J(EncountType type);
    void selectK();
    void selectL();
    void selectM_N();
    void setFormationId(int id);
    void setChapter(int chp);
    void setPartyCount(int cnt);
    void setTimeZone(TIME_ZONE time);
    void setMonsterNone(EncountType type);
};

}  // namespace formation
