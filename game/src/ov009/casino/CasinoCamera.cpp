#pragma ipa file

#include "ov009/casino/CasinoCamera.hpp"

ARM CasinoCamera::CasinoCamera()
{
}

ARM CasinoCamera::~CasinoCamera()
{
}

ARM CasinoCamera* CasinoCamera::getSingleton()
{
    static CasinoCamera m_singleton;
    return &m_singleton;
}

static const float CASINO_CAMERA_DISTANCE = 45.0f;
static const dss::Fix32Vector3 position(0, 0, -4);
#pragma explicit_zero_data on
static short cameraAngleZ = 0;
static short cameraAngleY = 0;
static short cameraAngleX = -13835;
#pragma explicit_zero_data reset
static dss::Vector3<short> default_angle(cameraAngleX, cameraAngleY, cameraAngleZ);
static short cameraDirOffset = 1456;
static short cameraFov = 10;
static dss::Fix32 distance(CASINO_CAMERA_DISTANCE);
static dss::Fix32 offset(2L);

ARM void CasinoCamera::initialize()
{
    camera_.unk_004.setup();
    camera_.unk_068.setup();
    camera_.setTarget(position, 0);
    camera_.setDistance(distance);
    camera_.setRotXYZ(default_angle);
    camera_.setOffset(offset);
    camera_.m_dirOffset = cameraDirOffset;
    camera_.m_cameraNo = 1;
    camera_.unk_d0 = 0;
    camera_.unk_d4 = 0;
    short fov = cameraFov;
    camera_.unk_004.setFOV2(fov);
    camera_.unk_068.setFOV2(fov);
}

ARM void CasinoCamera::terminate()
{
}

ARM void CasinoCamera::draw()
{
    camera_.applyCamera();
}
