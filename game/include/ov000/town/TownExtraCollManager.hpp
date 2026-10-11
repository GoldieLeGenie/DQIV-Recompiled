#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

struct TownExtraCollLinkData {
    int type;                                               // 0x00
    int id;                                                 // 0x04
    int objectId;                                           // 0x08
    int flag;                                               // 0x0C
};

struct TownExtraCollManager {
    static const int EXTRA_COLL_COUNT_MAX = 32;

    TownExtraCollLinkData extraCollData_[EXTRA_COLL_COUNT_MAX]; // 0x000
    int extraCollCount_;                                    // 0x200

    static dss::Fix32 sleepCharaW;
    static dss::Fix32 sleepCharaH;
    static dss::Fix32 sleepCharaY;

    static TownExtraCollManager* getSingleton();
    void setup();
    void resetCharaColl(int charaNo, int type);
    void addCharacterColl(int ctrl, int type);
    int isExtraCollChara(int objectNo, int& charaNo);
    void addSleepChara(int charaNo);
    void addMoveColl(int charNo, int type, dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos);
};
