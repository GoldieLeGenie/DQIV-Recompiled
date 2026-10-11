#pragma ipa file
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/status/StageStatus.hpp"
#include "main/sound/Sound.hpp"

const dss::Fix32 TownFallAction::fallStartFix(0x2000);
const dss::Fix32 TownFallAction::fallSpeed(0x225);

ARM int TownFallAction::setup()
{
    int ret = -1;
    dss::Fix32 height;
    dss::Fix32Vector3 pos;
    pos = position_;
    partyMove_ = 0;
    sePlay_ = 0;
    if (g_Stage.getFallFlag() == 1) {
        cameraPos_ = position_;
        cameraPos_ = TownStageManager::getSingleton()->compute(cameraPos_, cameraPos_, TownPlayerAction::collR, TownPlayerAction::collR, TownPlayerAction::townCharaPreR, height);
        cameraPos_.vy += height - TownPlayerAction::fixR;
        TownCamera::getSingleton()->camera_.unk_004.setTarget(cameraPos_);
        pos.vy += fallStartFix;
        gMoveToTarget.setAction(pos, cameraPos_, fallSpeed, 1, 0, 0);
        TownCamera::getSingleton()->setCameraLock(true);
        TownPlayerManager::getSingleton()->setRemote(1);
        TownPlayerManager::getSingleton()->setEncountLock(1);
        TownPlayerManager::getSingleton()->shadowSet_ = 1;
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(0);
        sePlay_ = 1;
        fallType_ = FALL_START;
        ret = ACTION_TYPE_FALL;
    } else {
        pos.vy += TownPlayerAction::collR;
        TownStageManager::getSingleton()->compute(pos, pos, TownPlayerAction::collR, TownPlayerAction::changePreR, TownPlayerAction::changePreR, height);
        pos.vy += height - TownPlayerAction::collR;
    }
    int surface = TownStageManager::getSingleton()->getHitSurfaceIdByType(5);
    TownActionWalk::getSingleton()->idoSurfaceid_ = surface;
    g_cmnPartyInfo.startPos_ = pos;
    g_Stage.setFallFlag(0);
    count_ = 0;
    return ret;
}

ARM void TownFallAction::execute()
{
    moveMode_ = 1;
    dss::Fix32 height;
    if (fallType_ == FALL_START) {
        if (count_ > 8) {
            gMoveToTarget.execute();
        }
        count_++;
        return;
    }
    count_++;
    if (count_ == 1) {
        setFixXZ();
    }
    if (count_ < 4) {
        position_ += vecXZ_;
        return;
    }
    if (count_ == 4) {
        if (sePlay_ == 1) {
            Sound::sePlayDirect(0x138);
        }
        return;
    }
    dss::Fix32Vector3 pos;
    pos = position_;
    pos.vy += TownPlayerAction::fixR;
    TownStageManager::getSingleton()->compute(pos, pos, TownPlayerAction::collR, TownPlayerAction::collR, TownPlayerAction::collR * 2, height);
    int surface = TownStageManager::getSingleton()->getHitSurfaceIdByType(0);
    if (surface == -1 || height < fallSpeed * -1) {
        position_.vy -= fallSpeed;
        return;
    }
    position_.vy += height;
    if (partyMove_ == 0) {
        TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
        partyMove_ = 1;
        return;
    }
    if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ != 0) {
        return;
    }
    moveMode_ = 0;
    if (surface != -1) {
        g_Stage.setFallFlag(0);
    }
}

ARM int TownFallAction::update()
{
    int ret = -1;
    if (fallType_ == FALL_START) {
        if (gMoveToTarget.update() != -1) {
            TownCamera::getSingleton()->setCameraLock(false);
            g_Stage.setFallFlag(0);
            ret = ACTION_TYPE_WALK;
            TownPlayerManager::getSingleton()->setRemote(0);
            TownPlayerManager::getSingleton()->shadowSet_ = 0;
            TownPlayerManager::getSingleton()->setEncountLock(0);
            TownPlayerManager::getSingleton()->mapChangeSE_ = 1;
            TownPlayerManager::getSingleton()->partyDraw_.setAnimation(1);
        }
    } else if (moveMode_ == 0) {
        TownCamera::getSingleton()->setCameraLock(false);
        TownPlayerManager::getSingleton()->setRemote(0);
        TownPlayerManager::getSingleton()->shadowSet_ = 0;
        TownPlayerManager::getSingleton()->setEncountLock(0);
        TownPlayerManager::getSingleton()->mapChangeSE_ = 1;
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(1);
        ret = ACTION_TYPE_WALK;
    }
    if (TownStageManager::getSingleton()->getExitIndex() != -1) {
        g_Stage.setFallFlag(1);
    }
    return ret;
}

ARM int TownFallAction::startCheck()
{
    int ret = -1;
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0xb) != -1 && TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == -1) {
        setCollFall();
        ret = ACTION_TYPE_FALL;
    }
    return ret;
}

ARM TownFallAction* TownFallAction::getSingleton()
{
    static TownFallAction townFallAction;
    return &townFallAction;
}

ARM void TownFallAction::setCollFall()
{
    partyMove_ = 0;
    fallType_ = FALL_COLL;
    g_Stage.setFallFlag(1);
    TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
    TownPlayerManager::getSingleton()->setEncountLock(1);
    TownPlayerManager::getSingleton()->shadowSet_ = 1;
    TownPlayerManager::getSingleton()->setRemote(1);
    TownPlayerManager::getSingleton()->mapChangeSE_ = 0;
    TownPlayerManager::getSingleton()->partyDraw_.setAnimation(0);
    count_ = 0;
    sePlay_ = 1;
}

ARM void TownFallAction::setFixXZ()
{
    static const dss::Fix32 searchLen(0xccd);
    static const dss::Fix32 sideLen(0x59a);
    dss::Fix32Vector3 dir;
    dss::Fix32Vector3 side;
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 out;
    TownActionCalculate::getDirByIdx(dirIdx_, dir);
    TownActionCalculate::getDirByIdx(dirIdx_ + 0x4000, side);
    position_ += dir * TownPlayerAction::walkSpeed;
    pos = position_ + (dir * TownPlayerAction::walkSpeed) * 6;
    int poly;
    if (TownStageManager::getSingleton()->checkCrossNumEraseSurface(position_, pos, 0x1000, 1, poly)) {
        vecXZ_.set(0, 0, 0);
    } else {
        vecXZ_ = dir * TownPlayerAction::walkSpeed / 2;
    }
    pos = position_ + side * sideLen;
    pos.vy += TownPlayerAction::collR;
    int surface = TownStageManager::getSingleton()->getHitSurfaceIdByType(0);
    TownStageManager::getSingleton()->coll_.m_surfaceType[0] = -1;
    TownStageManager::getSingleton()->searchFloorSurface(pos, TownPlayerAction::collR, searchLen, out);
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == 0) {
        vecXZ_ -= side * TownPlayerAction::walkSpeed;
    }
    TownStageManager::getSingleton()->coll_.m_surfaceType[0] = -1;
    TownStageManager::getSingleton()->coll_.m_surfaceType[0] = -1;
    pos = position_ - side * sideLen;
    pos.vy += TownPlayerAction::collR;
    TownStageManager::getSingleton()->searchFloorSurface(pos, TownPlayerAction::collR, searchLen, out);
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == 0) {
        vecXZ_ += side * TownPlayerAction::walkSpeed;
    }
    TownStageManager::getSingleton()->coll_.m_surfaceType[0] = surface;
}

ARM void TownFallAction::exitFall()
{
    TownCamera::getSingleton()->setCameraLock(true);
    position_.vy -= fallSpeed;
}
