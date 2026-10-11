#pragma ipa file
#include "ov000/town/TownRopeAction2.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/dss/Camera.hpp"
#include "main/fld/FldStage.hpp"
#include "main/status/StageStatus.hpp"

static const dss::Fix32 ropeFix(0x4cd);
static const dss::Fix32 ropeDownDY(0.35f);
static const dss::Fix32 ropeUpDY(0.9f);
static const dss::Fix32 lengthGetDown(0.35f);
static const dss::Fix32 ropeSearchR(0.4f);
static const dss::Fix32 ropeUpFixY(0xccd);

ARM int TownRopeAction2::setup()
{
    if (g_Stage.idoLink_.data_.link_.encount_ == 1) {
        return -1;
    }
    int ret = -1;
    dss::Fix32Vector3 surfacePos;
    dss::Fix32Vector3 startPos;
    dss::Fix32Vector3 tempDir;
    int surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(6);
    if (surfaceId != -1) {
        surfacePos = TownStageManager::getSingleton()->getHitSurfacePosByType(6);
        surfaceDir_ = TownStageManager::getSingleton()->getHitSurfaceDirByType(6);
        getRopeSide(surfaceId);
        if (g_Stage.ropeLink_ == 1) {
            surfacePos.vy = position_.vy + TownPlayerAction::rageSurfaceR;
            startPos = surfacePos + surfaceDir_ * ropeFix;
        } else {
            surfacePos.vy = position_.vy - TownPlayerAction::rageSurfaceR;
            startPos = surfacePos + surfaceDir_ * ropeFix;
        }
        tempDir = surfaceDir_ * -1;
        TownActionCalculate::getIdxByVec(dirIdx_, tempDir);
        TownPlayerManager::getSingleton()->setEncountLock(1);
        TownPlayerManager::getSingleton()->shadowSet_ = 1;
        g_cmnPartyInfo.startPos_ = startPos;
        moveMode_ = ROPE_MOVE;
        ret = ACTION_TYPE_ROPE;
    }
    return ret;
}

ARM void TownRopeAction2::execute()
{
    switch (moveMode_) {
    case ROPE_MOVE:
        ropeMove();
        break;
    case DOWN_SIDE_GET_ON:
    case DOWN_SIDE_GET_OFF:
    case UP_SIDE_GET_ON:
    case UP_SIDE_GET_OFF:
        gMoveToTarget.execute();
        break;
    }
}

ARM int TownRopeAction2::update()
{
    dss::Fix32Vector3 nowPos;
    dss::Fix32Vector3 nextPos;
    dss::Fix32 height;
    int ret = -1;
    switch (moveMode_) {
    case ROPE_MOVE:
        nowPos = position_;
        ropeMoveUpdate();
        nextPos = position_;
        TownStageManager::getSingleton()->compute(nowPos, nextPos, TownPlayerAction::collR, TownPlayerAction::surfaceR, TownPlayerAction::collR * 2, height);
        if (TownStageManager::getSingleton()->getHitSurfaceIdByType(5) != -1 && g_Stage.idoLink_.data_.link_.inFlag_ == 1) {
            g_Stage.idoLink_.data_.link_.outFlag_ = 1;
        } else if (TownStageManager::getSingleton()->getHitSurfaceIdByType(1) != 0) {
            if (position_.vy > (minY_ + maxY_) / 2) {
                g_Stage.ropeLink_ = 1;
            } else {
                g_Stage.ropeLink_ = 2;
            }
        }
        break;
    case DOWN_SIDE_GET_ON:
    case UP_SIDE_GET_ON:
        if (gMoveToTarget.update() != -1) {
            moveMode_ = ROPE_MOVE;
            TownPlayerManager::getSingleton()->setRemote(0);
            TownPlayerManager::getSingleton()->setEncountLock(1);
        }
        break;
    case DOWN_SIDE_GET_OFF:
    case UP_SIDE_GET_OFF:
        if (gMoveToTarget.update() != -1) {
            TownPlayerManager::getSingleton()->setRemote(0);
            TownPlayerManager::getSingleton()->setEncountLock(0);
            g_cmnPartyInfo.prev_position_ = g_cmnPartyInfo.position_;
            ret = ACTION_TYPE_WALK;
        }
        break;
    }
    return ret;
}

ARM int TownRopeAction2::startCheck()
{
    int ret = -1;
    static const dss::Fix32 cosRope(0.75f);
    dss::Fix32Vector3 playerDir;
    dss::Fix32Vector3 surfacePos;
    dss::Fix32Vector3 surfaceDir;
    dss::Fix32Vector3 targetPos;
    dss::Fix32Vector3 vec;
    dss::Fix32 minY;
    dss::Fix32 maxY;
    int surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(6);
    if (surfaceId != -1) {
        TownActionCalculate::getDirByIdx(dirIdx_, playerDir);
        playerDir.normalize();
        surfacePos = TownStageManager::getSingleton()->getHitSurfacePosByType(6);
        surfaceDir = TownStageManager::getSingleton()->getHitSurfaceDirByType(6);
        vec = surfacePos - position_;
        vec.vy = 0L;
        vec.normalize();
        getRopeSide(surfaceId);
        dss::Fix32 center = (minY_ + maxY_) / 2;
        dss::Fix32Vector3 dir = surfaceDir * -1;
        if (playerDir * (surfaceDir * -1) > cosRope) {
            if (playerDir * vec > cosRope) {
                if (center > position_.vy) {
                    surfacePos.vy = position_.vy;
                    targetPos = surfacePos + surfaceDir * ropeFix;
                    gMoveToTarget.setAction(position_, targetPos, TownPlayerAction::walkSpeed, 0, 0, 1);
                    TownActionCalculate::getIdxByVec(dirIdx_, dir);
                    moveMode_ = UP_SIDE_GET_ON;
                } else {
                    surfacePos.vy = position_.vy;
                    targetPos = surfacePos + surfaceDir * ropeFix * -1;
                    targetPos.vy -= TownPlayerAction::walkSpeed * 8;
                    gMoveToTarget.setAction(position_, targetPos, TownPlayerAction::walkSpeed, 0, 0, 1);
                    TownActionCalculate::getIdxByVec(dirIdx_, surfaceDir);
                    moveMode_ = DOWN_SIDE_GET_ON;
                }
                TownPlayerManager::getSingleton()->setRemote(1);
                ret = ACTION_TYPE_ROPE;
                TownPlayerManager::getSingleton()->shadowSet_ = 1;
            }
        }
    }
    return ret;
}

ARM TownRopeAction2* TownRopeAction2::getSingleton()
{
    static TownRopeAction2 townRopeAction2;
    return &townRopeAction2;
}

ARM void TownRopeAction2::getRopeSide(int surfaceId)
{
    int polyNo = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[6];
    COLL_POLY coll;
    if (!TownStageManager::getSingleton()->stage_.collGetPoly(polyNo, &coll)) {
        return;
    }
    minY_.value = coll.vertex[0].y;
    maxY_.value = coll.vertex[0].y;
    for (int i = 1; i < 4; i++) {
        if (coll.vertex[i].y < minY_.value) {
            minY_.value = coll.vertex[i].y;
        }
        if (coll.vertex[i].y > maxY_.value) {
            maxY_.value = coll.vertex[i].y;
        }
    }
}

ARM void TownRopeAction2::ropeMove()
{
    int padInput = TownPlayerManager::getSingleton()->player_.padInput_;
    int dirInput = TownPlayerManager::getSingleton()->player_.dirInput_;
    if (padInput == 0) {
        return;
    }
    switch (dirInput) {
    case 0:
        position_.vy += TownPlayerAction::ropeSpeed;
        break;
    case 0x8000:
        position_.vy -= TownPlayerAction::ropeSpeed;
        break;
    }
}

ARM void TownRopeAction2::ropeMoveUpdate()
{
    dss::Fix32Vector3 playerDir;
    dss::Fix32Vector3 target;
    dss::Fix32Vector3 pos;
    dss::Fix32 height;
    TownActionCalculate::getDirByIdx(dirIdx_, playerDir);
    int padInput = TownPlayerManager::getSingleton()->player_.padInput_;
    int dirInput = TownPlayerManager::getSingleton()->player_.dirInput_;
    if (padInput == 0) {
        return;
    }
    if (position_.vy <= minY_ + ropeDownDY && dirInput == 0x8000) {
        target = position_ + playerDir * ropeFix * -1;
        gMoveToTarget.setAction(position_, target, TownPlayerAction::walkSpeed, 0, 0, 0);
        moveMode_ = DOWN_SIDE_GET_OFF;
        TownPlayerManager::getSingleton()->shadowSet_ = 0;
        TownPlayerManager::getSingleton()->setRemote(1);
        return;
    }
    if (position_.vy >= (maxY_ + minY_) / 2 && dirInput == 0) {
        target = position_ + playerDir;
        target.vy += TownPlayerAction::collR;
        TownStageManager::getSingleton()->compute(target, target, ropeSearchR, ropeSearchR, ropeSearchR * 2, height);
        target.vy += height - TownPlayerAction::collR;
        if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == -1) {
            return;
        }
        pos = TownStageManager::getSingleton()->getHitSurfacePosByType(0);
        if (position_.vy > pos.vy && pos.vy > (maxY_ + minY_) / 2) {
            gMoveToTarget.setAction(position_, target, TownPlayerAction::walkSpeed, 0, 0, 0);
            moveMode_ = UP_SIDE_GET_OFF;
            TownPlayerManager::getSingleton()->shadowSet_ = 0;
            TownPlayerManager::getSingleton()->setRemote(1);
        }
    }
}
