#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147d08 */
struct TownActionRura : cmn::ActionBase {
    dss::Fix32Vector3 tempPos_;                 // 0x04
    dss::Fix32Vector3 startPos_;                // 0x10

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownActionRura* getSingleton();
};
