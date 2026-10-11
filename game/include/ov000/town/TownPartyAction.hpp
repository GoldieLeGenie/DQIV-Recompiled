#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/MoveBase.hpp"

enum SPEED_TYPE {
    SPEED_DEFALUT = 0,
    SPEED_TYPE1 = 1,
    SPEED_TYPE2 = 2
};

struct TownPartyAction {
    dss::Fix32Vector3* m_pos_array;             // 0x000
    short* m_dir_array;                         // 0x004
    short counter_;                             // 0x008
    short m_push_move;                          // 0x00A
    int moveFirstFlag_;                         // 0x00C
    int changeAlpha_;                           // 0x010
    int setFormation_;                          // 0x014
    int half_;                                  // 0x018
    int script_;                                // 0x01C
    int fixFlag_;                               // 0x020
    dss::Fix32Vector3 temp[8];                  // 0x024
    int partyAlphaFlag_[8];                     // 0x084
    int moveType_;                              // 0x0A4
    cmn::MoveBase partyMove_[8];                // 0x0A8
    int distanceCount_;                         // 0x348

    TownPartyAction();
    ~TownPartyAction();
    void setup();
    void cleanup();
    int moveAllPlayerToFirst(int count);
    void normalMove();
    void resetFixPos();
    void setPosition();
    void setAllPotition(dss::Fix32Vector3& pos);
    dss::Fix32Vector3 getMemberPosition(int index);
    void setMemberPosition(int index, dss::Fix32Vector3& pos);
    short getMemberDirIdx(int index);
    void setMemberDirIdx(int index, short dirIdx);
    int getDrawCount();
    void setPositionArrayPointer(dss::Fix32Vector3* pos);
    void setDirIdxArrayPointer(short* dir);
    bool isEqalNextPos(int index);
    void setFormation(dss::Fix32Vector3& dirVec, short dirIdx, dss::Fix32 speed);
    void formationMove();
    bool isFormationEnd();
    void setupPartyDelNotMoveFirst();
    void setMoveToFirstHalfSpeed(SPEED_TYPE flag);
};
