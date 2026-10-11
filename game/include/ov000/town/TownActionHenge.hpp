#pragma once
#include "globaldefs.h"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x0214790c */
struct TownActionHenge : cmn::ActionBase {
    enum {
        NONE = 0,
        CHANGE = 1,
        RESET = 2
    };

    int changeCharaNo;                          // 0x04
    int counter_;                               // 0x08
    int mode_;                                  // 0x0C

    virtual int setup();
    virtual void execute();
    virtual int update();
    static TownActionHenge* getSingleton();
    void setChangeAction();
    void resetParty();
};
