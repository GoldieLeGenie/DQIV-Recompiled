#pragma once
#include "main/dss/DssUtils.hpp"
#include "nitro/g3.hpp"
#include "nnsys/g3d.hpp"

namespace dss {
    struct DualCameraBase;

    struct CameraPerspective {
        unsigned int m_fovySin;                 // 0x00
        unsigned int m_fovyCos;                 // 0x04
        int m_aspect;                           // 0x08
        int m_near;                             // 0x0C
        int m_far;                              // 0x10
    };

    struct Camera {
        Vector3<short> m_angle;                 // 0x04
        Fix32Vector3 m_target_pos;               // 0x0C
        Fix32Vector3 m_pos;                      // 0x18
        Fix32Vector3 direction_;                 // 0x24
        Fix32Vector3 m_up;                       // 0x30
        Fix32 m_distance;                        // 0x3C
        Fix32 m_distanceSq;                      // 0x40
        int m_fov2;                             // 0x44
        Fix32 m_scaleW;                          // 0x48
        CameraPerspective m_perspective;        // 0x4C
        int unk_60;                             // 0x60

        Camera();
        virtual void update();
        virtual void calcPosition();
        void setup();
        void setPosition(const Fix32Vector3& pos);
        Fix32Vector3& getPosition();
        void setTarget(const Fix32Vector3& target);
        Fix32Vector3& getTarget();
        void setAngle(const Vector3short& angle);
        Vector3<short>& getAngle();
        void setDistance(const Fix32& distance);
        Fix32& getDistance();
        Fix32& getDistanceSq();
        Fix32Vector3& getDirection();
        void setFOV(unsigned int sin, unsigned int cos);
        void setFOV2(int fovy);
        void setScaleW(Fix32 scaleW);
        void setNear(int value);
        void setFar(int value);
        void applyCamera();
    };

}

// dss::Camera subclass whose target follows position + angle (main 0x02057b88); idk the real name yet
struct UnkCamera : dss::Camera {
    UnkCamera();
    virtual void update();
    virtual void calcPosition();
};

extern short data_020c4158[4];


extern int data_020c4160[40][2];

namespace dss {
    struct DualCameraBase {
        // vtable                               // 0x00
        Camera unk_004;                         // 0x04
        Camera unk_068;                         // 0x68
        int m_cameraNo;                         // 0xCC

        DualCameraBase();
        virtual void update();
        virtual void calcPosition();
        void updateCameraNo();
        void applyCamera();
        void applyG3d();
        Fix32Vector3& getPosition(int no);
        void setTarget(const Fix32Vector3& target, int no);
        Fix32Vector3& getTarget(int no);
    };

    struct DualCamera : DualCameraBase {
        int unk_d0;                             // 0xD0
        int unk_d4;                             // 0xD4
        Fix32 m_offset;                          // 0xD8
        short m_dirOffset;                      // 0xDC
        Vector3<short> unk_de;                  // 0xDE
        Vector3<short> unk_e4;                  // 0xE4
        char unk_ea[0xf0 - 0xea];
        int m_pursue;                           // 0xF0

        DualCamera();
        virtual void update();
        virtual void calcPosition();

        void setRotXYZ(Vector3short angle) { unk_004.setAngle(angle); }
        void setDistance(Fix32 distance)
        {
            unk_004.setDistance(distance);
            unk_068.setDistance(distance);
        }
        void setNear(const int& value)
        {
            unk_004.setNear(value);
            unk_068.setNear(value);
        }
        void setScaleW(Fix32 scaleW)
        {
            Camera* sub = &unk_004;
            sub->setScaleW(scaleW);
            (sub + 1)->setScaleW(scaleW);
        }
        void setOffset(Fix32 offset) { m_offset = offset; }
    };
}
