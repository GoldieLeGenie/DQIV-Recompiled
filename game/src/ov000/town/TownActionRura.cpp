#pragma ipa file
#include "ov000/town/TownActionRura.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/StageStatus.hpp"

ARM int TownActionRura::setup()
{
    return -1;
}

ARM void TownActionRura::execute()
{
    position_ = tempPos_;
    short dirIdx = dirIdx_;
    gMoveToTarget.execute();
    dss::Fix32Vector3 pos = position_;
    dirIdx_ = dirIdx;
    TownStageManager::getSingleton()->computeCollFloor(pos, TownPlayerAction::collR, pos);
    TownPlayerManager::getSingleton()->setEffectPos(position_);
    tempPos_ = position_;
    position_ = startPos_;
}

ARM int TownActionRura::update()
{
    if (gMoveToTarget.update() != -1) {
        int townId = g_Stage.getRuraTownID();
        cmn::g_extraMapLink.setRuraLink();
        if (g_Stage.ruraFlag_ != 3) {
            g_Stage.balloonPosition_ = cmn::CommonRuraData::getSingleton()->getBalloonTownPos(townId);
            g_Stage.shipPosition_ = cmn::CommonRuraData::getSingleton()->getShipTownPos(townId);
            g_cmnPartyInfo.setBalloonFieldTypeByTownId(townId);
            cmn::CommonRuraData::getSingleton()->getSymbolID(townId);
        }
        TownPlayerManager::getSingleton()->setLock(1);
        g_Stage.idoLink_.data_.link_.inFlag_ = 0;
        g_Stage.idoLink_.data_.link_.outFlag_ = 0;
        g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
    }
    return -1;
}

ARM int TownActionRura::startCheck()
{
    static const dss::Fix32 dy(0x14000);
    short dirIdx = dirIdx_;
    dss::Fix32Vector3 pos = position_;
    startPos_ = position_;
    tempPos_ = position_;
    pos.vy += dy;
    gMoveToTarget.setAction(position_, pos, TownPlayerAction::ruraSpeed, 0, 0, ACTION_TYPE_RURA);
    g_Stage.setRuraFlag(2);
    TownCamera::getSingleton()->setCameraLock(true);
    TownPlayerManager::getSingleton()->setRemote(1);
    TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
    TownPlayerManager::getSingleton()->shadowSet_ = 1;
    if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
        TownPlayerManager::getSingleton()->setPartyToFirst(position_);
    }
    dirIdx_ = dirIdx;
    TownPlayerManager::getSingleton()->mapChangeSE_ = 0;
    SoundManager::playSe(0x23b, 0);
    TownPlayerManager::getSingleton()->setEffectPos(position_);
    TownPlayerManager::getSingleton()->effectPosFlag_ = 1;
    return ACTION_TYPE_RURA;
}

ARM TownActionRura* TownActionRura::getSingleton()
{
    static TownActionRura townActionRura;
    return &townActionRura;
}
