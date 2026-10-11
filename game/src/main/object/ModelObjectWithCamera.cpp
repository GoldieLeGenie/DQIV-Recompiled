#pragma ipa file
#include "main/object/ModelObject.hpp"

dss::Camera* ModelObjectWithCamera::camera_;
dss::Fix32 ModelObjectWithCamera::distance_(2.5f);
dss::Fix32 ModelObjectWithCamera::relativeScale_(0.4f);

ARM ModelObjectWithCamera::ModelObjectWithCamera()
{
}

ARM void ModelObjectWithCamera::draw()
{
    dss::Fix32Vector3 position = getPosition();
    dss::Fix32Vector3 scale = getScale();
    if (camera_) {
        if (type_ == Far) {
            execFar();
        }
        if (type_ == Near) {
            execNear();
        }
        if (type_ == Near2) {
            execNear2();
        }
        if (type_ == Normal) {
            execNormal();
        }
        if (type_ == Follow) {
            execFollow();
        }
    }
    ModelObject::draw();
    setPosition(position);
    setScale(scale);
}

ARM void ModelObjectWithCamera::execNormal()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = getPosition();
    dss::Fix32Vector3 distance = cameraPosition - m_pos;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(m_pos + (distance / 2));
    setScale(m_scl / 2);
}

ARM void ModelObjectWithCamera::execFollow()
{
    dss::Fix32Vector3 cameraPosition;
    dss::Fix32Vector3 target;
    dss::Fix32Vector3 direction;
    cameraPosition = camera_->getPosition();
    target = camera_->getTarget();
    direction = target - cameraPosition;
    direction.normalize();
    target = cameraPosition + direction * distance_;
    setPosition(target);
    setScale(relativeScale_);
}

ARM void ModelObjectWithCamera::execNear()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = getPosition();
    dss::Fix32Vector3 distance = cameraPosition - m_pos;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(m_pos + (distance * 15 / 16));
    setScale(m_scl / 16);
}

ARM void ModelObjectWithCamera::execNear2()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = getPosition();
    dss::Fix32Vector3 distance = cameraPosition - m_pos;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(m_pos + (distance * 3 / 4));
    setScale(m_scl / 4);
}

ARM void ModelObjectWithCamera::execFar()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = getPosition();
    dss::Fix32Vector3 distance = cameraPosition - m_pos;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(m_pos + (distance * 1 / 4));
    setScale(m_scl * 3 / 4);
}
