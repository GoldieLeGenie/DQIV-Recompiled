#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147e94 */
struct TownSubeAction : cmn::ActionBase {
    int subeDir4_;                              // 0x04
    int prev_subeDir4_;                         // 0x08
    int count_;                                 // 0x0C
    dss::Fix32Vector3 vec[4];                   // 0x10

    static const dss::Fix32 subeR;
    static const dss::Fix32 subeSpeed;

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownSubeAction* getSingleton();
};
