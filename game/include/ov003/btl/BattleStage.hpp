#pragma once
#include "main/menu/MenuAPI.hpp"
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/fld/FldStage.hpp"

struct BattleStage {
    FldStage stage_;                            // 0x000
    dss::Fix32Vector3 prev_;                    // 0x8B4
    dss::Fix32Vector3 next_;                    // 0x8C0
    int frame_;                                 // 0x8CC
    int counter_;                               // 0x8D0
    char mapName_[32];                          // 0x8D4

    BattleStage();
    ~BattleStage();
    static BattleStage* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void execFade();
    int isFade();
    void setFldRGBRate(dss::Fix32Vector3& rgb) {
        VecFx32 v;
        v.x = rgb.vx.value;
        v.y = rgb.vy.value;
        v.z = rgb.vz.value;
        stage_.m_fld.SetRGBRate(&v, 0);
    }
    void setRGBRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b);
};

extern "C" {
}
