#pragma ipa file
#include "main/dss/Camera.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Matrix43.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif

// camera constants shared with the UnkCamera TU (same block there), unused here
static const dss::Fix32 s_unk0(0.0625f);
static const dss::Fix32 s_unk1(150L);
static const dss::Fix32 s_unk2(4L);
static const dss::Fix32 s_unk3(4L);
static const dss::Fix32 s_unk4(-4L);

// default angles of the two cameras
#pragma explicit_zero_data on
static short s_camera0DefaultAngleX = 0x2000;
static short s_camera0DefaultAngleY = 0;
static short s_camera0DefaultAngleZ = 0;
static short s_camera1DefaultAngleX = 0x2000;
static short s_camera1DefaultAngleY = 0;
static short s_camera1DefaultAngleZ = 0;
#pragma explicit_zero_data reset

ARM dss::DualCameraBase::DualCameraBase()
{
    m_camera0.m_pos.setFix32(0, 0, 1);
    m_camera0.m_up.setFix32(0, 1, 0);
    m_camera0.m_angle.vx = s_camera0DefaultAngleX;
    m_camera0.m_angle.vy = s_camera0DefaultAngleY;
    m_camera0.m_angle.vz = s_camera0DefaultAngleZ;
    m_camera0.m_distance = 8L;
    m_camera0.m_fov2 = 5;
    m_camera1.m_pos.setFix32(0, 0, 1);
    m_camera1.m_up.setFix32(0, 1, 0);
    m_camera1.m_angle.vx = s_camera1DefaultAngleX;
    m_camera1.m_angle.vy = s_camera1DefaultAngleY;
    m_camera1.m_angle.vz = s_camera1DefaultAngleZ;
    m_camera1.m_distance = 8L;
    m_camera1.m_fov2 = 5;
    m_cameraNo = 1;
}

ARM void dss::DualCameraBase::updateCameraNo()
{
    if (getDisplayFrame() & 1) {
        m_cameraNo = 0;
    } else {
        m_cameraNo = 1;
    }
}

ARM void dss::DualCameraBase::update()
{
}

ARM void dss::DualCameraBase::calcPosition()
{
    updateCameraNo();
    dss::Matrix43 rotX;
    dss::Matrix43 rotY;
    Camera* camera = &m_camera0;
    camera->calcPosition();
}

ARM void dss::DualCameraBase::applyCamera()
{
    update();
    calcPosition();
    applyG3d();
}

ARM void dss::DualCameraBase::applyG3d()
{
#ifdef DQIV_NATIVE
    // One picture for both screens: always the bottom camera (see DualScreen.hpp).
    if (pc::unifiedFrame()) {
        m_cameraNo = 0;
    } else
#endif
    updateCameraNo();
    Camera* cam = m_cameraNo != 0 ? &m_camera1 : &m_camera0;
    CameraPerspective perspective = cam->m_perspective;
    Fix32Vector3 pos = cam->m_pos;
    Fix32Vector3 target = cam->m_target_pos;
    Fix32Vector3 up = cam->m_up;
    Fix32 scaleW = cam->m_scaleW;
    func_02065604(perspective.m_fovySin, perspective.m_fovyCos, perspective.m_aspect, perspective.m_near, perspective.m_far, scaleW.value, 0, &data_0210cf28.projMtx);
#ifdef DQIV_NATIVE
    if (pc::unifiedFrame()) {
        pc::extendProjection(data_0210cf28.projMtx.m);
    }
#endif
    data_0210cf28.flag &= ~0x50;
    data_0210cf28.camPos.x = pos.vx.value;
    data_0210cf28.camPos.y = pos.vy.value;
    data_0210cf28.camPos.z = pos.vz.value;
    data_0210cf28.camUp.x = up.vx.value;
    data_0210cf28.camUp.y = up.vy.value;
    data_0210cf28.camUp.z = up.vz.value;
    data_0210cf28.camTarget.x = target.vx.value;
    data_0210cf28.camTarget.y = target.vy.value;
    data_0210cf28.camTarget.z = target.vz.value;
    func_02065a98((VecFx32*)&pos, (VecFx32*)&up, (VecFx32*)&target, 0, &data_0210cf28.cameraMtx);
    data_0210cf28.flag &= ~0xe8;
}

ARM dss::Fix32Vector3& dss::DualCameraBase::getPosition(int no)
{
    if (no == 0) {
        return m_camera0.getPosition();
    }
    return m_camera1.getPosition();
}

ARM void dss::DualCameraBase::setTarget(const Fix32Vector3& target, int no)
{
    if (no == 0) {
        m_camera0.setTarget(target);
        return;
    }
    m_camera1.setTarget(target);
}

ARM dss::Fix32Vector3& dss::DualCameraBase::getTarget(int no)
{
    if (no == 0) {
        return m_camera0.getTarget();
    }
    return m_camera1.getTarget();
}

ARM dss::DualCamera::DualCamera()
{
    dss::Fix32Vector3 zero(0, 0, 0);
    setTarget(zero, 0);
    m_offset = 0.0f;
    m_cameraNo = 1;
    m_dirOffset = 0;
    m_pursue = 1;
}

ARM void dss::DualCamera::update()
{
}

ARM void dss::DualCamera::calcPosition()
{
    if (m_pursue == 1) {
        DualCameraBase::calcPosition();
    } else {
        Fix32Vector3 dir = m_camera0.getTarget() - m_camera0.getPosition();
        dir.normalize();
        m_camera0.direction_ = dir;
    }
    dss::Matrix43 rotX;
    dss::Matrix43 rotY;
    rotX.unkfunc_02088698(m_camera0.m_angle.vx);
    rotY.unkfunc_020886d0(m_camera0.m_angle.vy);
    Fix32Vector3 up(0, 1, 0);
    Fix32Vector3 offset;
    up = rotX * up;
    up = rotY * up;
    offset = up * m_offset;
    m_camera1.setPosition(m_camera0.getPosition() + offset);
    m_camera1.setTarget(m_camera0.getTarget() + offset);
    m_camera1.setDistance(m_camera0.getDistance());
    m_camera1.setAngle(m_camera0.getAngle());
    offset = m_camera1.m_target_pos - m_camera1.m_pos;
    rotX.unkfunc_02088698(m_dirOffset);
    rotY.unkfunc_020886d0(-m_camera1.m_angle.vy);
    offset = rotY * offset;
    offset = rotX * offset;
    rotY.unkfunc_020886d0(m_camera1.m_angle.vy);
    offset = rotY * offset;
    m_camera1.m_target_pos = m_camera1.m_pos + offset;
}
