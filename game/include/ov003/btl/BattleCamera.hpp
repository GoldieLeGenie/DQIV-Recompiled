#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Camera.hpp"
#include "main/CameraControl/CameraControl.hpp"
#include "ov003/btl/BattleCameraHoming.hpp"

struct BattleCamera {
    UnkCamera unk_000;
    UnkCamera normalCamera_;            // 0x064
    dss::Fix32Vector3 initposition_;    // 0x0C8
    int enable_;                        // 0x0D4
    int camera1;                        // 0x0D8
    int camera2;                        // 0x0DC
    char file_[16];                     // 0x0E0
    char file2_[16];                    // 0x0F0
    BattleCameraHoming homing_;         // 0x100

    static int cameraSetting;

    BattleCamera();
    ~BattleCamera();
    static BattleCamera* getSingleton();
    void initialize();
    void terminate();
    void reset();
    void executeForMap();
    void draw();
    void initCamera();
    void setCameraAnimation(unsigned char camera1, unsigned char camera2, unsigned short wait);
    void setHomingTarget(int drawCtrlId);
    void setWait(int wait);
    dss::Camera* getCamera();
    void setFilename(const char* file, const char* file2);
    bool isCameraAnimation();
    void setCameraSeting(bool flag);

    // inline helper: the argument is evaluated before getSingleton() (matches BattleEffectUnit::setTarget)
    void setHomingWaitTime(int time) { homing_.setWaitTime(time); }
};
