#pragma ipa file
#include "ov006/BookCamera.hpp"

static const float s_distanceF = 17.35f;

static const dss::Fix32Vector3 s_target(3.25f, 0.0f, 0.0f);
static dss::Vector3<short> s_angle(-0x216c, 0, 0);
static dss::Fix32 s_distance(s_distanceF);
static dss::Fix32 s_offset(2L);
static short s_dirOffset = 0x5b0;
static short s_fov = 50;

ARM BookCamera::BookCamera()
{
}

ARM BookCamera::~BookCamera()
{
}

ARM BookCamera* BookCamera::getSingleton()
{
    static BookCamera m_singleton;
    return &m_singleton;
}

ARM void BookCamera::initialize()
{
    camera_.unk_004.setup();
    camera_.unk_068.setup();
    camera_.setTarget(s_target, 0);
    camera_.setDistance(s_distance);
    camera_.setRotXYZ(s_angle);
    camera_.setOffset(s_offset);
    camera_.m_dirOffset = s_dirOffset;
    camera_.m_cameraNo = 1;
    camera_.unk_d0 = 0;
    camera_.unk_d4 = 0;
    int fov = s_fov;
    camera_.unk_004.setFOV2(fov);
    camera_.unk_068.setFOV2(fov);
    camera_.setScaleW(dss::Fix32(0x1000));
}

ARM void BookCamera::terminate()
{
}

ARM void BookCamera::draw()
{
    camera_.applyCamera();
}
