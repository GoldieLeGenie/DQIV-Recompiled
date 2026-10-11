#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Camera.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/object/BillboardCharacter.hpp"


struct TownCamera {
    dss::DualCamera camera_;                         // 0x000
    dss::Fix32 limitL;                               // 0x0F4
    dss::Fix32 limitR;                               // 0x0F8
    int flagRotateL;                                // 0x0FC
    int flagRotateR;                                // 0x100
    int cameraLock_;                                // 0x104
    int pointLock_;                                 // 0x108
    int povLock_;                                   // 0x10C
    int isPovMove_;                                 // 0x110
    int effect_;                                    // 0x114
    int remote_;                                    // 0x118
    dss::Fix32Vector3 savePos_;                      // 0x11C
    dss::Vector3<short> saveAngle_;                 // 0x128
    int saveFlag_;                                  // 0x130
    cmn::MoveBase cameraMove_;                      // 0x134
    cmn::MoveBase povMove_;                         // 0x188
    cmn::MoveBase effecter_;                        // 0x1DC
    int changeDistance_;                            // 0x230
    dss::Fix32 distance_;                            // 0x234
    dss::Fix32 addDistance_;                         // 0x238
    dss::Fix32 endDistance_;                         // 0x23C
    dss::Fix32Vector3 povOffset_;                    // 0x240
    int counter_;                                   // 0x24C
    int frame_;                                     // 0x250
    int targetChara_;                               // 0x254
    int changeAngle_;                               // 0x258
    int changeDefaultAngleFlag_;                    // 0x25C
    dss::Vector3<short> changeDefaultAngle_;        // 0x260
    dss::Vector3<short> preAngle_;                  // 0x266
    int notEqualPreAngle_;                          // 0x26C


    TownCamera();
    ~TownCamera();
    static TownCamera* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    bool setAngleNorth(short& retAngle);
    void rotateL();
    void rotateR();
    void setLimitL(dss::Fix32 left);
    void setLimitR(dss::Fix32 right);
    void resetAngle();
    void restore();
    void store();
    void setMoveTo(dss::Fix32Vector3& target, int frame, bool absFlag);
    void setRotTo(dss::Vector3<short>& angle, int frame, bool absFlag);
    void resetCameraMove(int frame);
    void setShake(int type, int count);
    void setChangeDistance(int frame, dss::Fix32 distance);
    void resetDistance(int frame);
    bool isEndChangeDistance();
    void setPovMove(dss::Fix32Vector3 target, int frame, int flag);
    void calculatePursue(dss::Vector3short& angle, dss::Fix32Vector3& pos, dss::Fix32Vector3& target);
    void setTargetPlayer(int flag);
    void setMoveTargetPlayer(int frame);
    void setMoveTragetChara(int index);
    void setCameraLock(bool flag);
    void setLockPov(int flag);
    void setDefaultAngle(dss::Vector3<short>& angle);

};
