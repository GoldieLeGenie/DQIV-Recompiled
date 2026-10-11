#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

struct FieldPlayerInfo {
    dss::Fix32Vector3 nowPos;           // 0x00
    dss::Fix32Vector3 nextPos;          // 0x0C
    short dirIdx;                       // 0x18
};

struct FieldCollInfo {
    dss::Fix32 collLine[4];             // 0x00
    dss::Fix32 fixLine[4];              // 0x10
    int blockColl[9];                   // 0x20
};

struct FieldActionCalculate {
    static int playerFixMove(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd);
    static short getDir8ByVector3(dss::Fix32Vector3& vec);
    static dss::Fix32Vector3 getVector3ByDir8(int dir);
    static int getDir8RotIdx(int idx, int rot);
    static void getVecByScriptParam4(dss::Fix32Vector3& vec, int param);
    static int getIdxByParam4(int param);
    static int playerFixMoveUp(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag);
    static int playerFixMoveRightUp(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd);
    static int playerFixMoveRight(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag);
    static int playerFixMoveRightDown(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd);
    static int playerFixMoveDown(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag);
    static int playerFixMoveLeftDown(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd);
    static int playerFixMoveLeft(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag);
    static int playerFixMoveLeftUp(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd);
    static int checkDiagonalLine(dss::Fix32Vector3& pos, int blkX, int blkY, int type);
    static void frontHitFix(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd);
    static void frontBlankFix(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd);
};
