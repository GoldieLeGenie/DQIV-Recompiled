#pragma ipa file
#include "main/cmn/ActionBase.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/status/BaseActionStatus.hpp"

dss::Fix32Vector3& cmn::ActionBase::position_ = g_cmnPartyInfo.position_;
short& cmn::ActionBase::dirIdx_ = g_cmnPartyInfo.getDirIdx2();
const dss::Fix32 cmn::MoveBase::grav(0x29);


ARM void cmn::MoveBase::setup()
{
    endMoveFrame_ = 0;
    endRotFrame_ = 0;
    moveCounter_ = 0;
    rotCounter_ = 0;
    moveFlag_ = 0;
    rotFlag_ = 0;
    moveType_ = 0;
}

ARM void cmn::MoveBase::execMove(dss::Fix32Vector3& pos)
{
    switch (moveType_) {
        case 1:
            simpleMove(pos);
            break;
        case 2:
            moveVibMotion(pos);
            break;
        case 3:
            shakeExecute(pos);
            break;
        case 4:
            jumpExecute(pos);
            break;
        case 5:
            moveAddExecute(pos);
            break;
        case 6:
            dirMoveExec(pos);
            break;
    }
}

ARM void cmn::MoveBase::execRot(dss::Vector3<short>& rot)
{
    simpleRot(rot);
}

ARM int cmn::MoveBase::moveUpdate()
{
    switch (moveType_) {
        case 1:
            return simpleMoveUpdate();
        case 2:
            return updateVibMotion();
        case 3:
            return updateShake();
        case 4:
            return updateJump();
        case 5:
            return updateMoveAdd();
        case 6:
            return updateDirMove();
    }
    return 0;
}

ARM int cmn::MoveBase::rotUpdate()
{
    return simpleRotUpdata();
}

ARM bool cmn::MoveBase::isEnd()
{
    if (moveFlag_ == 0 && rotFlag_ == 0) {
        return true;
    }
    return false;
}

ARM void cmn::MoveBase::setActionMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target)
{
    moveType_ = 1;
    startPos_ = start;
    targetPos_ = target;
    moveFlag_ = 1;
    moveCounter_ = 0;
}

ARM void cmn::MoveBase::setActionRot(dss::Vector3<short>& start, const dss::Vector3<short>& target)
{
    moveType_ = 1;
    startDirIdx_ = start;
    targetDirIdx_ = target;
    rotFlag_ = 1;
    rotCounter_ = 0;
}

ARM void cmn::MoveBase::setMoveSpeed(dss::Fix32 speed)
{
    moveVec_ = (targetPos_ - startPos_);
    dss::Fix32 frame = moveVec_.length() / speed;
    endMoveFrame_ = frame.value / 0x1000;
    setMoveFrame(endMoveFrame_);
}

ARM void cmn::MoveBase::setMoveFrame(int frame)
{
    endMoveFrame_ = frame;
    int div = dss::max<int>(frame, 1);
    moveVec_ = ((targetPos_ - startPos_) / div);
}

ARM void cmn::MoveBase::setRotFrame(int frame, int type)
{
    endRotFrame_ = frame;
    rotIdx_.vx = targetDirIdx_.vx - startDirIdx_.vx;
    rotIdx_.vy = targetDirIdx_.vy - startDirIdx_.vy;
    rotIdx_.vz = targetDirIdx_.vz - startDirIdx_.vz;
    if (frame != 0) {
        rotIdx_.vx = setRot(rotIdx_.vx, frame, type);
        rotIdx_.vy = setRot(rotIdx_.vy, frame, type);
        rotIdx_.vz = setRot(rotIdx_.vz, frame, type);
    }
}

ARM short cmn::MoveBase::setRot(short rot, int frame, int type)
{
    if (rot == 0) {
        return rot;
    }
    switch (type) {
        case 1:
            if (rot < 0) {
                rot = rot / frame;
            } else {
                rot = (0xffff - rot) / frame;
                rot *= -1;
            }
            break;
        case 2:
            if (rot > 0) {
                rot = rot / frame;
            } else {
                rot = (unsigned short)((unsigned short)rot / frame);
            }
            break;
        default:
            rot = rot / frame;
            break;
    }
    return rot;
}

ARM void cmn::MoveBase::simpleMove(dss::Fix32Vector3& pos)
{
    if (moveFlag_ != 1) {
        return;
    }
    if (moveCounter_ < endMoveFrame_) {
        pos = startPos_ + moveVec_ * moveCounter_;
    } else {
        pos = targetPos_;
    }
}

ARM void cmn::MoveBase::simpleRot(dss::Vector3<short>& rot)
{
    if (rotFlag_ != 1) {
        return;
    }
    if (rotCounter_ < endRotFrame_) {
        rot.vx = startDirIdx_.vx + rotIdx_.vx * rotCounter_;
        rot.vy = startDirIdx_.vy + rotIdx_.vy * rotCounter_;
        rot.vz = startDirIdx_.vz + rotIdx_.vz * rotCounter_;
    } else {
        rot = targetDirIdx_;
    }
}

ARM void cmn::MoveBase::setSimpleRot(dss::Vector3<short>& start, dss::Vector3<short>& add, int frame)
{
    rotIdx_ = add;
    endRotFrame_ = frame;
    startDirIdx_ = start;
    targetDirIdx_.vx = start.vx + add.vx * frame;
    targetDirIdx_.vy = start.vy + add.vy * frame;
    targetDirIdx_.vz = start.vz + add.vz * frame;
    rotCounter_ = 0;
    rotFlag_ = 1;
}

ARM void cmn::MoveBase::setRotSpeedY(short speed)
{
    rotIdx_.vx = 0;
    rotIdx_.vy = targetDirIdx_.vy - startDirIdx_.vy;
    endRotFrame_ = rotIdx_.vy / speed;
    endRotFrame_ = status::BaseActionStatus::abs(endRotFrame_);
    rotIdx_.vz = 0;
    rotIdx_.vy = setRot(rotIdx_.vy, endRotFrame_, 0);
    rotCounter_ = 0;
    rotFlag_ = 1;
}

ARM int cmn::MoveBase::simpleMoveUpdate()
{
    if (moveFlag_ == 1) {
        if (moveCounter_ >= endMoveFrame_) {
            moveFlag_ = 0;
            return 1;
        }
        moveCounter_++;
    } else {
        return 1;
    }
    return 0;
}

ARM int cmn::MoveBase::simpleRotUpdata()
{
    if (rotFlag_ == 1) {
        if (rotCounter_ >= endRotFrame_) {
            rotFlag_ = 0;
            return 1;
        }
        rotCounter_++;
    } else {
        return 1;
    }
    return 0;
}

namespace cmn {
    static dss::Fix32Vector3 shakeOffset;
}

ARM void cmn::MoveBase::setVibMotion(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int amp, int damp, int frame)
{
    endMoveFrame_ = frame;
    startPos_ = start;
    targetPos_ = start;
    moveVec_ = ((target - start) * 4 / 8);
    moveCounter_ = 0;
    dampFrame_ = damp;
    ampFrame_ = amp;
    moveType_ = 2;
    moveFlag_ = 1;
    shakeOffset.set(0, 0, 0);
}

ARM void cmn::MoveBase::moveVibMotion(dss::Fix32Vector3& pos)
{
    int count = moveCounter_ % 8;
    int sign = (count < 4) ? 1 : -1;
    count = count % 4;
    if (count >= 2) {
        count = 4 - count;
    }
    dss::Fix32Vector3 vec = (moveVec_ * (sign * count) / 2);
    if (moveCounter_ < ampFrame_) {
        vec = (vec * moveCounter_ / ampFrame_);
    } else if (moveCounter_ > dampFrame_) {
        vec = vec * (endMoveFrame_ - moveCounter_) / (endMoveFrame_ - dampFrame_);
    }
    shakeOffset += vec;
    if (moveLock_ == 1) {
        pos += shakeOffset;
    } else {
        pos += vec;
    }
    if (moveCounter_ >= endMoveFrame_) {
        pos -= shakeOffset;
    }
}

ARM int cmn::MoveBase::updateVibMotion()
{
    if (moveCounter_ >= endMoveFrame_) {
        moveFlag_ = 0;
        return 1;
    }
    moveCounter_++;
    return 0;
}

namespace cmn {
    static dss::Fix32 randomL(0x10a);
}

ARM void cmn::MoveBase::setRandomShake(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int count)
{
    moveType_ = 3;
    dampFrame_ = 0;
    moveCounter_ = 0;
    endMoveFrame_ = 2;
    startPos_ = start;
    dss::Fix32Vector3 vec = (target - start);
    vec.normalize();
    vec *= randomL;
    targetPos_ = start + vec;
    ampFrame_ = count * 6;
    moveVec_ = getShakeVec(dampFrame_);
    moveFlag_ = 1;
    shakeOffset.set(0, 0, 0);
}

ARM void cmn::MoveBase::shakeExecute(dss::Fix32Vector3& pos)
{
    shakeOffset += moveVec_;
    if (moveLock_ == 1) {
        pos += shakeOffset;
    } else {
        pos += moveVec_;
    }
    if (moveCounter_ == endMoveFrame_ && dampFrame_ == ampFrame_) {
        pos -= shakeOffset;
        shakeOffset.set(0, 0, 0);
    }
}

ARM int cmn::MoveBase::updateShake()
{
    if (moveCounter_ >= endMoveFrame_) {
        dampFrame_++;
        if (dampFrame_ > ampFrame_) {
            moveType_ = 0;
            moveFlag_ = 0;
            return 1;
        }
        moveVec_ = getShakeVec(dampFrame_ % 6);
        moveCounter_ = 0;
        return 0;
    }
    moveCounter_++;
    return 0;
}

ARM dss::Fix32Vector3 cmn::MoveBase::getShakeVec(int index)
{
    dss::Fix32Vector3 vec[2];
    vec[0] = (targetPos_ - startPos_);
    vec[0].normalize();
    vec[0] *= randomL;
    vec[1].set(dss::Fix32(0L), randomL, dss::Fix32(0L));
    static const int add0[6] = {1, -2, 1, 1, -2, 1};
    static const int add1[6] = {1, 0, -1, -1, 0, 1};
    return vec[0] * add0[index] + vec[1] * add1[index];
}

ARM void cmn::MoveBase::setJumpMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int frame)
{
    startPos_ = start;
    targetPos_ = target;
    endMoveFrame_ = frame;
    moveCounter_ = 0;
    moveVec_ = (target - start);
    moveVec_.vy += grav * frame * frame / 2;
    moveVec_ /= frame;
    moveType_ = 4;
    moveFlag_ = 1;
}

ARM void cmn::MoveBase::jumpExecute(dss::Fix32Vector3& pos)
{
    pos += moveVec_;
    moveVec_.vy -= grav;
    if (moveCounter_ >= endMoveFrame_) {
        pos = targetPos_;
    }
}

ARM int cmn::MoveBase::updateJump()
{
    if (moveCounter_ >= endMoveFrame_) {
        moveFlag_ = 0;
        return 1;
    }
    moveCounter_++;
    return 0;
}

namespace cmn {
    static dss::Fix32Vector3 moveOffset;
}

ARM void cmn::MoveBase::setAddMove(dss::Fix32Vector3& start, dss::Fix32Vector3& target, int frame)
{
    endMoveFrame_ = frame;
    moveCounter_ = 0;
    moveVec_ = (target - start);
    if (frame != 0) {
        moveVec_ /= frame;
    }
    moveType_ = 5;
    moveFlag_ = 1;
}

ARM void cmn::MoveBase::moveAddExecute(dss::Fix32Vector3& pos)
{
    pos += moveVec_;
}

ARM int cmn::MoveBase::updateMoveAdd()
{
    moveCounter_++;
    if (moveCounter_ < endMoveFrame_) {
        return 0;
    }
    moveFlag_ = 0;
    moveType_ = 0;
    return 1;
}

ARM void cmn::MoveBase::dirMoveExec(dss::Fix32Vector3& pos)
{
    pos += moveVec_;
    dss::Fix32 dist;
    dss::Fix32 speed;
    if (targetPos_.vx != dss::Fix32(0L)) {
        dist = targetPos_.vx - pos.vx;
        speed = moveVec_.vx;
    } else {
        dist = targetPos_.vy - pos.vy;
        speed = moveVec_.vy;
    }
    if (unkfunc_02031e84(dist.value) < unkfunc_02031e84(speed.value)) {
        moveCounter_ = 0;
    }
}

ARM int unkfunc_02031e84(int value)
{
    if (value < 0) {
        value = -value;
    }
    return value;
}

ARM int cmn::MoveBase::updateDirMove()
{
    if (moveCounter_ != 0) {
        return 0;
    }
    moveFlag_ = 0;
    moveType_ = 0;
    return 1;
}

ARM void cmn::MoveBase::setDirMove(dss::Fix32 value, int dir, dss::Fix32 speed)
{
    moveFlag_ = 0;
    moveType_ = 6;
    moveCounter_ = 1;
    targetPos_.set(0, 0, 0);
    moveVec_.set(0, 0, 0);
    switch (dir) {
        case 0:
            moveVec_.vy = speed;
            targetPos_.vy = value;
            break;
        case 1:
            moveVec_.vx = speed;
            targetPos_.vx = value;
            break;
        case 2:
            moveVec_.vy = speed * -1;
            targetPos_.vy = value;
            break;
        case 3:
            moveVec_.vx = speed * -1;
            targetPos_.vx = value;
            break;
    }
}
