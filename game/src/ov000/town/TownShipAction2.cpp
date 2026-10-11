#pragma ipa file
#include "ov000/town/TownShipAction2.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/fld/FldStage.hpp"
#include "main/global/Global.hpp"
#include "main/status/BaseActionStatus.hpp"

static const dss::Fix32 unusedR(0.4f);

ARM int TownShipAction2::setup()
{
    int ret = -1;
    ctrSurfacePoly_ = -1;
    ctrSurfaceId_ = -1;
    shipObjectId_ = -1;
    namiAlpha_ = 0;
    shipObjectId_ = TownStageManager::getSingleton()->getObjectIDfromMapUid(0x1f4);
    shipNamiObjectId_ = TownStageManager::getSingleton()->getObjectIDfromMapUid(0x1f3);
    shipDirection_ = prevShipDirection_ = 0;
    moveMode_ = GET_ON_SHIP;
    if (shipObjectId_ != -1) {
        if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
            position_.vy = shipPosition_.vy;
            prevShipDirection_ = dirIdx_;
            setShipPosition(position_);
            shipDirection_ = dirIdx_;
            TownStageManager::getSingleton()->setAlpha(shipNamiObjectId_, 0x1f);
            moveMode_ = SHIP_MOVE;
            ret = ACTION_TYPE_SHIP;
        } else {
            if (*g_cmnPartyInfo.getShipMapName() == 0) {
                shipPosition_ = TownStageManager::getSingleton()->getMapUidPos(0x1f4);
                shipNamiPosition_ = TownStageManager::getSingleton()->getMapUidPos(0x1f3);
                g_cmnPartyInfo.setShipInfo(g_Global.getMapName(), &shipPosition_, shipDirection_);
            }
            if (dss::strcmp(g_cmnPartyInfo.getShipMapName(), g_Global.getMapName()) == 0) {
                g_cmnPartyInfo.getShipInfo(&shipPosition_, &shipDirection_);
                TownStageManager::getSingleton()->rotObjectUid(0x1f4, shipDirection_);
                setShipPosition(shipPosition_);
            } else {
                TownStageManager::getSingleton()->eraseObject(0x1f4, 1);
                TownStageManager::getSingleton()->eraseObject(0x1f3, 1);
                shipObjectId_ = -1;
            }
            TownStageManager::getSingleton()->setAlpha(shipNamiObjectId_, 0);
        }
        prevShipPosition_ = shipPosition_;
        prevShipDirection_ = shipDirection_;
    }
    return ret;
}

ARM void TownShipAction2::execute()
{
    switch (moveMode_) {
    case SHIP_MOVE:
        shipMove();
        break;
    case SHIP_MOVE_TO:
        gMoveToTarget.execute();
        setShipPosition(position_);
        setDirection(dirIdx_);
        break;
    case SHIP_DOCKED_AT:
        gMoveToTarget.execute();
        setShipPosition(position_);
        setDirection(dockedDirIdx_);
        break;
    case GET_ON_SHIP:
    case GET_OFF_SHIP:
        gMoveToTarget.execute();
        setShipNamiAlpha();
        break;
    }
}

/* not original: without it the idx2 arithmetic is sunk past the first abs() call */
#pragma push
#pragma opt_common_subs off
ARM int TownShipAction2::update()
{
    int ret = -1;
    static dss::Fix32 fixOne;
    fixOne.value = 0x1000;
    switch (moveMode_) {
    case SHIP_MOVE:
        if (ctrSurfaceId_ == -1) {
            ctrSurfaceId_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(0xc);
            ctrSurfacePoly_ = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[0xc];
        }
        if (ctrSurfaceId_ != -1) {
            dss::Fix32Vector3 pos;
            dss::Fix32Vector3 vec;
            TownActionCalculate::getDirByIdx(dirIdx_, vec);
            pos = position_ + vec;
            if (TownActionCalculate::checkIkadaTalk(pos, dirIdx_, ctrSurfaceId_, ctrSurfacePoly_, 0) == true) {
                break;
            }
        }
        shipVec_ = shipPosition_ - prevShipPosition_;
        {
            dss::Fix32Vector3 surfaceDir;
            dss::Fix32Vector3 surfacePos;
            if (TownActionCalculate::checkGetDownShipAndIkada(position_, dirIdx_, targetPos_, surfaceDir, surfacePos, TownPlayerAction::getDownL) == true) {
                dss::Fix32Vector3 vec = shipPosition_ - surfacePos;
                vec.vy = 0L;
                dockedLen_ = surfaceDir * vec - fixOne;
                dockedatPos_ = position_ - surfaceDir * dockedLen_;
                short idx = 0;
                TownActionCalculate::getIdxByVec(idx, surfaceDir);
                short idx1 = idx + 0x4000 - shipDirection_;
                short idx2 = idx - 0x4000 - shipDirection_;
                short tIdx;
                if (status::BaseActionStatus::abs(idx1) < status::BaseActionStatus::abs(idx2)) {
                    tIdx = idx + 0x4000;
                } else {
                    tIdx = idx - 0x4000;
                }
                dockedDirIdx_ = tIdx;
                gMoveToTarget.setAction(shipPosition_, dockedatPos_, TownPlayerAction::getOnOffSpeed / 3, 0, 2, ACTION_TYPE_SHIP);
                TownPlayerManager::getSingleton()->setRemote(1);
                moveMode_ = SHIP_DOCKED_AT;
            }
            ret = TownDoorAction::getSingleton()->startCheck();
        }
        break;
    case SHIP_DOCKED_AT:
        if (gMoveToTarget.update() == -1) {
            break;
        }
        if (status::BaseActionStatus::abs((short)(dockedDirIdx_ - shipDirection_)) > 200) {
            break;
        }
        shipDirection_ = dockedDirIdx_;
        prevShipPosition_ = shipPosition_;
        prevShipDirection_ = shipDirection_;
        gMoveToTarget.setAction(position_, targetPos_, TownPlayerAction::getOnOffSpeed, 1, 0, ACTION_TYPE_SHIP);
        TownPlayerManager::getSingleton()->setPartyToFirst(position_);
        TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
        moveMode_ = GET_OFF_SHIP;
        break;
    case GET_ON_SHIP:
        if (gMoveToTarget.update() != -1) {
            moveMode_ = SHIP_MOVE_TO;
            gMoveToTarget.setAction(position_, dockedatPos_, TownPlayerAction::getOnOffSpeed, 0, 2, ACTION_TYPE_SHIP);
        }
        break;
    case SHIP_MOVE_TO:
        if (gMoveToTarget.update() != -1) {
            TownPlayerManager::getSingleton()->setRemote(0);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_SHIP_IKADA;
            prevShipPosition_ = shipPosition_;
            shipVec_ = shipPosition_ - prevShipPosition_;
            moveMode_ = SHIP_MOVE;
        }
        break;
    case GET_OFF_SHIP:
        if (gMoveToTarget.update() != -1) {
            TownPlayerManager::getSingleton()->setRemote(0);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
            g_cmnPartyInfo.setShipInfo(g_Global.getMapName(), &shipPosition_, shipDirection_);
            ret = ACTION_TYPE_WALK;
        }
        break;
    }
    return ret;
}

#pragma pop

ARM int TownShipAction2::startCheck()
{
    int ret = -1;
    static const dss::Fix32 unusedL(0x333);
    static const dss::Fix32 searchL(0x159a);
    dss::Fix32Vector3 target;
    dss::Fix32 length;
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0xa) != -1 && shipObjectId_ != -1) {
        length = TownPlayerAction::shipR + TownPlayerAction::getDownL * 2;
        if (TownActionCalculate::checkGetOnShipAndIkada(position_, shipPosition_, dirIdx_, length)) {
            target = shipPosition_;
            gMoveToTarget.setAction(position_, target, TownPlayerAction::getOnOffSpeed, 1, 2, ACTION_TYPE_SHIP);
            moveMode_ = GET_ON_SHIP;
            TownPlayerManager::getSingleton()->setRemote(1);
            dockedatPos_ = shipPosition_;
            {
                dss::Fix32Vector3 vec;
                TownActionCalculate::getDirByIdx(shipDirection_ + 0x4000, vec);
                dss::Fix32Vector3 pos = shipPosition_ + vec * searchL;
                dss::Fix32 retLen;
                if (TownStageManager::getSingleton()->stage_.collCrossCheckPoly(shipPosition_, pos, &retLen, 1) > 0) {
                    dockedLen_ = retLen;
                    dockedatPos_ = shipPosition_ - vec * dockedLen_;
                }
                pos = shipPosition_ - vec * searchL;
                if (TownStageManager::getSingleton()->stage_.collCrossCheckPoly(shipPosition_, pos, &retLen, 1) > 0) {
                    dockedLen_ = retLen;
                    dockedatPos_ = shipPosition_ + vec * dockedLen_;
                }
            }
            ret = ACTION_TYPE_SHIP;
        }
    }
    return ret;
}

ARM TownShipAction2* TownShipAction2::getSingleton()
{
    static TownShipAction2 townShipAction;
    return &townShipAction;
}

ARM void TownShipAction2::setDirection(short playerDir)
{
    prevShipDirection_ = shipDirection_;
    if (((playerDir < 0 ? -playerDir : playerDir) <= 0x3fff || (prevShipDirection_ < 0 ? -prevShipDirection_ : prevShipDirection_) <= 0x3fff)
        && ((playerDir - prevShipDirection_) < 0 ? -(playerDir - prevShipDirection_) : (playerDir - prevShipDirection_)) < 0x7fff) {
        short tempDir = playerDir / 12 + prevShipDirection_ / 12 * 11;
        shipDirection_ = tempDir;
        TownStageManager::getSingleton()->rotObjectUid(0x1f4, shipDirection_);
        TownStageManager::getSingleton()->rotObjectUid(0x1f3, shipDirection_);
    } else {
        u16 tempDir = (u16)playerDir / 12 + (u16)prevShipDirection_ / 12 * 11;
        shipDirection_ = tempDir;
        TownStageManager::getSingleton()->rotObjectUid(0x1f4, shipDirection_);
        TownStageManager::getSingleton()->rotObjectUid(0x1f3, shipDirection_);
    }
}

ARM void TownShipAction2::setShipNamiAlpha()
{
    dss::Fix32Vector3 vec;
    vec = shipPosition_ - prevShipPosition_;
    short dir = shipDirection_ - prevShipDirection_;
    int add;
    if (unkfunc_02031e84(vec.lengthsq().value) > 10 || status::BaseActionStatus::abs(dir) > 200) {
        add = 2;
    } else {
        add = -2;
    }
    namiAlpha_ += add;
    namiAlpha_ = dss::min<int>(namiAlpha_, 0x1f);
    namiAlpha_ = dss::max<int>(namiAlpha_, 0);
    TownStageManager::getSingleton()->setAlpha(shipNamiObjectId_, namiAlpha_);
}

ARM void TownShipAction2::setShipPosition(dss::Fix32Vector3& pos)
{
    TownStageManager::getSingleton()->setPosByObjectID(shipObjectId_, pos);
    TownStageManager::getSingleton()->setPosByObjectID(shipNamiObjectId_, pos);
    shipPosition_ = pos;
    shipNamiPosition_ = pos;
}

ARM void TownShipAction2::shipMove()
{
    dss::Fix32Vector3 tempPos;
    dss::Fix32Vector3 nowPos;
    dss::Fix32Vector3 nextPos;
    prevShipPosition_ = shipPosition_;
    nowPos = shipPosition_;
    TownActionCalculate::normalMove(shipPosition_, dirIdx_, TownPlayerAction::shipSpeed);
    nextPos = shipPosition_;
    TownActionCalculate::townCharaColl(nowPos, nextPos, TownPlayerAction::shipR, -1, -1, -1, TownPlayerAction::shipCtrLen, 0);
    tempPos = nextPos;
    TownActionCalculate::townShipStageColl(nowPos, nextPos, TownPlayerAction::shipCollR, TownPlayerAction::shipR, TownPlayerAction::shipR);
    nextPos.vy = nowPos.vy;
    setShipPosition(nextPos);
    setDirection(dirIdx_);
    setShipNamiAlpha();
    if (nowPos.vx != nextPos.vx || nowPos.vz != nextPos.vz) {
        ctrSurfacePoly_ = -1;
        ctrSurfaceId_ = -1;
    }
    position_ = nextPos;
}
