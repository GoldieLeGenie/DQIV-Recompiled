#pragma ipa file
#include "main/effect/UnkEffectCamera.hpp"

static const float s_distanceF = 34.2901f;

static const dss::Fix32Vector3 s_target(0, 0, 0);
static dss::Vector3<short> s_angle(0, 0, 0);
static dss::Fix32 s_distance(s_distanceF);
static dss::Fix32 s_offset(2L);

// unreferenced global: it is dead-stripped at link but takes part in the .data/.bss heapsort order
short unusedDirOffset = 0x5b0;
static short s_fov = 10;

THUMB UnkEffectCamera::UnkEffectCamera()
{
}

THUMB UnkEffectCamera::~UnkEffectCamera()
{
}

THUMB UnkEffectCamera* UnkEffectCamera::getSingleton()
{
    static UnkEffectCamera m_singleton;
    return &m_singleton;
}

THUMB void UnkEffectCamera::initialize()
{
    camera_.setup();
    camera_.setTarget(s_target);
    camera_.setDistance(s_distance);
    camera_.setAngle(s_angle);
    camera_.setFOV2(s_fov);
}

THUMB void UnkEffectCamera::terminate()
{
}

THUMB void UnkEffectCamera::draw()
{
    camera_.applyCamera();
}
