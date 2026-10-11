#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147df0 */
struct TownKaidanAction2 : cmn::ActionBase {
    enum KAIDAN_MOVE_TYPE {
        KAIDAN_MOVE_BACK = 0,
        KAIDAN_MOVE_SIDE = 1,
        KAIDAN_MOVE_FRONT = 2,
        KAIDAN_MOVE_UP = 3,
        KAIDAN_MOVE_NOT_EXIT = 4,
        KAIDAN_MOVE_STOP = 5
    };

    struct TownKaidan {
        int objectId;                           // 0x00
        dss::Fix32Vector3 center;               // 0x04
        dss::Fix32Vector3 normal;               // 0x10
        dss::Fix32Vector3 pos1;                 // 0x1C
        dss::Fix32Vector3 pos2;                 // 0x28
    };

    TownKaidan downKaidan_;                     // 0x04
    TownKaidan upKaidan_;                       // 0x38
    int downChange_;                            // 0x6C
    int exitBeforeUpKaidan_;                    // 0x70
    int kaidanBackWall_;                        // 0x74
    int firstDown_;                             // 0x78
    dss::Fix32 kaidanMaxH_;                     // 0x7C
    dss::Fix32 downKaidanFixY_;                 // 0x80
    KAIDAN_MOVE_TYPE moveType_;                 // 0x84
    int side1Wall_;                             // 0x88
    dss::Fix32Vector3 side1WallLine_;           // 0x8C
    dss::Fix32Vector3 side1WallNormal_;         // 0x98
    int side2Wall_;                             // 0xA4
    dss::Fix32Vector3 side2WallLine_;           // 0xA8
    dss::Fix32Vector3 side2WallNormal_;         // 0xB4
    dss::Fix32Vector3 kaidanArea_[2];           // 0xC0

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownKaidanAction2* getSingleton();
    void checkObject();
    void checkSurface();
    void setKaidanByObject(TownKaidan& kaidan, int id, dss::Fix32Vector3& pos);
    void checkKaidanMoveStart();
    void checkKaidanSide(dss::Fix32Vector3& mVec, dss::Fix32Vector3& target, dss::Fix32& length);
    void setPlayerFixPosition(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos);
    bool setSideFix(dss::Fix32Vector3& pos1, dss::Fix32Vector3& pos2, dss::Fix32Vector3& center, dss::Fix32Vector3& sideLine, dss::Fix32Vector3& sideNormal);
    void setKaidanArea(int id);
    bool isSaveOK();
};
