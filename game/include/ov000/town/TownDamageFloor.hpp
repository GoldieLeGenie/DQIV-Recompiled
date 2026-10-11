#pragma once
#include "globaldefs.h"
#include "main/cmn/CommonWalkDamage.hpp"

struct TownDamageFloor : cmn::CommonWalkDamage {
    static TownDamageFloor* getSingleton();
    void setup();
    virtual int checkBarrier();
    virtual int checkPoison();
    virtual void setPartyMemberColor(int index, int type);
    void checkDamageFloor(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos);
};
