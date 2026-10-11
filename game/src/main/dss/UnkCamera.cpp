#pragma ipa file
#include "main/dss/Camera.hpp"
#include "main/dss/UnkMatrix43.hpp"

// constructed by the sinit but never used
static UnkCamera camera;

// camera constants shared with the DualCamera TU (same block there), unused here
static const dss::Fix32 s_unk0(0.0625f);
static const dss::Fix32 s_unk1(150L);
static const dss::Fix32 s_unk2(4L);
static const dss::Fix32 s_unk3(4L);
static const dss::Fix32 s_unk4(-4L);

ARM UnkCamera::UnkCamera()
{
    m_pos.setFix32(0, 0, 10);
    m_up.setFix32(0, 1, 0);
    m_angle.set(0, 0, 0);
    setDistance(dss::Fix32(16L));
}

ARM void UnkCamera::update()
{
}

ARM void UnkCamera::calcPosition()
{
    dss::UnkMatrix43 rotX;
    dss::UnkMatrix43 rotY;
    dss::UnkMatrix43 rot;
    rotX.unkfunc_02088698(m_angle.vx);
    rotY.unkfunc_020886d0(m_angle.vy);
    rot = rotX * rotY;
    direction_.setFix32(0, 0, -10);
    direction_ = rot * direction_;
    m_target_pos = direction_ * getDistance() + m_pos;
}
