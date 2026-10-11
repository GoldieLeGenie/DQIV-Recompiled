#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147e34 */
struct TownRopeAction2 : cmn::ActionBase {
    enum ROPE_MOVE_TYPE {
        ROPE_MOVE = 0,
        DOWN_SIDE_GET_ON = 1,
        DOWN_SIDE_GET_OFF = 2,
        UP_SIDE_GET_ON = 3,
        UP_SIDE_GET_OFF = 4
    };

    dss::Fix32 minY_;                           // 0x04
    dss::Fix32 maxY_;                           // 0x08
    dss::Fix32Vector3 surfaceDir_;              // 0x0C
    ROPE_MOVE_TYPE moveMode_;                   // 0x18

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownRopeAction2* getSingleton();
    void getRopeSide(int surfaceId);
    void ropeMove();
    void ropeMoveUpdate();
};
