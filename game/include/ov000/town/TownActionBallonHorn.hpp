#pragma once
#include "globaldefs.h"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147868 */
struct TownActionBallonHorn : cmn::ActionBase {
    int ballonEnable_;                          // 0x04
    short counter_;                             // 0x08
    char type_;                                 // 0x0A
    int prevAction_;                            // 0x0C

    virtual int setup();
    virtual void execute();
    virtual int update();
    static TownActionBallonHorn* getSingleton();
    void startAction(int action);
};
