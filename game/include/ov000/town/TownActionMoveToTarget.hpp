#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147eac */
struct TownActionMoveToTarget : cmn::ActionBase {
    enum MOVE_TYPE {
        MOVE_TO_TARGET = 0,
        MOVE_TO_FIRST = 1,
        MOVE_END = 2
    };

    dss::Fix32Vector3 target_;                  // 0x04
    dss::Fix32Vector3 nowPos_;                  // 0x10
    dss::Fix32Vector3 moveVec_;                 // 0x1C
    dss::Fix32 speed_;                          // 0x28
    int moveType_;                              // 0x2C
    int drawType_;                              // 0x30
    int nextAction_;                            // 0x34
    int partyMoveFlag_;                         // 0x38
    MOVE_TYPE moveMode_;                        // 0x3C
    int eraseFlag_;                             // 0x40
    int count_;                                 // 0x44

    virtual int setup();
    virtual void execute();
    virtual int update();
    void setAction(dss::Fix32Vector3& startPos, dss::Fix32Vector3& target, dss::Fix32 speed, int moveFlag, int drawFlag, int nextAction);
    void drawAllExec();
    void drawEraseExec(int start);
};

extern TownActionMoveToTarget gMoveToTarget;
