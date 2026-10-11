#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x0214794c */
struct TownActionRuraFailed : cmn::ActionBase {
    enum {
        FAILED_RURA_UP1 = 0,
        FAILED_RURA_UP2 = 1,
        FAILED_RURA_TOP = 2,
        FAILED_RURA_DOWN1 = 3,
        FAILED_RURA_DOWN2 = 4
    };

    int mode_;                                  // 0x04
    int counter_;                               // 0x08
    int prevAction_;                            // 0x0C
    dss::Fix32Vector3 startPos_;                // 0x10
    dss::Fix32Vector3 tempPos_;                 // 0x1C
    short prev_dirIdx_;                         // 0x28

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownActionRuraFailed* getSingleton();
};
