#pragma once
#include "main/dss/DssUtils.hpp"

namespace cmn {
    struct MoveBase {
        dss::Fix32Vector3 targetPos_;            // 0x00
        dss::Fix32Vector3 startPos_;             // 0x0C
        dss::Fix32Vector3 moveVec_;              // 0x18
        dss::Vector3<short> targetDirIdx_;      // 0x24
        dss::Vector3<short> startDirIdx_;       // 0x2A
        dss::Vector3<short> rotIdx_;            // 0x30
        short endMoveFrame_;                    // 0x36
        short endRotFrame_;                     // 0x38
        short moveCounter_;                     // 0x3A
        short rotCounter_;                      // 0x3C
        short dampFrame_;                       // 0x3E
        short ampFrame_;                        // 0x40
        int rotFlag_;                           // 0x44
        int moveFlag_;                          // 0x48
        int moveLock_;                          // 0x4C
        int moveType_;                          // 0x50

        static const dss::Fix32 grav;

        MoveBase() : targetDirIdx_(), startDirIdx_(), rotIdx_() {}

        void setup();
        void execMove(dss::Fix32Vector3& pos);
        void execRot(dss::Vector3<short>& rot);
        int moveUpdate();
        int rotUpdate();
        bool isEnd();
        void setActionMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target);
        void setActionRot(dss::Vector3<short>& start, const dss::Vector3<short>& target);
        void setMoveSpeed(dss::Fix32 speed);
        void setMoveFrame(int frame);
        void setRotFrame(int frame, int type);
        short setRot(short rot, int frame, int type);
        void simpleMove(dss::Fix32Vector3& pos);
        void simpleRot(dss::Vector3<short>& rot);
        void setSimpleRot(dss::Vector3<short>& start, dss::Vector3<short>& add, int frame);
        void setRotSpeedY(short speed);
        int simpleMoveUpdate();
        int simpleRotUpdata();
        void setVibMotion(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int amp, int damp, int frame);
        void moveVibMotion(dss::Fix32Vector3& pos);
        int updateVibMotion();
        void setRandomShake(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int count);
        void shakeExecute(dss::Fix32Vector3& pos);
        int updateShake();
        dss::Fix32Vector3 getShakeVec(int index);
        void setJumpMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int frame);
        void jumpExecute(dss::Fix32Vector3& pos);
        int updateJump();
        void setAddMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int frame);
        void moveAddExecute(dss::Fix32Vector3& pos);
        int updateMoveAdd();
        void dirMoveExec(dss::Fix32Vector3& pos);
        int updateDirMove();
        void setDirMove(dss::Fix32 value, int dir, dss::Fix32 speed);
    };
}

int unkfunc_02031e84(int value);   // abs
