#pragma ipa file
#include "main/object/DSSACharacter.hpp"
#include "main/script/sys/ScriptParam.hpp"

ARM DSSACharacter::DSSACharacter()
{
    flag_.clear();
    currentAnimationIndex_ = 0;
}

ARM DSSACharacter::~DSSACharacter()
{
}

ARM void DSSACharacter::setup(void* texture, DataObject* data)
{
    flag_.clear();
    texture_ = texture;
    dataObject_ = data;
    palette_.unkfunc_02086ccc((TextureObject*)texture, 0);
    currentAnimationIndex_ = -1;
    for (int i = 0; i < unkfunc_0207f8c4(dataObject_->getAddr()); i++) {
        if (isEnable(i)) {
            if (currentAnimationIndex_ == -1) {
                currentAnimationIndex_ = i;
                firstAnimationIndex_ = i;
            }
            dssaObject_[i].setup(unkfunc_0207f8dc(dataObject_->getAddr(), i));
            dssaObject_[i].setTexture(texture_);
            dssaObject_[i].setPalette(&palette_);
        }
    }
}

ARM void DSSACharacter::cleanup()
{
    palette_.unkfunc_02086d4c();
    if (unk_34 != 0) {
        ((TextureObject*)texture_)->unkfunc_02086868();
    }
    for (int i = 0; i < 14; i++) {
        if (dssaObject_[i].isEnable()) {
            dssaObject_[i].cleanup();
        }
    }
    unk_34 = 0;
    dataObject_ = 0;
}

ARM void DSSACharacter::draw()
{
    dssaObject_[currentAnimationIndex_].setPosition(position_);
    dssaObject_[currentAnimationIndex_].draw();
    dssaObject_[currentAnimationIndex_].execute();
    if (nextAnimationIndex_ != -1 && dssaObject_[currentAnimationIndex_].isEnd()) {
        start(nextAnimationIndex_, 0);
        if (flag_.check(0x20)) {
            dssaObject_[nextAnimationIndex_].setAlpha(dss::Fix32(0L));
        }
    }
    if (flag_.check(1)) {
        dss::Fix32 alpha(0x1f - flagCount_);
        alpha /= 0x1f;
        dssaObject_[currentAnimationIndex_].setAlpha(alpha);
        flagCount_++;
        flagCount_ = dss::clamp<int>(flagCount_, 0, 0x1f);
        if (flagCount_ == 0x1f) {
            flagCount_ = 0;
            flag_.flag_ &= ~1;
            dssaObject_[currentAnimationIndex_].setAlpha(dss::Fix32(0L));
        }
    }
    if (flag_.check(8)) {
        dss::Fix32 alpha(flagCount_);
        alpha /= 0x1f;
        dssaObject_[currentAnimationIndex_].setAlpha(alpha);
        flagCount_++;
        flagCount_ = dss::clamp<int>(flagCount_, 0, 0x1f);
        if (flagCount_ == 0x1f) {
            flagCount_ = 0;
            flag_.flag_ &= ~8;
            dssaObject_[currentAnimationIndex_].setAlpha(dss::Fix32(0x1000));
        }
    }
    if (flag_.check(2)) {
        if (flagCount_ & 4) {
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0L));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0L));
        } else {
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0x1000));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0x1000));
        }
        if (++flagCount_ == 0x19) {
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0x1000));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0x1000));
            flagCount_ = 0;
            flag_.flag_ &= ~2;
        }
    }
    if (flag_.check(4)) {
        if (flagCount_ & 4) {
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0L));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0L));
        } else {
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0x1000));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0x1000));
        }
        if (++flagCount_ >= 0x18) {
            flagCount_ = 0;
            flag_.flag_ &= ~4;
            dssaObject_[flagIndex_].setAlpha(dss::Fix32(0x1000));
            dssaObject_[firstAnimationIndex_].setAlpha(dss::Fix32(0L));
            currentAnimationIndex_ = firstAnimationIndex_;
        }
    }
}

ARM bool DSSACharacter::start(int index, int loop)
{
    if (index == 0x1e) {
        return true;
    }
    if (index == 0x22) {
        specialIndex_ = 0x22;
        flag_.flag_ |= 4;
        flagCount_ = 0;
        flagIndex_ = currentAnimationIndex_;
        return true;
    }
    if (index == 0x23) {
        specialIndex_ = 0x23;
        flag_.flag_ |= 2;
        flagIndex_ = currentAnimationIndex_;
        flagCount_ = 0;
        return true;
    }
    if (index == 0x24) {
        specialIndex_ = 0x24;
        dssaObject_[currentAnimationIndex_].setAlpha(dss::Fix32(0));
        return true;
    }
    if (index == 0x25) {
        specialIndex_ = 0x25;
        dssaObject_[currentAnimationIndex_].setAlpha(dss::Fix32(0x1000));
        return true;
    }
    if (index == 0x1f) {
        specialIndex_ = 0x1f;
        flag_.flag_ |= 1;
        flagCount_ = 0;
        return true;
    }
    if (index == 0x20) {
        specialIndex_ = 0x20;
        flag_.flag_ |= 8;
        flagCount_ = 0;
        return true;
    }
    if (index == 0x21) {
        specialIndex_ = 0x21;
        dssaObject_[currentAnimationIndex_].setAlpha(dss::Fix32(0));
        return true;
    }
    if (!isEnable(index)) {
        return false;
    }
    currentAnimationIndex_ = index;
    nextAnimationIndex_ = !loop ? firstAnimationIndex_ : -1;
    dssaObject_[currentAnimationIndex_].start(0);
    return true;
}

ARM void DSSACharacter::setAlpha(int alpha)
{
    dss::Fix32 value;
    value.value = (alpha << 12) / 31;
    dssaObject_[currentAnimationIndex_].setAlpha(value);
}

ARM bool DSSACharacter::isEnable(int index)
{
    if (dataObject_ == 0) {
        return false;
    }
    if (unkfunc_0207f8cc(dataObject_->getAddr(), index) != 0) {
        return true;
    }
    return false;
}

ARM int DSSACharacter::getCurrentFrame()
{
    return dssaObject_[currentAnimationIndex_].frame_;
}

ARM int DSSACharacter::getMaxFrame()
{
    return dssaObject_[currentAnimationIndex_].dssaData_.frame_;
}

ARM dss::Fix32Vector3 DSSACharacter::getNullPosition(int index, int type)
{
    return dssaObject_[index].getNullPosition(type);
}

ARM dss::Fix32 DSSACharacter::getWidth()
{
    dss::Fix32 width;
    dss::Fix32Vector3 pos[2];
    if (!dssaObject_[1].isEnable()) {
        width.value = 0;
        return width;
    }
    pos[0] = dssaObject_[1].getNullPosition(7);
    pos[1] = dssaObject_[1].getNullPosition(8);
    width = pos[1].vx - pos[0].vx;
    return width;
}

ARM int DSSACharacter::getWidthInt()
{
    if (!dssaObject_[1].isEnable()) {
        return 0;
    }
    int left = dssaObject_[1].getNullPositionInt(7).vx;
    return dssaObject_[1].getNullPositionInt(8).vx - left;
}

ARM dss::Fix32Vector3 DSSACharacter::getBoundingBox(int index)
{
    dss::Fix32 left(0L);
    dss::Fix32 right(0L);
    left = (long)dssaObject_[1].dssaData_.getAreaLeft(index);
    right = (long)dssaObject_[1].dssaData_.getAreaRight(index);
    dss::Fix32Vector3 ret;
    ret.vx = left;
    ret.vy = right;
    return ret;
}

ARM void DSSACharacter::setPositionInt(dss::Vector3int position)
{
    dss::Fix32Vector3 pos(0, 0, 0);
    pos.vx = (long)position.vx;
    pos.vy = (long)position.vy;
    pos.vz = (long)position.vz;
    pos *= DSSAObject::getDefaultScale();
    Position::setPosition(pos);
}

ARM void DSSACharacter::setCurrentFrame(int index, int frame)
{
    dssaObject_[index].setCurrentFrame(frame);
}

ARM void DSSACharacter::setCameraType(DSSAObjectWithCamera::CameraType type)
{
    for (int i = 0; i < 14; i++) {
        dssaObject_[i].type_ = type;
    }
}

ARM void DSSACharacter::pause(bool pause)
{
    for (int i = 0; i < 14; i++) {
        dssaObject_[i].pause(pause);
    }
}
