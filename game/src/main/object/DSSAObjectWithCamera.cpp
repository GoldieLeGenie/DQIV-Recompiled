#pragma ipa file
#include "main/object/DSSAObject.hpp"

dss::Camera* DSSAObjectWithCamera::camera_;
dss::Fix32 DSSAObjectWithCamera::distance_(2.5f);
dss::Fix32 DSSAObjectWithCamera::relativeScale_(0.4f);

ARM PaletteAnimation::PaletteAnimation()
{
    frame16_ = 0;
    frame256_ = 0;
}

ARM void PaletteAnimation::setup(void* data)
{
    PaletteAnimationHeader* header = (PaletteAnimationHeader*)data;
    header_ = *header++;
    if (header_.mode_ == 0) {
        frame16_ = (PaletteFrame16*)header;
    } else {
        frame256_ = (PaletteFrame256*)header;
    }
}

ARM int PaletteAnimation::getWait(int frame)
{
    if (header_.mode_ == 0) {
        return frame16_[frame].wait_;
    }
    return frame256_[frame].wait_;
}

ARM unsigned short* PaletteAnimation::getColor(int frame)
{
    if (header_.mode_ == 0) {
        return frame16_[frame].color_;
    }
    return frame256_[frame].color_;
}

ARM int PaletteAnimation::getColorCount()
{
    if (header_.mode_ == 0) {
        return 16;
    }
    return 256;
}

ARM int PaletteAnimation::getFrameCount()
{
    return header_.frameCount_;
}

ARM DSSAObjectWithCamera::DSSAObjectWithCamera()
{
}

ARM void DSSAObjectWithCamera::draw()
{
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 scale = *getScale();
    if (camera_) {
        if (type_ == Far) {
            execFar();
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
        if (type_ == Near) {
            execNear();
        }
        if (type_ == Normal2) {
            execNormal2();
        }
    }
    DSSAObject::draw();
    setPosition(position);
    setScale(scale);
}

ARM void DSSAObjectWithCamera::execute()
{
    DSSAObject::execute();
}

ARM void DSSAObjectWithCamera::execNormal2()
{
    static const dss::Fix32 rate(0xf33);
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position_;
    dss::Fix32Vector3 move = distance;
    move.normalize();
    setPosition(position_ + move);
    dss::Fix32Vector3 rest = distance - move;
    setScale(scale_ * (rest.length() / distance.length()) * rate);
}

ARM void DSSAObjectWithCamera::execNormal()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position_;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(position_ + (distance / 2));
    setScale((scale_ / 2));
}

ARM void DSSAObjectWithCamera::execFollow()
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

ARM void DSSAObjectWithCamera::execNear()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position_;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(position_ + (distance * 15 / 16));
    setScale((scale_ / 16));
}

ARM void DSSAObjectWithCamera::execNear2()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position_;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(position_ + (distance * 3 / 4));
    setScale((scale_ / 4));
}

ARM void DSSAObjectWithCamera::execFar()
{
    dss::Fix32Vector3 cameraPosition = camera_->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position_;
    dss::Fix32Vector3 unk1;
    dss::Fix32Vector3 unk2;
    setPosition(position_ + (distance * 1 / 4));
    setScale((scale_ * 3 / 4));
}
