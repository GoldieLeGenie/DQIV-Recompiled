#pragma ipa file
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionHenge.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownExtraMapObjManager.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownIkadaAction2.hpp"
#include "ov000/town/TownKaidanAction2.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownRopeAction2.hpp"
#include "ov000/town/TownShipAction2.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSubeAction.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"

ARM int TownActionWalk::setup()
{
    searchObjectId_ = -1;
    ctrSurfaceId_ = -1;
    sekaijyuSurfaceId_ = -1;
    cureFloor_ = 0;
    moveFlag_ = 1;
    menu_ = 0;
    return -1;
}

ARM void TownActionWalk::execute()
{
    collActionFlag_ = -1;
    moveFlag_ = 0;
    short nowIdx = dirIdx_;
    dss::Fix32Vector3 nowPos = position_;
    TownActionCalculate::normalMove(position_, dirIdx_, TownPlayerAction::walkSpeed);
    short nextIdx = dirIdx_;
    dss::Fix32Vector3 nextPos = position_;
    nowPos.vy += TownPlayerAction::collR;
    nextPos.vy += TownPlayerAction::collR;
    if (nowPos != nextPos) {
        moveFlag_ = 1;
    }
    if (nowIdx != nextIdx) {
        searchPolyNo_ = -1;
        searchObjectId_ = -1;
    }
    townCharColl(nowPos, nextPos, 0);
    collActionFlag_ = TownActionCalculate::townStageColl(nowPos, nextPos, TownPlayerAction::collR, TownPlayerAction::surfaceR, TownPlayerAction::townCharaPreR);
    if (nowPos != nextPos) {
        g_HengeNoTsue.execute();
        ctrSurfaceId_ = -1;
        idoSurfaceid_ = -1;
        searchPolyNo_ = -1;
        searchObjectId_ = -1;
        sekaijyuSurfaceId_ = -1;
    }
    nextPos.vy -= TownPlayerAction::collR;
    nowPos.vy -= TownPlayerAction::collR;
    TownKaidanAction2::getSingleton()->setPlayerFixPosition(nowPos, nextPos);
    position_ = nextPos;
}

ARM int TownActionWalk::update()
{
    int ret;
    if (g_HengeNoTsue.isEnd() == 1) {
        g_HengeNoTsue.setNextAction(0);
        TownActionHenge::getSingleton()->resetParty();
        return ACTION_TYPE_HENGE;
    }
    if (ctrSurfaceId_ == -1) {
        ctrSurfaceId_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(0xc);
    }
    if (idoSurfaceid_ == -1) {
        idoSurfaceid_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(5);
    }
    if (TownActionCalculate::checkTalking(position_, dirIdx_, searchObjectId_)) {
        return -1;
    }
    if (collActionFlag_ != -1) {
        TownFallAction::getSingleton()->setCollFall();
        return collActionFlag_;
    }
    ret = TownFallAction::getSingleton()->startCheck();
    if (ret != -1) {
        TownFallAction::getSingleton()->setCollFall();
        return ret;
    }
    ret = TownKaidanAction2::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    ret = TownSubeAction::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    ret = TownRopeAction2::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    ret = TownShipAction2::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    ret = TownIkadaAction2::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    ret = TownDoorAction::getSingleton()->startCheck();
    if (ret != -1) {
        return ret;
    }
    if (searchObjectId_ == -1) {
        searchObjectId_ = TownStageManager::getSingleton()->coll_.getSearchObjectId();
        searchPolyNo_ = TownStageManager::getSingleton()->coll_.getSearchPolyNo();
    }
    checkCureFloor();
    if (sekaijyuSurfaceId_ == -1) {
        sekaijyuSurfaceId_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(0xd);
    }
    if (TownPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BENRI_BUTTON) {
        int searchRet = searchObject(0);
        TownPlayerManager::getSingleton()->searchAction_ = searchRet;
    }
    return ret;
}

ARM int TownActionWalk::getMapUid()
{
    int mapUid = TownStageManager::getSingleton()->getMapObjUid(searchObjectId_);
    if (mapUid != 0) {
        TownStageManager::getSingleton()->getObjectPos(searchObjectId_, searchPolyNo_, &searchObjectPos_);
        if (TownActionCalculate::directionCheckByPosition(position_, searchObjectPos_, dirIdx_, 0xc42) == true) {
            return mapUid;
        }
    } else {
        mapUid = TownExtraMapObjManager::getSingleton()->checkFloorMapUid(position_);
    }
    return mapUid;
}

ARM int TownActionWalk::checkMapUidObject(int mapUid, int flag)
{
    int ret = 1;
    if (mapUid != 0) {
        searchObjectSide_ = 0;
        if (TownStageManager::getSingleton()->getObjWallNo(searchObjectId_, searchPolyNo_) == 2) {
            searchObjectSide_ = 1;
        }
        if (TownFurnitureManager::getSingleton()->checkObject(mapUid, searchObjectSide_, flag, 0) == true) {
            TownPlayerManager::getSingleton()->setPlayerCommand(START_SEARCH_COMMAND);
            if (!flag) {
                switch (TownStageManager::getSingleton()->getMapObjCommonId(searchObjectId_)) {
                case 0x98:
                case 0xef:
                case 0x106:
                case 0x107:
                    searchObjectId_ = -1;
                    break;
                }
            }
            ret = 2;
        }
        TownPlayerManager::getSingleton()->searchMapUid_ = mapUid;
    }
    return ret;
}

ARM void TownActionWalk::checkCureFloor()
{
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(4) != -1) {
        if (cureFloor_ == 0) {
            TownPlayerManager::getSingleton()->setCureFloor();
            cureFloor_ = 1;
        }
    } else {
        cureFloor_ = 0;
    }
}

ARM TownActionWalk* TownActionWalk::getSingleton()
{
    static TownActionWalk townActionWalk;
    return &townActionWalk;
}

ARM void TownActionWalk::townCharColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, int flag)
{
    TownActionCalculate::townCharaColl(nowPos, nextPos, TownPlayerAction::townCharaR, ctrSurfaceId_, searchObjectId_, searchPolyNo_, TownPlayerAction::walkCtrLen, flag);
}

ARM int TownActionWalk::unkfunc_02125dcc(int mapUid)
{
    int monster = TownFurnitureManager::getSingleton()->monsterEncount(mapUid);
    if (monster == 0) {
        return 0;
    }
    if (monster == 0xa8) {
        TextAPI::setMACRO0(0xd, 0x60000000, 0xe3);
        TextAPI::setMACRO0(0x13, 0x60000000, 0xe3);
    } else {
        TextAPI::setMACRO0(0xd, 0x60000000, 0xe2);
        TextAPI::setMACRO0(0x13, 0x60000000, 0xe2);
    }
    if ((unsigned int)(monster - 0xa7) > 1) {
        return 0;
    }
    ui_MsgSndSet(0x30);
    TownWindowSystem::getSingleton()->openCommonMessage();
    TownWindowSystem::getSingleton()->addCommonMessage(0xc40c6);
    TownWindowSystem::getSingleton()->addCommonMessage(0xc40d8);
    TownPlayerManager::getSingleton()->setEncount(monster);
    return 1;
}

ARM int TownActionWalk::searchObject(int flag)
{
    menu_ = flag;
    int floorMapUid = TownExtraMapObjManager::getSingleton()->checkFloorMapUid(position_);
    if (floorMapUid != 0) {
        int ret = TownFurnitureManager::getSingleton()->checkObject(floorMapUid, 0, 1, 1);
        TownPlayerManager::getSingleton()->searchMapUid_ = floorMapUid;
        if (TownFurnitureManager::getSingleton()->prevCheck_ == 2 || TownFurnitureManager::getSingleton()->prevCheck_ == 3) {
            cmn::PartyTalk::getSingleton()->resetPartyTalk();
        }
        if (ret == 1) {
            TownPlayerManager::getSingleton()->setPlayerCommand(START_SEARCH_COMMAND);
            return 2;
        }
    }
    int mapUid = TownStageManager::getSingleton()->getMapObjUid(searchObjectId_);
    if (unkfunc_02125dcc(mapUid) == 1) {
        TownPlayerManager::getSingleton()->setLock(1);
        g_Stage.encountMapUid_ = mapUid;
        return 3;
    }
    if (idoSurfaceid_ != -1 && TownStageManager::getSingleton()->getHitSurfaceIdByType(6) == -1) {
        int surface = idoSurfaceid_;
        TownStageManager::getSingleton()->coll_.m_surfaceType[5] = surface;
        if (TownPlayerManager::getSingleton()->isIdoLinkPos() == true) {
            TownPlayerManager::getSingleton()->flagIdoLink_ = 1;
            TownPlayerManager::getSingleton()->setPlayerCommand(START_IDO_LINK_COMMAND);
            return 4;
        }
    }
    if (mapUid != 0) {
        if (TownStageManager::getSingleton()->getObjectPos(searchObjectId_, searchPolyNo_, &searchObjectPos_) == 1) {
            if (TownActionCalculate::directionCheckByPosition(position_, searchObjectPos_, dirIdx_, 0xc42) == true) {
                if (checkMapUidObject(mapUid, flag) == 2) {
                    TownPlayerManager::getSingleton()->searchMapUid_ = mapUid;
                    if (TownFurnitureManager::getSingleton()->prevCheck_ == 2 || TownFurnitureManager::getSingleton()->prevCheck_ == 3) {
                        cmn::PartyTalk::getSingleton()->resetPartyTalk();
                    }
                    return 2;
                }
            }
        }
        return 1;
    }
    if (sekaijyuSurfaceId_ != -1) {
        return status::g_Party.isHaveItem(0x82) == false ? 6 : 7;
    }
    return 1;
}

ARM int TownActionWalk::getSekaijyuUid()
{
    switch (g_Global.getMapName()[3]) {
    case '2':
        return 0x1843;
    case '3':
        return 0x1845;
    case '4':
        return 0x1848;
    case '5':
        return 0x184e;
    }
    return 0;
}
