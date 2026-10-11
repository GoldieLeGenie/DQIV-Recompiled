#pragma once
#include <globaldefs.h>
#include "main/cmn/CommonWalkDamage.hpp"

struct FieldPlayerDoku : cmn::CommonWalkDamage {
    int blockAttr_;                     // 0x10

    static FieldPlayerDoku* getSingleton();
    void setup();
    virtual int checkBarrier();
    virtual int checkPoison();
    virtual void setPartyMemberColor(int index, int type);
    void checkDokuDamage(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos);
    void setBlockAttr(int attr);
};
