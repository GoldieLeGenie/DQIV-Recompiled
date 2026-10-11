#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147d1c */
struct TownActionWalk : cmn::ActionBase
{
    int collActionFlag_;                        // 0x04
    int searchObjectId_;                        // 0x08
    int searchPolyNo_;                          // 0x0C
    dss::Fix32Vector3 searchObjectPos_;         // 0x10
    int searchObjectSide_;                      // 0x1C
    int ctrSurfaceId_;                          // 0x20
    int idoSurfaceid_;                          // 0x24
    int sekaijyuSurfaceId_;                     // 0x28
    int cureFloor_;                             // 0x2C
    int moveFlag_;                              // 0x30
    int menu_;                                  // 0x34

    virtual int setup();
    virtual void execute();
    virtual int update();
    int getMapUid();
    int checkMapUidObject(int mapUid, int flag);
    void checkCureFloor();
    static TownActionWalk* getSingleton();
    void townCharColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, int flag);
    int unkfunc_02125dcc(int mapUid);
    int searchObject(int flag);
    int getSekaijyuUid();
};
