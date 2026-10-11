#pragma once
#include <globaldefs.h>
#include "main/fld/FldStage.hpp"

struct CasinoStage {
    FldStage stage_;                            // 0x000
    dss::Fix32Vector3 prev_;                    // 0x8B4
    dss::Fix32Vector3 next_;                    // 0x8C0
    int frame_;                                 // 0x8CC
    int counter_;                               // 0x8D0

    CasinoStage();
    ~CasinoStage();
    static CasinoStage* getSingleton();
    void initialize();
    void terminate();
    void setRotObjectUid(int uid, dss::Fix32Vector3& rot) { stage_.setRotObjectUid(uid, rot); }
    void setObjectDraw(int id, int draw, int uidFlag);
};
