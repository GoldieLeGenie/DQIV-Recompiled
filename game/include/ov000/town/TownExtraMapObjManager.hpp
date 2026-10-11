#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

struct TownFloorMapObjData {
    int mapUid;                                 // 0x00
    dss::Fix32Vector3 position;                 // 0x04
};

struct TownExtraMapObjManager {
    static const int FLOOR_MAP_OBJ_COUNT = 20;

    int floorMapObjCount_;                      // 0x000
    TownFloorMapObjData floorMapObj_[FLOOR_MAP_OBJ_COUNT]; // 0x004

    static TownExtraMapObjManager* getSingleton();
    void setup();
    void setData(int mapUid, dss::Fix32Vector3 pos);
    int checkFloorMapUid(dss::Fix32Vector3& pos);
    bool getPosition(int mapUid, dss::Fix32Vector3& position);
};
