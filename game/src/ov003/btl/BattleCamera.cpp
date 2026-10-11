#pragma ipa file
#include "ov003/btl/BattleCamera.hpp"
#include "main/param/Param.hpp"

CameraControl livecamera;
CameraControl livecamera2;
int BattleCamera::cameraSetting;

THUMB BattleCamera::BattleCamera()
{
}

THUMB BattleCamera::~BattleCamera()
{
}

THUMB BattleCamera* BattleCamera::getSingleton()
{
    static BattleCamera m_singleton;
    return &m_singleton;
}

THUMB void BattleCamera::initialize()
{
    normalCamera_.setup();
    dss::Fix32Vector3 position(0.0f, 0.69f, 6.82f);
    normalCamera_.setPosition(position);
    normalCamera_.setFOV(0x6c3, 0xe80);
    normalCamera_.setFar(0x400000);
    normalCamera_.setFOV2(0x32);
    normalCamera_.unk_60 = 1;
    dss::Fix32 scaleW;
    scaleW.value = 0x1000;
    normalCamera_.setScaleW(scaleW);
    enable_ = 0;
    camera1 = 0;
    camera2 = 0;
    reset();
    normalCamera_.applyCamera();
    setFilename("start", 0);
    initCamera();
}

THUMB void BattleCamera::terminate()
{
    livecamera.terminate();
    livecamera2.terminate();
}

THUMB void BattleCamera::reset()
{
    dss::Vector3<short> angle;
    dss::Fix32Vector3 position;
    livecamera.readCameraData("inicamera", 0);
    livecamera.moveCamera(position, angle);
    normalCamera_.setPosition(position);
    normalCamera_.setAngle(angle);
    initposition_ = position;
}

THUMB void BattleCamera::executeForMap()
{
    dss::Vector3<short> angle;
    dss::Vector3<short> angle2;
    dss::Fix32Vector3 position;
    dss::Fix32Vector3 position2;

    if (enable_ != 0) {
        if (livecamera.calc(position, angle)) {
            if (livecamera2.calc(position2, angle2)) {
                dss::Vector3<short> rot;
                rot.vx -= livecamera.iniAngle_.vx;
                rot.vy -= livecamera.iniAngle_.vy;
                rot.vz -= livecamera.iniAngle_.vz;
                rot.vx += angle.vx + angle2.vx;
                rot.vy += angle.vy + angle2.vy;
                rot.vz += angle.vz + angle2.vz;
                normalCamera_.setAngle(rot);
                normalCamera_.setPosition((position + position2 - livecamera.iniPosition_));
                normalCamera_.applyCamera();
            }
            else {
                normalCamera_.setAngle(angle);
                normalCamera_.setPosition(position);
                normalCamera_.applyCamera();
                camera2 = 0;
            }
        }
        else {
            if (livecamera2.calc(position2, angle2)) {
                normalCamera_.setAngle(angle2);
                normalCamera_.setPosition(position2);
                normalCamera_.applyCamera();
            }
            else {
                camera2 = 0;
                reset();
                enable_ = 0;
            }
            camera1 = 0;
        }
    }

    if (homing_.step_ != 0) {
        dss::Vector3short& current = normalCamera_.getAngle();
        angle.vx = current.vx;
        angle.vy = current.vy;
        angle.vz = current.vz;
        homing_.calculation(angle);
        normalCamera_.setAngle(angle);
    }
    normalCamera_.applyCamera();
}

THUMB void BattleCamera::draw()
{
}

THUMB void BattleCamera::initCamera()
{
    enable_ = 1;
    if (camera1 != 0) {
        livecamera.readCameraData(file_, 1);
        livecamera.initCameraControl(normalCamera_.getPosition(), normalCamera_.getAngle());
    }
    if (camera2 != 0) {
        livecamera2.readCameraData(file2_, 1);
        livecamera2.initCameraControl(normalCamera_.getPosition(), normalCamera_.getAngle());
    }
}

THUMB void BattleCamera::setCameraAnimation(unsigned char camera1, unsigned char camera2, unsigned short wait)
{
    this->camera1 = 1;
    this->camera2 = 1;
    param::EffectParam::getCameraFile(camera1, file_);
    param::EffectParam::getCameraFile(camera2, file2_);
    setWait(wait);
    getSingleton()->initCamera();
}

THUMB void BattleCamera::setHomingTarget(int drawCtrlId)
{
    homing_.setup(initposition_, drawCtrlId);
}

THUMB void BattleCamera::setWait(int wait)
{
    livecamera2.wait_ = wait;
}

THUMB dss::Camera* BattleCamera::getCamera()
{
    return &normalCamera_;
}

THUMB void BattleCamera::setFilename(const char* file, const char* file2)
{
    camera2 = 0;
    camera1 = 1;
    dss::strcpy_s(file_, 16, (char*)file);
    if (file2 != 0) {
        camera2 = 1;
        dss::strcpy_s(file2_, 16, (char*)file2);
    }
}

// keeps cameraSetting in the .bss pool
THUMB void BattleCamera::setCameraSeting(bool flag)
{
    cameraSetting = flag;
}

THUMB bool BattleCamera::isCameraAnimation()
{
    return enable_ == 1 || homing_.step_ != 0;
}
