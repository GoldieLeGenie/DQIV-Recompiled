#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"

struct BattleCameraHoming {
    int enable_;
    int step_;
    unsigned char startTime_;
    unsigned char waitTime_;
    unsigned char restoreTime_;
    unsigned char count_;
    short rotAngle_;

    BattleCameraHoming();
    ~BattleCameraHoming();
    void setup(dss::Fix32Vector3 target, int drawCtrlId);
    void calcHomingTarget(dss::Vector3short& angle);
    void waitHomingTarget(dss::Vector3short& angle);
    void restoreHomingTarget(dss::Vector3short& angle);
    void calculation(dss::Vector3short& angle);
    void setRotateTime(int time);
    void setWaitTime(int time);
};
