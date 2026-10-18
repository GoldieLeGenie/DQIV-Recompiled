#include "main/dss/Camera.hpp"
#include "main/dss/Matrix43.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif

#pragma explicit_zero_data on
short g_cameraDefaultAngle[4] = { 0, 0, 0, 0 };
#pragma explicit_zero_data reset

int g_cameraFovSinCosTable[40][2] = {
    { 0x00000000, 0x00001000 },
    { 0x00000047, 0x00000fff },
    { 0x0000008f, 0x00000ffe },
    { 0x000000d6, 0x00000ffa },
    { 0x0000011e, 0x00000ff6 },
    { 0x00000165, 0x00000ff0 },
    { 0x000001ac, 0x00000fea },
    { 0x000001f3, 0x00000fe1 },
    { 0x0000023a, 0x00000fd8 },
    { 0x00000281, 0x00000fce },
    { 0x000002c7, 0x00000fc2 },
    { 0x0000030e, 0x00000fb5 },
    { 0x00000354, 0x00000fa6 },
    { 0x00000399, 0x00000f97 },
    { 0x000003df, 0x00000f86 },
    { 0x00000424, 0x00000f74 },
    { 0x00000469, 0x00000f61 },
    { 0x000004ae, 0x00000f4d },
    { 0x000004f2, 0x00000f38 },
    { 0x00000536, 0x00000f21 },
    { 0x00000579, 0x00000f09 },
    { 0x000005bc, 0x00000ef0 },
    { 0x000005fe, 0x00000ed6 },
    { 0x00000640, 0x00000eba },
    { 0x00000682, 0x00000e9e },
    { 0x000006c3, 0x00000e80 },
    { 0x00000704, 0x00000e61 },
    { 0x00000744, 0x00000e42 },
    { 0x00000783, 0x00000e21 },
    { 0x000007c2, 0x00000dfe },
    { 0x00000800, 0x00000ddb },
    { 0x0000083e, 0x00000db7 },
    { 0x0000087b, 0x00000d92 },
    { 0x000008b7, 0x00000d6b },
    { 0x000008f2, 0x00000d44 },
    { 0x0000092d, 0x00000d1b },
    { 0x00000968, 0x00000cf2 },
    { 0x000009a1, 0x00000cc7 },
    { 0x000009da, 0x00000c9c },
    { 0x00000a12, 0x00000c6f },
};

ARM dss::Camera::Camera()
{
    unk_60 = 0;
    m_perspective.m_fovySin = 0x424;
    m_perspective.m_fovyCos = 0xf74;
    m_perspective.m_aspect = 0x1555;
    m_perspective.m_near = 0x400;
    m_perspective.m_far = 0xc8000;
    m_scaleW.value = 0x100;
    m_fov2 = 0xa;
    m_distance = 0x10L;
    m_distanceSq = m_distance * m_distance;
}

ARM void dss::Camera::setup()
{
    m_target_pos.setFix32(0, 0, 0);
    m_angle.vx = g_cameraDefaultAngle[0];
    m_angle.vy = g_cameraDefaultAngle[2];
    m_angle.vz = g_cameraDefaultAngle[1];
    m_distance = 0x10L;
    m_distanceSq = m_distance * m_distance;
    m_up.setFix32(0, 1, 0);
}

ARM void dss::Camera::calcPosition()
{
    dss::UnkMatrix43 rotX;
    dss::UnkMatrix43 rotY;
    rotX.unkfunc_02088698(m_angle.vx);
    rotY.unkfunc_020886d0(m_angle.vy);
    direction_.setFix32(0, 0, 1);
    direction_ *= m_distance;
    direction_ = rotX * direction_;
    direction_ = rotY * direction_;
    m_pos = m_target_pos + direction_;
    direction_ = direction_ * -1;
    direction_.normalize();
}

ARM void dss::Camera::applyCamera()
{
    if (unk_60 == 0) {
        update();
    }
    calcPosition();
    func_02065604(m_perspective.m_fovySin, m_perspective.m_fovyCos, m_perspective.m_aspect, m_perspective.m_near, m_perspective.m_far, m_scaleW.value, 0, &data_0210cf28.projMtx);
#ifdef DQIV_NATIVE
    if (pc::wideBattleFrame()) {
        pc::extendProjection(data_0210cf28.projMtx.m);
    }
#endif
    data_0210cf28.flag &= ~0x50;
    data_0210cf28.camPos.x = m_pos.vx.value;
    data_0210cf28.camPos.y = m_pos.vy.value;
    data_0210cf28.camPos.z = m_pos.vz.value;
    data_0210cf28.camUp.x = m_up.vx.value;
    data_0210cf28.camUp.y = m_up.vy.value;
    data_0210cf28.camUp.z = m_up.vz.value;
    data_0210cf28.camTarget.x = m_target_pos.vx.value;
    data_0210cf28.camTarget.y = m_target_pos.vy.value;
    data_0210cf28.camTarget.z = m_target_pos.vz.value;
    func_02065a98((VecFx32*)&m_pos, (VecFx32*)&m_up, (VecFx32*)&m_target_pos, 0, &data_0210cf28.cameraMtx);
    data_0210cf28.flag &= ~0xe8;
}

ARM void dss::Camera::update()
{
}

ARM void dss::Camera::setPosition(const Fix32Vector3& pos)
{
    m_pos = pos;
}

ARM dss::Fix32Vector3& dss::Camera::getPosition()
{
    return m_pos;
}

ARM void dss::Camera::setTarget(const Fix32Vector3& target)
{
    m_target_pos = target;
}

ARM dss::Fix32Vector3& dss::Camera::getTarget()
{
    return m_target_pos;
}

ARM void dss::Camera::setAngle(const Vector3short& angle)
{
    m_angle.vx = angle.vx;
    m_angle.vy = angle.vy;
    m_angle.vz = angle.vz;
}

ARM dss::Vector3<short>& dss::Camera::getAngle()
{
    return m_angle;
}

ARM void dss::Camera::setDistance(const Fix32& distance)
{
    m_distance = distance;
    m_distanceSq = m_distance * m_distance;
}

ARM dss::Fix32& dss::Camera::getDistance()
{
    return m_distance;
}

ARM dss::Fix32& dss::Camera::getDistanceSq()
{
    return m_distanceSq;
}

ARM dss::Fix32Vector3& dss::Camera::getDirection()
{
    return direction_;
}

ARM void dss::Camera::setFOV(unsigned int sin, unsigned int cos)
{
    m_perspective.m_fovySin = sin;
    m_perspective.m_fovyCos = cos;
}

ARM void dss::Camera::setFOV2(int fovy)
{
    int index = (int)dss::clamp<int>(fovy, 0, 0x4e) / 2;
    m_fov2 = index;
    setFOV(g_cameraFovSinCosTable[index][0], g_cameraFovSinCosTable[index][1]);
}

ARM void dss::Camera::setScaleW(Fix32 scaleW)
{
    m_scaleW = scaleW;
}

ARM void dss::Camera::setNear(int value)
{
    m_perspective.m_near = value;
}

ARM void dss::Camera::setFar(int value)
{
    m_perspective.m_far = value;
}
