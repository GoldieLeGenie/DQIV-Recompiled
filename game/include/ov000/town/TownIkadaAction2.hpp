#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"

/* vtable 0x02147d8c */
struct TownIkadaAction2 : cmn::ActionBase {
    enum IKADA_MOVE_TYPE {
        IKADA_MOVE = 0,
        GET_ON_IKADA = 1,
        GET_OFF_IKADA = 2
    };

    int ikadaObjectId_;                         // 0x04
    int ctrSurfaceId_;                          // 0x08
    int ctrSurfacePoly_;                        // 0x0C
    int counter_;                               // 0x10
    dss::Fix32Vector3 ikadaPosition_;           // 0x14
    int moveMode_;                              // 0x20

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownIkadaAction2* getSingleton();
    void ikadaMove();
    void setIkadaDataByScript(const char* name, dss::Fix32Vector3& pos);
    void setIkadaPosition(dss::Fix32Vector3& pos);
    bool checkIkadaTalk(bool flag);
};
