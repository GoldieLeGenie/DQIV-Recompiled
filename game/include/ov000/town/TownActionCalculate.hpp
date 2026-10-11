#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

struct TownActionCalculate {
    static const dss::Fix32 cos_PI_6;

    static bool IntersectRaySphere(dss::Fix32Vector3& p, dss::Fix32Vector3& d, dss::Fix32Vector3& sc, dss::Fix32 r, dss::Fix32& t, dss::Fix32Vector3& q);
    static bool crossCheck(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32Vector3& target, dss::Fix32 radius);
    static void townCharaColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 r, int surfaceId, int objectId, int polyNo, dss::Fix32 ctrLen, int menuFlag);
    static int townStageColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 srufaceRad, dss::Fix32 preR);
    static void townShipStageColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 surfaceRad, dss::Fix32 preR);
    static bool directionCheckByPosition(dss::Fix32Vector3& pos, dss::Fix32Vector3& target, short idx, int value);
    static void getDirByIdx(short dirIdx, dss::Fix32Vector3& dir);
    static void getIdxByVec(short& idx, dss::Fix32Vector3& dir);
    static int searchPairWdoor(int objectId, dss::Fix32Vector3* door1, dss::Fix32Vector3* door2);
    static bool checkTalking(dss::Fix32Vector3& pos, short dirIdx, int objectId);
    static bool checkIkadaTalk(dss::Fix32Vector3 pos, short dirIdx, int surfaceId, int polyId, int menuSearch);
    static void normalMove(dss::Fix32Vector3& position, short& dirIdx, dss::Fix32 speed);
    static bool checkGetOnShipAndIkada(const dss::Fix32Vector3& nextPos, const dss::Fix32Vector3& pos, short dirIdx, dss::Fix32 length);
    static bool checkGetDownShipAndIkada(dss::Fix32Vector3& nextPos, short dirIdx, dss::Fix32Vector3& targetPos, dss::Fix32Vector3& surfaceDir, dss::Fix32Vector3& surfacePos, dss::Fix32 downL);
    static int getParamDir4ByIdx(short idx);
    static dss::Fix32Vector3 getParamVec(unsigned char dir);
    static short getIdxByParam(unsigned char dir);
    static bool checkLineOver(dss::Fix32Vector3& pos, dss::Fix32Vector3& linePos, dss::Fix32Vector3 normal);
    static void setAngle(char axisNo, short idx, dss::Vector3<short>& angle);
    static bool checkGetDownIkada(dss::Fix32Vector3& nextPos, short dirIdx, dss::Fix32Vector3& targetPos);
};
