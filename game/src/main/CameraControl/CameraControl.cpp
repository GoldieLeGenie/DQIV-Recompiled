#include "main/CameraControl/CameraControl.hpp"
#include "main/status/HaveStatusInfo.hpp"
#include "main/dss/UnkMatrix43.hpp"

static short MAX_ROTATION = 0x3f00;

ARM CameraControl::CameraControl()
{
}

ARM CameraControl::~CameraControl()
{
}

ARM void CameraControl::terminate()
{
    if (this->data_.getAddr() == 0)
    {
        return;
    }
    this->data_.cleanup();
}

ARM void CameraControl::initCameraControl(dss::Fix32Vector3 position, dss::Vector3<short> angle)
{
    this->dt_ = 0;
    this->waitCounter_ = 0;
    this->seqPhase_ = 1;
    this->iniAngle_.vx = angle.vx;
    this->iniAngle_.vy = angle.vy;
    this->iniAngle_.vz = angle.vz;
    this->iniPosition_ = position;
}

ARM void CameraControl::readCameraData(const char* fname, int arg)
{
    const char* path = "data/camera/";
    dss::sprintf_s(camera_name, sizeof(camera_name), "%s%s", path, fname);
    if (data_.getAddr() != 0) {
        data_.cleanup();
    }
    if (dss::strlen(fname) == 0) {
        seqPhase_ = 1;
        maxSeqPhase_ = 0;
        return;
    }
    data_.setup(camera_name, arg, 1);
    int* addr = (int*)data_.getAddr();
    maxSeqPhase_ = *addr++;
    seqData_ = (SeqCameraControl*)addr;
}

ARM bool CameraControl::calc(dss::Fix32Vector3& arg_position, dss::Vector3<short>& arg_angle)
{
    static short MIN_ROTATION = -MAX_ROTATION;
    if (wait_ > waitCounter_) {
        arg_position = iniPosition_;
        arg_angle.vx = iniAngle_.vx;
        arg_angle.vy = iniAngle_.vy;
        arg_angle.vz = iniAngle_.vz;
        waitCounter_++;
        return true;
    }
    if (seqPhase_ >= maxSeqPhase_) {
        return false;
    }
    dss::Fix32Vector3 cur_angle;
    dss::Fix32Vector3 cur_position;
    dss::Fix32Vector3 unused_vec0;
    dss::UnkMatrix43 unused_mtx0;
    dss::Fix32Vector3 unused_vec1;
    dss::UnkMatrix43 unused_mtx1;

    cur_angle.vx = seqData_[seqPhase_ - 1].angEnd.vx + seqData_[seqPhase_].ang.vx * dt_ +
                   seqData_[seqPhase_].ang_a.vx * dt_ * dt_ * 3 / 2;
    cur_angle.vy = seqData_[seqPhase_ - 1].angEnd.vy + seqData_[seqPhase_].ang.vy * dt_ +
                   seqData_[seqPhase_].ang_a.vy * dt_ * dt_ * 3 / 2;
    arg_angle.vx = iniAngle_.vx + (cur_angle.vx.value >> 12);
    arg_angle.vy = iniAngle_.vy + (cur_angle.vy.value >> 12);
    arg_angle.vx = status::HaveStatusInfo::getClampValue(arg_angle.vx, MIN_ROTATION, MAX_ROTATION);

    cur_position.vx = seqData_[seqPhase_ - 1].posEnd.vx + seqData_[seqPhase_].pos.vx * dt_ +
                      seqData_[seqPhase_].pos_a.vx * dt_ * dt_ * 3 / 2;
    cur_position.vx = iniPosition_.vx + cur_position.vx.value / 4096;
    arg_position.vx = cur_position.vx;
    cur_position.vz = seqData_[seqPhase_ - 1].posEnd.vz + seqData_[seqPhase_].pos.vz * dt_ +
                      seqData_[seqPhase_].pos_a.vz * dt_ * dt_ * 3 / 2;
    cur_position.vz = iniPosition_.vz + cur_position.vz.value / 4096;
    arg_position.vz = cur_position.vz;
    cur_position.vy = seqData_[seqPhase_ - 1].posEnd.vy + seqData_[seqPhase_].pos.vy * dt_ +
                      seqData_[seqPhase_].pos_a.vy * dt_ * dt_ * 3 / 2;
    cur_position.vy = iniPosition_.vy + cur_position.vy.value / 4096;
    arg_position.vy = cur_position.vy;

    if (dt_ >= seqData_[seqPhase_].frame) {
        dt_ = 0;
        seqPhase_++;
    }
    dt_++;
    return true;
}

ARM bool CameraControl::moveCamera(dss::Fix32Vector3& arg_position, dss::Vector3<short>& arg_angle)
{
    arg_angle.vx = seqData_->angEnd.vx.value >> 12;
    arg_angle.vy = seqData_->angEnd.vy.value >> 12;
    arg_angle.vz = seqData_->angEnd.vz.value >> 12;
    arg_position.vx.value = seqData_->posEnd.vx.value >> 12;
    arg_position.vy.value = seqData_->posEnd.vy.value >> 12;
    arg_position.vz.value = seqData_->posEnd.vz.value >> 12;
    return true;
}
