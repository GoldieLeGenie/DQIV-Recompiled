#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147e58 */
struct TownShipAction2 : cmn::ActionBase {
    enum SHIP_MOVE_MODE {
        SHIP_MOVE = 0,
        GET_ON_SHIP = 1,
        SHIP_MOVE_TO = 2,
        SHIP_DOCKED_AT = 3,
        GET_OFF_SHIP = 4
    };

    int shipObjectId_;                          // 0x04
    int shipNamiObjectId_;                      // 0x08
    int namiAlpha_;                             // 0x0C
    int olgNamiAlpha_;                          // 0x10
    dss::Fix32Vector3 shipPosition_;            // 0x14
    dss::Fix32Vector3 prevShipPosition_;        // 0x20
    dss::Fix32Vector3 shipNamiPosition_;        // 0x2C
    short prevShipDirection_;                   // 0x38
    short shipDirection_;                       // 0x3A
    SHIP_MOVE_MODE moveMode_;                   // 0x3C
    dss::Fix32Vector3 shipVec_;                 // 0x40
    dss::Fix32Vector3 dockedatPos_;             // 0x4C
    dss::Fix32 dockedLen_;                      // 0x58
    short dockedDirIdx_;                        // 0x5C
    dss::Fix32Vector3 targetPos_;               // 0x60
    int ctrSurfacePoly_;                        // 0x6C
    int ctrSurfaceId_;                          // 0x70

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownShipAction2* getSingleton();
    void setDirection(short playerDir);
    void setShipNamiAlpha();
    void setShipPosition(dss::Fix32Vector3& pos);
    void shipMove();
};
