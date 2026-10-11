#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/GameFlag.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
char s_srout_map[8] = "srout";
cmn::CommonPartyInfo g_cmnPartyInfo;



cmn::CommonPartyInfo::CommonPartyInfo()
{
    this->prevLocation_ = 0;
    this->mapNameAtIkada_[0] = 0;
    this->mapNameAtShip_[0] = 0;
}

ARM dss::Fix32Vector3 *cmn::CommonPartyInfo::getPositionArrayPointer()
{
  return this->positionArray_;
}


ARM short* cmn::CommonPartyInfo::getDirectionArrayPointer()
{
  return this->directionArray_;
}


ARM void cmn::CommonPartyInfo::setPartyInfo(dss::Fix32Vector3* partyPosition, short partyDirIdx)
{
    this->position_ = *partyPosition;
    this->dirIdx_ = partyDirIdx;
}


ARM void cmn::CommonPartyInfo::getPartyInfo(dss::Fix32Vector3* partyPos, short* partyDirIdx)
{
    *partyPos = this->position_;
    *partyDirIdx = this->dirIdx_;
}

ARM void cmn::CommonPartyInfo::setBashaInfo(dss::Fix32Vector3* bashaLPos, dss::Fix32Vector3* bashaRPos,
                                        short bashaLIdx, short bashaRIdx,
                                        int countParty, int countLFix, int countRFix)
{
    this->bashaLPos_ = *bashaLPos;
    this->bashaRPos_ = *bashaRPos;
    this->bashaLIdx_ = bashaLIdx;
    this->bashaRIdx_ = bashaRIdx;
    this->countPartyArray_ = countParty;
    this->countLFix_ = countLFix;
    this->countRFix_ = countRFix;
}


ARM void cmn::CommonPartyInfo::getBashaInfo(dss::Fix32Vector3* bashaLPos, dss::Fix32Vector3* bashaRPos,
                                        short* bashaLIdx, short* bashaRIdx,
                                        int* countParty, int* countLFix, int* countRFix)
{
    *bashaLPos = this->bashaLPos_;
    *bashaRPos = this->bashaRPos_;
    *bashaLIdx = this->bashaLIdx_;
    *bashaRIdx = this->bashaRIdx_;
    *countParty = this->countPartyArray_;
    *countLFix = this->countLFix_;
    *countRFix = this->countRFix_;
}

ARM void cmn::CommonPartyInfo::setDirIdx(short dirIdx)
{
  this->dirIdx_ = dirIdx;
  return;
}


ARM void cmn::CommonPartyInfo::setStartPosition()
{
  this->position_ = this->startPos_;
  return;
}

ARM void cmn::CommonPartyInfo::setShipInfo(char* name, dss::Fix32Vector3* pos, short idx)
{
    dss::strcpy_s(this->mapNameAtShip_, 10, name);
    this->townShipPos_ = *pos;
    this->shipDirection_ = idx;
}

ARM char * cmn::CommonPartyInfo::getShipMapName()
{
  return this->mapNameAtShip_;
}


ARM void cmn::CommonPartyInfo::getShipInfo(dss::Fix32Vector3* pos, short* idx)
{
    *pos = this->townShipPos_;
    *idx = this->shipDirection_;
}

ARM void cmn::CommonPartyInfo::setIkadaInfo(const char*name,dss::Fix32Vector3 *pos)
{
  dss::strcpy_s(this->mapNameAtIkada_,10,name);
  this->townIkadaPos_ = *pos;
  return;
}

ARM void cmn::CommonPartyInfo::setIkadaMapName(const char*name)
{
  dss::strcpy_s(this->mapNameAtIkada_,10,name);
  return;
}


ARM char * cmn::CommonPartyInfo::getIkadaMapName()
{
  return this->mapNameAtIkada_;
}


ARM dss::Fix32Vector3 cmn::CommonPartyInfo::getIkadaPos()
{
    return this->townIkadaPos_;
}

ARM void cmn::CommonPartyInfo::resetShipIkadaMapName()
{
  this->mapNameAtIkada_[0] = '\0';
  this->mapNameAtShip_[0] = '\0';
  return;
}


ARM int cmn::CommonPartyInfo::isBalloonEnable()
{
    if (status::g_Party.balloon_  == 1) {
        int fieldType = g_Stage.balloonFieldType_;
        if (fieldType == g_Global.getFieldType()) {
            return 1;
        }
    }
    return 0;
}

ARM int cmn::CommonPartyInfo::isShipEnable()
{
    if (status::g_Party.ship_ == 1 && g_Global.getFieldType() == 0) {
        return 1;
    }
    return 0;
}


ARM void cmn::CommonPartyInfo::setBalloonFieldTypeByTownId(int townId)
{
    switch (townId) {
    case 22:
    case 23:
    case 24:
    case 25:
        ::g_Stage.balloonFieldType_ = 1;
        break;
    default:
        ::g_Stage.balloonFieldType_ = 0;
        break;
    }
}


ARM void cmn::CommonPartyInfo::callCarriage()
{
    status::g_Party.basha_ = 1;
    g_Stage.setBashaEnable(1);
    g_Stage.setBashaEnter(1);
    this->barron_ = 1;
    if (data_0210bb94.unkfunc_02058114(0xE) != 0) {
        FieldPlayerManager::getSingleton()->resetParty();
        return;
    }
    TownPlayerManager::getSingleton()->resetParty();
}


ARM int cmn::CommonPartyInfo::isBarronArea(dss::Fix32Vector3* pos)
{
    if (data_0210bb94.unkfunc_02058114(0xE) != 0) {
        return FieldPlayerManager::getSingleton()->checkBarronArea(*pos);
    }
    if (dss::strcmp(s_srout_map, g_Global.getMapName()) == 0) {
        return 1;
    }
    this->barron_ = 0;
    return 0;
}

ARM int cmn::CommonPartyInfo::isUsedBarron()
{
  return this->barron_;
}


ARM void cmn::CommonPartyInfo::checkBallon(dss::Fix32Vector3* pos)
{
    if (isBarronArea(pos) == 1) {
        if (this->barron_ == 1) {
            g_Stage.setBashaEnable(1);
            g_Stage.setBashaEnter(1);
            status::g_Party.basha_ = 1;
            return;
        }
        if (data_0210bb94.unkfunc_02058114(0xE) != 0) {
            status::g_Party.basha_ = 0;
            return;
        }
        status::g_Party.basha_ = 1;
        ::g_Stage.setBashaEnter(0);
        return;
    }
    if (g_AreaFlag.check(310) != 1
     && g_AreaFlag.check(388) != 1) {
        return;
    }
    status::g_Party.basha_ = 1;
}

ARM void cmn::CommonPartyInfo::setMenuAction(MENU_ACTION mode)
{
    if (mode != MENU_ACTION_NONE) {
        if (data_0210bb94.unkfunc_02058114(0xE) != MENU_ACTION_NONE) {
            FieldPlayerManager::getSingleton();
            cmn::PlayerManager::setLock(1);
        } else {
            TownPlayerManager::getSingleton()->setLock(1);
        }
    } else {
        if (data_0210bb94.unkfunc_02058114(0xE) != MENU_ACTION_NONE) {
            FieldPlayerManager::getSingleton();
            cmn::PlayerManager::setLock(0);
        } else {
            TownPlayerManager::getSingleton()->setLock(0);
        }
    }
    if (mode == MENU_RURA_FAILED) {
        TownSystem::getSingleton()->scriptLock_ = 1;
    }
    this->menuAction_ = mode;
}

ARM void cmn::CommonPartyInfo::setBalloonPosByExtraSave()
{
    this->tempBalloonPos_ = dss::Fix32Vector3(g_Stage.balloonPosition_);
    this->tempBalloonFieldType_ = g_Stage.balloonFieldType_;

    dss::Fix32Vector3 townPos = cmn::CommonRuraData::getSingleton()->getBalloonTownPos(1);
    g_Stage.balloonPosition_ = dss::Fix32Vector3(townPos);
    g_Stage.balloonFieldType_ = 0;

    this->tempShipPos_ = dss::Fix32Vector3(g_Stage.shipPosition_);
}


ARM void cmn::CommonPartyInfo::resetBalloonPosByExtraSave()
{
    g_Stage.balloonPosition_ = dss::Fix32Vector3(this->tempBalloonPos_);
    g_Stage.balloonFieldType_ = this->tempBalloonFieldType_;
    g_Stage.shipPosition_ = dss::Fix32Vector3(this->tempShipPos_);
}