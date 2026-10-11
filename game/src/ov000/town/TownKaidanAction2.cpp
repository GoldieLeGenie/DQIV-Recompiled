#pragma ipa file
#include "ov000/town/TownKaidanAction2.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "main/cmn/CommonCalculate.hpp"
#include "main/dss/Camera.hpp"
#include "main/fld/FldStage.hpp"
#include "main/sound/Sound.hpp"

static const dss::Fix32 kaidanF(0x4cd);
static const dss::Fix32 kaidanUp(0x666);
static const dss::Fix32 kaidanSideFix(0x19a);
static const dss::Fix32 startSearchLen(0x1333);

ARM int TownKaidanAction2::setup()
{
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 pos1;
    dss::Fix32Vector3 pos2;
    dss::Fix32 length;
    pos1 = position_;
    pos2 = position_;
    pos2.vz += startSearchLen;
    upKaidan_.objectId = -1;
    downKaidan_.objectId = -1;
    firstDown_ = 0;
    downChange_ = 0;
    kaidanBackWall_ = 0;
    side1Wall_ = 0;
    side2Wall_ = 0;
    downKaidanFixY_ = 0L;
    int polyNo = TownStageManager::getSingleton()->stage_.collCrossCheckPoly(pos1, pos2, &length, 0);
    int objectNo = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, polyNo);
    int commonId = TownStageManager::getSingleton()->getMapObjCommonId(objectNo);
    if (commonId == 0x8a || commonId == 0x8b) {
        int id = StageLink::getTownExitIndex();
        if (id == -1) {
            return -1;
        }
        id = coll_GetPolyNoBySurface(TownStageManager::getSingleton()->stage_.m_fld.m_coll, id, 0);
        if (id == -1) {
            return -1;
        }
        coll_EraseObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, objectNo);
        setKaidanArea(id);
        TownStageManager::getSingleton()->getObjectPos(objectNo, polyNo, &pos);
        setKaidanByObject(downKaidan_, objectNo, pos);
        dss::Fix32Vector3 target = pos - downKaidan_.normal * startSearchLen;
        if (TownStageManager::getSingleton()->checkCrossNum(pos, target, 1) >= 1) {
            kaidanBackWall_ = 1;
        }
        side1Wall_ = setSideFix(downKaidan_.pos1, downKaidan_.pos2, pos, side1WallLine_, side1WallNormal_);
        side2Wall_ = setSideFix(downKaidan_.pos2, downKaidan_.pos1, pos, side2WallLine_, side2WallNormal_);
        dirIdx_ = TownStageManager::getSingleton()->stage_.getObjectRotIdxY(objectNo) - 0x4003;
        firstDown_ = 1;
        downChange_ = 1;
    }
    return -1;
}

ARM void TownKaidanAction2::execute()
{
    if (moveType_ != KAIDAN_MOVE_STOP) {
        gMoveToTarget.execute();
    }
}

ARM int TownKaidanAction2::update()
{
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 centerPos;
    dss::Fix32Vector3 target;
    dss::Fix32Vector3 targetPos;
    dss::Fix32Vector3 mVec;
    dss::Fix32 length;
    dss::Fix32 dot;
    dss::Fix32 speedToTarget;
    static const dss::Fix32 rad(0x99a);
    static const dss::Fix32 col(0x8cd);
    dss::Fix32 h;
    speedToTarget = TownPlayerAction::walkSpeed;
    int ret = gMoveToTarget.update();
    if (ret != -1) {
        moveType_ = (KAIDAN_MOVE_TYPE)ret;
        vec = position_ - upKaidan_.center;
        vec.vy = 0L;
        vec.normalize();
        checkKaidanSide(mVec, target, length);
        dot = vec * upKaidan_.normal;
        switch (moveType_) {
        case KAIDAN_MOVE_BACK:
            moveType_ = KAIDAN_MOVE_SIDE;
            targetPos = target + mVec * kaidanF;
            break;
        case KAIDAN_MOVE_SIDE:
            moveType_ = KAIDAN_MOVE_FRONT;
            targetPos = target + (mVec * kaidanF * -3 / 2);
            break;
        case KAIDAN_MOVE_FRONT:
            moveType_ = KAIDAN_MOVE_UP;
            targetPos = upKaidan_.center;
            targetPos.vy += kaidanUp;
            speedToTarget = TownPlayerAction::walkSpeed / 2;
            break;
        case KAIDAN_MOVE_UP:
            Sound::sePlayDirect(0x131);
            TownPlayerManager::getSingleton()->mapChangeSE_ = 0;
            TownPlayerManager::getSingleton()->resetMapLink(RESET_EXIT_LOCK_KAIDAN);
            TownStageManager::getSingleton()->compute(position_, position_, col, col, rad, h);
            if (TownStageManager::getSingleton()->getHitSurfaceIdByType(1) != -1 || TownStageManager::getSingleton()->getHitSurfaceIdByType(7) != -1) {
                moveType_ = KAIDAN_MOVE_STOP;
            } else {
                dss::Fix32Vector3 dir;
                TownActionCalculate::getDirByIdx(dirIdx_, dir);
                targetPos = position_ + dir;
                moveType_ = KAIDAN_MOVE_NOT_EXIT;
            }
            break;
        case KAIDAN_MOVE_NOT_EXIT:
            TownPlayerManager::getSingleton()->setRemote(0);
            return ACTION_TYPE_WALK;
        }
        if (moveType_ != KAIDAN_MOVE_STOP) {
            gMoveToTarget.setAction(position_, targetPos, speedToTarget, 0, 0, moveType_);
        }
    }
    return -1;
}

ARM int TownKaidanAction2::startCheck()
{
    int ret = -1;
    upKaidan_.objectId = -1;
    checkObject();
    checkSurface();
    if (upKaidan_.objectId != -1) {
        if (upKaidan_.objectId != downKaidan_.objectId) {
            dss::Fix32Vector3 vec;
            TownActionCalculate::getDirByIdx(dirIdx_, vec);
            int padInput = TownPlayerManager::getSingleton()->player_.padInput_;
            int tmp = 0;
            if ((upKaidan_.center - position_) * vec > dss::Fix32(0L)) {
                if (padInput) {
                    tmp = 1;
                }
            }
            if (tmp) {
                TownPlayerManager::getSingleton()->lockMapLink(EXIT_LOCK_KAIDAN);
                checkKaidanMoveStart();
                TownPlayerManager::getSingleton()->setRemote(1);
                ret = ACTION_TYPE_KAIDAN;
            }
        } else {
            TownStageManager::getSingleton()->collResetObject(downKaidan_.objectId);
        }
    } else if (firstDown_ == 1) {
        if (!cmn::CommonCalculate::simpleAreaInCheck(kaidanArea_[0], kaidanArea_[1], position_)) {
            TownStageManager::getSingleton()->collResetObject(downKaidan_.objectId);
            downKaidan_.objectId = -1;
            dss::Fix32Vector3 pos(position_);
            pos.vy = kaidanMaxH_;
            pos.vy.value -= 0x1f4;
            TownStageManager::getSingleton()->computeCollFloor(pos, TownPlayerAction::collR, position_);
            position_.vy -= TownPlayerAction::collR;
            firstDown_ = 0;
        }
    }
    return ret;
}

ARM TownKaidanAction2* TownKaidanAction2::getSingleton()
{
    static TownKaidanAction2 townKaidanAction2;
    return &townKaidanAction2;
}

ARM void TownKaidanAction2::checkObject()
{
    int polyNo = TownStageManager::getSingleton()->coll_.m_id;
    int objectNo = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, polyNo);
    if (objectNo == -1) {
        return;
    }
    int commonId = TownStageManager::getSingleton()->getMapObjCommonId(objectNo);
    if (commonId == 0x8a || commonId == 0x8b) {
        dss::Fix32Vector3 pos;
        TownStageManager::getSingleton()->getObjectPos(objectNo, polyNo, &pos);
        setKaidanByObject(upKaidan_, objectNo, pos);
        upKaidan_.normal.vy = 0L;
        upKaidan_.normal.normalize();
        upKaidan_.pos1.vy = position_.vy;
        upKaidan_.pos2.vy = position_.vy;
    }
}

ARM void TownKaidanAction2::checkSurface()
{
    exitBeforeUpKaidan_ = 0;
    TownPlayerManager::getSingleton()->resetMapLink(RESET_EXIT_LOCK_KAIDAN);
    dss::Fix32Vector3 dir;
    static const dss::Fix32Vector3 upN(0.0f, 1.0f, 0.0f);
    static const dss::Fix32 dotN1(0.85f);
    static const dss::Fix32 dotN2(0.25f);
    dss::Fix32 dot;
    int surfaceId;
    int polyNo;
    surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(1);
    if (surfaceId != -1) {
        polyNo = TownStageManager::getSingleton()->getHitSurfaceIdByType(1);
        dir = TownStageManager::getSingleton()->getHitSurfaceDirByType(1);
    } else {
        surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(7);
        if (surfaceId != -1) {
            polyNo = TownStageManager::getSingleton()->getHitSurfaceIdByType(7);
            dir = TownStageManager::getSingleton()->getHitSurfaceDirByType(7);
        }
    }
    if (surfaceId != -1) {
        dir.normalize();
        dot = dir * upN;
        if (dot > dotN2 && dot < dotN1) {
            exitBeforeUpKaidan_ = 1;
            TownPlayerManager::getSingleton()->lockMapLink(EXIT_LOCK_KAIDAN);
        }
    } else {
        downChange_ = 0;
    }
}

ARM void TownKaidanAction2::setKaidanByObject(TownKaidan& kaidan, int id, dss::Fix32Vector3& pos)
{
    int polyNo = TownStageManager::getSingleton()->stage_.getObjWallPolyNo(id, 2);
    kaidan.objectId = id;
    COLL_POLY coll;
    TownStageManager::getSingleton()->stage_.collGetPoly(polyNo, &coll);
    kaidan.center = pos;
    kaidan.pos1 = FldStage::getFx32Vector3(coll.vertex[0]);
    kaidan.pos2 = FldStage::getFx32Vector3(coll.vertex[2]);
    kaidan.normal = FldStage::getFx32Vector3(coll.normal);
    TownPlayerManager::getSingleton()->lockMapLink(EXIT_LOCK_KAIDAN);
}

ARM void TownKaidanAction2::checkKaidanMoveStart()
{
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 target;
    dss::Fix32Vector3 mVec;
    dss::Fix32Vector3 targetPos;
    dss::Fix32 length;
    dss::Fix32 speedToTarget;
    dss::Fix32 dot;
    speedToTarget = TownPlayerAction::walkSpeed;
    vec = position_ - upKaidan_.center;
    vec.vy = 0L;
    vec.normalize();
    dot = vec * upKaidan_.normal;
    if (dot.value <= -0xb50) {
        moveType_ = KAIDAN_MOVE_BACK;
        checkKaidanSide(mVec, target, length);
        targetPos = position_ + mVec * length;
    } else if (dot.value < 0x800) {
        moveType_ = KAIDAN_MOVE_SIDE;
        checkKaidanSide(mVec, target, length);
        targetPos = target + mVec * kaidanF;
    } else {
        moveType_ = KAIDAN_MOVE_UP;
        checkKaidanSide(mVec, target, length);
        targetPos = upKaidan_.center;
        targetPos.vy += kaidanUp;
        speedToTarget = TownPlayerAction::walkSpeed / 2;
    }
    gMoveToTarget.setAction(position_, targetPos, speedToTarget, 0, 0, moveType_);
}

ARM void TownKaidanAction2::checkKaidanSide(dss::Fix32Vector3& mVec, dss::Fix32Vector3& target, dss::Fix32& length)
{
    dss::Fix32Vector3 vec0;
    dss::Fix32Vector3 vec1;
    dss::Fix32Vector3 vec2;
    dss::Fix32Vector3 checkPos;
    static const dss::Fix32 fixL(0x1333);
    vec1 = position_ - upKaidan_.pos1;
    vec1.vy = 0L;
    vec2 = position_ - upKaidan_.pos2;
    vec2.vy = 0L;
    vec0 = upKaidan_.pos1 - upKaidan_.pos2;
    vec0 *= fixL;
    checkPos = upKaidan_.center + vec0;
    int num1 = TownStageManager::getSingleton()->checkCrossNum(upKaidan_.center, checkPos, 0);
    checkPos = upKaidan_.center - vec0;
    int num2 = TownStageManager::getSingleton()->checkCrossNum(upKaidan_.center, checkPos, 0);
    if (moveType_ == KAIDAN_MOVE_BACK) {
        if (num1 > 1 || (vec1.lengthsq() > vec2.lengthsq() && num2 < 2)) {
            mVec = upKaidan_.pos2 - upKaidan_.pos1;
            mVec.vy = 0L;
            mVec.normalize();
            length = mVec * (position_ - upKaidan_.pos2);
            target = upKaidan_.pos2;
        } else {
            mVec = upKaidan_.pos1 - upKaidan_.pos2;
            mVec.vy = 0L;
            mVec.normalize();
            length = mVec * (position_ - upKaidan_.pos1);
            target = upKaidan_.pos1;
        }
    } else {
        if (vec1.lengthsq() > vec2.lengthsq()) {
            mVec = upKaidan_.pos2 - upKaidan_.pos1;
            mVec.vy = 0L;
            mVec.normalize();
            length = mVec * (position_ - upKaidan_.pos2);
            target = upKaidan_.pos2;
        } else {
            mVec = upKaidan_.pos1 - upKaidan_.pos2;
            mVec.vy = 0L;
            mVec.normalize();
            length = mVec * (position_ - upKaidan_.pos1);
            target = upKaidan_.pos1;
        }
    }
    length.value = unkfunc_02031e84(length.value) + kaidanF.value;
}

ARM void TownKaidanAction2::setPlayerFixPosition(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos)
{
    if (firstDown_ != 1) {
        return;
    }
    downKaidanFixY_ = newPos.vy - kaidanMaxH_;
    if (side1Wall_ == 1) {
        if (TownActionCalculate::checkLineOver(newPos, side1WallLine_, side1WallNormal_) == 1) {
            dss::Fix32Vector3 vec = newPos - oldPos;
            dss::Fix32 val = side1WallNormal_ * vec;
            newPos -= side1WallNormal_ * val;
        }
    }
    if (side2Wall_ == 1) {
        if (TownActionCalculate::checkLineOver(newPos, side2WallLine_, side2WallNormal_) == 1) {
            dss::Fix32Vector3 vec = newPos - oldPos;
            dss::Fix32 val = side2WallNormal_ * vec;
            newPos -= side2WallNormal_ * val;
        }
    }
    if (downKaidanFixY_ >= dss::Fix32(0L)) {
        if (kaidanBackWall_ == 1) {
            newPos = oldPos;
        }
        newPos.vy = kaidanMaxH_;
    }
}

ARM bool TownKaidanAction2::setSideFix(dss::Fix32Vector3& pos1, dss::Fix32Vector3& pos2, dss::Fix32Vector3& center, dss::Fix32Vector3& sideLine, dss::Fix32Vector3& sideNormal)
{
    bool sideWall = false;
    dss::Fix32Vector3 normal = pos1 - pos2;
    normal.vy = 0L;
    normal.normalize();
    dss::Fix32Vector3 target = center + normal * startSearchLen;
    if (TownStageManager::getSingleton()->checkCrossNum(center, target, 1) >= 1) {
        sideWall = true;
        sideLine = center + normal * kaidanSideFix;
        sideNormal = normal;
    }
    return sideWall;
}

ARM void TownKaidanAction2::setKaidanArea(int id)
{
    COLL_POLY coll;
    TownStageManager::getSingleton()->stage_.collGetPoly(id, &coll);
    kaidanArea_[0] = FldStage::getFx32Vector3(coll.bbox[0]);
    kaidanArea_[1] = FldStage::getFx32Vector3(coll.bbox[1]);
    kaidanMaxH_.value = kaidanArea_[1].vy.value - 0x3e8;
}

ARM bool TownKaidanAction2::isSaveOK()
{
    return firstDown_ == 0;
}
