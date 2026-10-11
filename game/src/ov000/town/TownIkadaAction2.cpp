#pragma ipa file
#include "ov000/town/TownIkadaAction2.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownDamageFloor.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/Global.hpp"
#include "main/status/StageStatus.hpp"

ARM int TownIkadaAction2::setup()
{
    int ret = -1;
    ctrSurfacePoly_ = -1;
    ctrSurfaceId_ = -1;
    counter_ = 0;
    ikadaObjectId_ = TownStageManager::getSingleton()->getObjectIDfromMapUid(0x1ea);
    if (dss::strcmp("hhk1f1", g_Global.getPrevMapName()) == 0) {
        g_cmnPartyInfo.setIkadaMapName("hhout");
    }
    if (ikadaObjectId_ != -1) {
        if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
            if (g_Stage.idoLink_.data_.link_.encount_ == 0) {
                position_.vy = ikadaPosition_.vy;
            }
            setIkadaPosition(position_);
            moveMode_ = IKADA_MOVE;
            ret = ACTION_TYPE_IKADA;
        } else {
            if (*g_cmnPartyInfo.getIkadaMapName() == 0) {
                ikadaPosition_ = TownStageManager::getSingleton()->getMapUidPos(0x1ea);
                g_cmnPartyInfo.setIkadaInfo(g_Global.getMapName(), &ikadaPosition_);
            }
            if (dss::strcmp(g_cmnPartyInfo.getIkadaMapName(), g_Global.getMapName()) == 0) {
                ikadaPosition_ = g_cmnPartyInfo.getIkadaPos();
                setIkadaPosition(ikadaPosition_);
            } else {
                TownStageManager::getSingleton()->eraseObject(0x1ea, 1);
                ikadaObjectId_ = -1;
            }
        }
    }
    return ret;
}

ARM void TownIkadaAction2::execute()
{
    switch (moveMode_) {
    case IKADA_MOVE:
        ikadaMove();
        break;
    case GET_ON_IKADA:
    case GET_OFF_IKADA:
        gMoveToTarget.execute();
        break;
    }
}

ARM int TownIkadaAction2::update()
{
    dss::Fix32Vector3 target;
    int ret = -1;
    switch (moveMode_) {
    case IKADA_MOVE:
        if (ctrSurfaceId_ == -1) {
            ctrSurfaceId_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(0xc);
            ctrSurfacePoly_ = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[0xc];
        }
        if (TownActionCalculate::checkIkadaTalk(position_, dirIdx_, ctrSurfaceId_, ctrSurfacePoly_, 0) == true) {
            break;
        }
        {
            dss::Fix32Vector3 nowPos;
            dss::Fix32Vector3 nextPos;
            if (TownActionCalculate::checkGetDownIkada(position_, dirIdx_, target) == true) {
                gMoveToTarget.setAction(position_, target, TownPlayerAction::getOnOffSpeed, 1, 0, ACTION_TYPE_IKADA);
                TownPlayerManager::getSingleton()->setPartyToFirst(position_);
                TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
                moveMode_ = GET_OFF_IKADA;
                TownPlayerManager::getSingleton()->setRemote(1);
                counter_ = 0;
            }
            ret = TownDoorAction::getSingleton()->startCheck();
        }
        break;
    case GET_ON_IKADA:
        if (gMoveToTarget.update() != -1) {
            moveMode_ = IKADA_MOVE;
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_SHIP_IKADA;
            TownPlayerManager::getSingleton()->setRemote(0);
            TownDamageFloor::getSingleton()->clear();
        }
        break;
    case GET_OFF_IKADA:
        if (counter_ == 10) {
            TownDamageFloor::getSingleton()->damageFlag_ = 1;
            TownDamageFloor::getSingleton()->effectFlag_ = 1;
            TownDamageFloor::getSingleton()->walkCount_ = 0;
        }
        counter_++;
        if (gMoveToTarget.update() != -1) {
            ret = ACTION_TYPE_WALK;
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
            TownPlayerManager::getSingleton()->setRemote(0);
            g_cmnPartyInfo.setIkadaInfo(g_Global.getMapName(), &ikadaPosition_);
        }
        break;
    }
    return ret;
}

ARM int TownIkadaAction2::startCheck()
{
    int ret = -1;
    dss::Fix32Vector3 target;
    dss::Fix32 len;
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0xa) != -1 && ikadaObjectId_ != -1) {
        len = TownPlayerAction::ikadaR + TownPlayerAction::getDownL * 2;
        if (TownActionCalculate::checkGetOnShipAndIkada(position_, ikadaPosition_, dirIdx_, len)) {
            target = ikadaPosition_;
            gMoveToTarget.setAction(position_, target, TownPlayerAction::getOnOffSpeed, 1, 1, ACTION_TYPE_IKADA);
            moveMode_ = GET_ON_IKADA;
            TownPlayerManager::getSingleton()->setRemote(1);
            ret = ACTION_TYPE_IKADA;
        }
    }
    return ret;
}

ARM TownIkadaAction2* TownIkadaAction2::getSingleton()
{
    static TownIkadaAction2 townIkadaAction;
    return &townIkadaAction;
}

ARM void TownIkadaAction2::ikadaMove()
{
    dss::Fix32Vector3 out;
    dss::Fix32Vector3 nowPos;
    dss::Fix32Vector3 nextPos;
    nowPos = ikadaPosition_;
    TownActionCalculate::normalMove(ikadaPosition_, dirIdx_, TownPlayerAction::shipSpeed);
    nextPos = ikadaPosition_;
    nowPos.vy += TownPlayerAction::collR;
    nextPos.vy += TownPlayerAction::collR;
    TownStageManager::getSingleton()->boxCompute(nowPos, nextPos, TownPlayerAction::ikadaR, &out);
    nextPos = out;
    nextPos.vy = nowPos.vy - TownPlayerAction::collR;
    ikadaPosition_ = nextPos;
    position_ = nextPos;
    if (nowPos.vx != nextPos.vx || nowPos.vz != nextPos.vz) {
        ctrSurfacePoly_ = -1;
        ctrSurfaceId_ = -1;
    }
    TownStageManager::getSingleton()->setPosByObjectID(ikadaObjectId_, ikadaPosition_);
}

ARM void TownIkadaAction2::setIkadaDataByScript(const char* name, dss::Fix32Vector3& pos)
{
    g_cmnPartyInfo.setIkadaInfo((char*)name, &pos);
    if (dss::strcmp(name, g_Global.getMapName()) == 0) {
        ikadaPosition_ = pos;
        ikadaObjectId_ = TownStageManager::getSingleton()->getObjectIDfromMapUid(0x1ea);
    } else {
        TownStageManager::getSingleton()->eraseObject(0x1ea, 1);
        ikadaObjectId_ = -1;
    }
}

ARM void TownIkadaAction2::setIkadaPosition(dss::Fix32Vector3& pos)
{
    TownStageManager::getSingleton()->setPosByObjectID(ikadaObjectId_, pos);
    ikadaPosition_ = pos;
}

ARM bool TownIkadaAction2::checkIkadaTalk(bool flag)
{
    return TownActionCalculate::checkIkadaTalk(position_, dirIdx_, ctrSurfaceId_, ctrSurfacePoly_, flag);
}
