#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

enum {
    RECT_NONE = 0,
    RECT_UMINARI = 1,
    RECT_YAMI_BARRIER = 2,
    RECT_BALLON = 3
};

struct FieldRectColl {
    dss::Fix32Vector3 pos1;                     // 0x00
    dss::Fix32Vector3 pos2;                     // 0x0C
    int type;                                   // 0x18
};

// rectangle areas of the field map
struct FieldRectCollManager {
    static const int RECT_COLL_NUM = 5;

    FieldRectColl rectColl_[RECT_COLL_NUM];     // 0x00
    int rectCollCount_;                         // 0x8C

    static FieldRectCollManager* getSingleton();
    void setup();
    int checkFieldColl(dss::Fix32Vector3& pos);
    void setRectColl(dss::Fix32Vector3& pos1, dss::Fix32Vector3& pos2, int type);
    bool checkTypeColl(dss::Fix32Vector3& pos, int type);
};
