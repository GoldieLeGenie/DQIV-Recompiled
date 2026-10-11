#pragma ipa file
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/sound/Sound.hpp"

static const dss::Fix32 length(0x1800);

ARM int TownDoorAction::setup()
{
    dss::Fix32 len;
    len.value = 0x11f4;
    wDoor1_ObjNo_ = -1;
    wDoor2_ObjNo_ = -1;
    sDoor_ObjNo_ = -1;
    tDoor_ObjNo_ = -1;
    doorType_ = DOOR_S;
    openType_ = DOOR_OPEN_KEY;
    doorKeyType_ = KEY_NONE;
    judgeType_ = JUDGE_KEY;
    eventDoorCount_ = 0;
    message_ = 0;
    crackOrin_ = 0;
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    pos.vx += 0x46;
    pos.vz += 0x46;
    pos.vy += 0x64;
    short dirIdx = TownPlayerManager::getSingleton()->getDirection();
    dss::Fix32Vector3 dir;
    TownActionCalculate::getDirByIdx(dirIdx, dir);
    dss::Fix32Vector3 end = pos - dir * len;
    int exitIndex = StageLink::getTownExitIndex();
    coll_GetPolyNoBySurface(TownStageManager::getSingleton()->stage_.m_fld.m_coll, exitIndex, 0);
    int poly;
    short surface[2] = { 0x1000, 0x7000 };
    if (g_Stage.idoLink_.data_.link_.encount_ == 1) {
        return -1;
    }
    TownStageManager::getSingleton()->getCrossPolygonOtherSurface(pos, end, surface, 2, &poly, NULL, 0);
    if (poly != -1) {
        int obj = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, poly);
        if (obj != -1) {
            int commonId = TownStageManager::getSingleton()->getMapObjCommonId(obj);
            if (isDoorObject(commonId) == true) {
                if (checkOpen(obj, commonId) == false) {
                    return -1;
                }
                int uid = TownStageManager::getSingleton()->getMapObjUid(obj);
                if (doorType_ == DOOR_W) {
                    int obj2 = TownActionCalculate::searchPairWdoor(obj, NULL, NULL);
                    int uid2 = TownStageManager::getSingleton()->getMapObjUid(obj2);
                    TownFurnitureManager::getSingleton()->openDoor(uid);
                    TownFurnitureManager::getSingleton()->openDoor(uid2);
                    TownStageManager::getSingleton()->eraseObject(uid, 1);
                    TownStageManager::getSingleton()->eraseObject(uid2, 1);
                } else {
                    TownFurnitureManager::getSingleton()->openDoor(uid);
                    TownStageManager::getSingleton()->eraseObject(uid, 1);
                }
            }
        }
    }
    return -1;
}

ARM void TownDoorAction::execute()
{
    if (counter_ == 0) {
        switch (doorType_) {
        case DOOR_S:
            setDoorS(backupObj_);
            Sound::sePlayDirect(0x134);
            break;
        case DOOR_T:
            Sound::sePlayDirect(0x136);
            setDoorT(backupObj_);
            break;
        case DOOR_W:
            setDoorW(backupObj_);
            Sound::sePlayDirect(0x135);
            break;
        }
    }
    counter_++;
}

ARM int TownDoorAction::update()
{
    int ret = -1;
    int w1 = TownStageManager::getSingleton()->getMapObjUid(wDoor1_ObjNo_);
    int w2 = TownStageManager::getSingleton()->getMapObjUid(wDoor2_ObjNo_);
    int s = TownStageManager::getSingleton()->getMapObjUid(sDoor_ObjNo_);
    int t = TownStageManager::getSingleton()->getMapObjUid(tDoor_ObjNo_);
    switch (doorType_) {
    case DOOR_W:
        if (w1 != 0 && TownStageManager::getSingleton()->stage_.IsCommonAnimationEnd(w1) == true) {
            TownStageManager::getSingleton()->collEraseObject(wDoor1_ObjNo_);
            TownStageManager::getSingleton()->setSoftErase(wDoor1_ObjNo_);
            TownFurnitureManager::getSingleton()->openDoor(w1);
            ret = nextAction_;
        }
        if (w2 != 0 && TownStageManager::getSingleton()->stage_.IsCommonAnimationEnd(w2) == true) {
            TownStageManager::getSingleton()->collEraseObject(wDoor2_ObjNo_);
            TownStageManager::getSingleton()->setSoftErase(wDoor2_ObjNo_);
            TownFurnitureManager::getSingleton()->openDoor(w2);
            ret = nextAction_;
        }
        break;
    case DOOR_S:
        if (s != 0 && TownStageManager::getSingleton()->stage_.IsCommonAnimationEnd(s) == true) {
            TownStageManager::getSingleton()->collEraseObject(sDoor_ObjNo_);
            TownStageManager::getSingleton()->setSoftErase(sDoor_ObjNo_);
            TownFurnitureManager::getSingleton()->openDoor(s);
            ret = nextAction_;
        }
        break;
    case DOOR_T:
        if (TownStageManager::getSingleton()->isEndSoftErase(tDoor_ObjNo_)) {
            TownFurnitureManager::getSingleton()->openDoor(t);
            ret = nextAction_;
        }
        break;
    }
    if (ret != -1) {
        TownPlayerManager::getSingleton()->resetMapLink((RESET_EXIT_LOCK_TYPE)1);
        TownPlayerManager::getSingleton()->setRemote(0);
    }
    return ret;
}

ARM int TownDoorAction::startCheck()
{
    int ret = -1;
    wDoor1_ObjNo_ = -1;
    wDoor2_ObjNo_ = -1;
    sDoor_ObjNo_ = -1;
    tDoor_ObjNo_ = -1;
    message_ = 0;
    if (checkSurface() == true) {
        TownPlayerManager::getSingleton()->lockMapLink((EXIT_LOCK_TYPE)1);
        nextAction_ = TownPlayerManager::getSingleton()->player_.actionType_;
        TownPlayerManager::getSingleton()->setRemote(1);
        ret = ACTION_TYPE_DOOR;
    } else if (checkObject() == true) {
        TownPlayerManager::getSingleton()->lockMapLink((EXIT_LOCK_TYPE)1);
        nextAction_ = TownPlayerManager::getSingleton()->player_.actionType_;
        TownPlayerManager::getSingleton()->setRemote(1);
        ret = ACTION_TYPE_DOOR;
    }
    if (message_ == 1) {
        ui_MsgSndSet(0x30);
        int obj = TownStageManager::getSingleton()->coll_.getSearchObjectId();
        int msg = -1;
        int count = 1;
        switch (openType_) {
        case DOOR_NOT_OPEN_KEY:
            if (checkOpenMessage(obj) == true) {
                if (haveKey_ == KEY_NONE) {
                    msg = 0xc4119;
                } else {
                    msg = 0xc411b;
                }
            }
            TownStageManager::getSingleton()->coll_.m_surfaceType[1] = -1;
            TownStageManager::getSingleton()->coll_.m_surfaceType[7] = -1;
            break;
        case DOOR_NOT_OPEN_EVENT:
            if (checkOpenMessage(obj) == true) {
                msg = 0x26391;
            }
            TownStageManager::getSingleton()->coll_.m_surfaceType[1] = -1;
            TownStageManager::getSingleton()->coll_.m_surfaceType[7] = -1;
            break;
        case DOOR_NOT_OPEN_MAP_ID:
            TownStageManager::getSingleton()->collEraseObject(obj);
            break;
        case DOOR_CRACK_ORIN:
            msg = 0xb99e;
            count = 2;
            break;
        }
        if (msg != -1) {
            TownWindowSystem::getSingleton()->openMessage(msg, count);
        }
    }
    prev_w1_objNo_ = wDoor1_ObjNo_;
    prev_w2_objNo_ = wDoor2_ObjNo_;
    prev_s_objNo_ = sDoor_ObjNo_;
    prev_tDoorNo_ = tDoor_ObjNo_;
    return ret;
}

ARM TownDoorAction* TownDoorAction::getSingleton()
{
    static TownDoorAction townDoorAction;
    return &townDoorAction;
}

ARM bool TownDoorAction::checkSurface()
{
    dss::Fix32 dist;
    dss::Fix32Vector3 dir;
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 start;
    dss::Fix32Vector3 end;
    int surface = TownStageManager::getSingleton()->getHitSurfaceIdByType(1);
    int poly;
    if (surface != -1 || TownStageManager::getSingleton()->getHitSurfaceIdByType(7) != -1) {
        if (surface != -1) {
            dir = TownStageManager::getSingleton()->getHitSurfaceDirByType(1);
            pos = TownStageManager::getSingleton()->getHitSurfacePosByType(1);
            poly = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[1];
        } else {
            dir = TownStageManager::getSingleton()->getHitSurfaceDirByType(7);
            pos = TownStageManager::getSingleton()->getHitSurfacePosByType(7);
            poly = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[7];
        }
        start = pos + dir;
        end = pos - dir * length;
        int obj = TownStageManager::getSingleton()->stage_.collCrossCheckOtherNo(start, end, poly, &dist);
        if (obj != -1) {
            obj = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, obj);
            if (checkOpen(obj, TownStageManager::getSingleton()->getMapObjCommonId(obj)) == true) {
                return true;
            }
        }
    }
    return false;
}

ARM bool TownDoorAction::checkObject()
{
    int obj = TownStageManager::getSingleton()->coll_.getSearchObjectId();
    int commonId = TownStageManager::getSingleton()->getMapObjCommonId(obj);
    if (commonId == -1) {
        return false;
    }
    return checkOpen(obj, commonId) == true;
}

ARM bool TownDoorAction::checkOpen(int objNo, int commonId)
{
    if (isDoorObject(commonId) == false) {
        return false;
    }
    status::g_Party.setBattleMode();
    crackOrin_ = 0;
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        if (status::g_Party.getPlayerIndex(i) == 0x13) {
            bool alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
            if (alive == true) {
                crackOrin_ = 1;
            }
        }
    }
    if (crackOrin_ == 0) {
        openType_ = (DOOR_OPEN_TYPE)getOpenType(objNo);
        if (openType_ != DOOR_OPEN_KEY && openType_ != DOOR_OPEN_EVENT) {
            return false;
        }
    } else if (doorKeyType_ <= 2 && doorKeyType_ >= 1) {
        message_ = 1;
        openType_ = DOOR_CRACK_ORIN;
    } else {
        openType_ = (DOOR_OPEN_TYPE)getOpenType(objNo);
        openType_ = (DOOR_OPEN_TYPE)getOpenType(objNo);
        if (openType_ != DOOR_OPEN_KEY && openType_ != DOOR_OPEN_EVENT) {
            return false;
        }
    }
    backupObj_ = objNo;
    counter_ = 0;
    return true;
}

ARM bool TownDoorAction::isDoorObject(int commonId)
{
    bool ret = true;
    switch (commonId) {
    case 0x28:
        doorKeyType_ = KEY_NONE;
        doorType_ = DOOR_S;
        break;
    case 0x29:
        doorKeyType_ = KEY_TOUZOKU;
        doorType_ = DOOR_S;
        break;
    case 0x2a:
        doorKeyType_ = KEY_MAHOU;
        doorType_ = DOOR_S;
        break;
    case 0x2b:
    case 0x2c:
    case 0x2d:
        doorKeyType_ = KEY_NONE;
        doorType_ = DOOR_S;
        break;
    case 0xf9:
        doorType_ = DOOR_T;
        doorKeyType_ = KEY_SAIGO;
        break;
    case 0x11b:
        doorKeyType_ = KEY_NONE;
        doorType_ = DOOR_W;
        break;
    case 0x11c:
        doorKeyType_ = KEY_TOUZOKU;
        doorType_ = DOOR_W;
        break;
    case 0x11d:
        doorKeyType_ = KEY_MAHOU;
        doorType_ = DOOR_W;
        break;
    case 0x11e:
    case 0x11f:
    case 0x120:
        doorKeyType_ = KEY_NONE;
        doorType_ = DOOR_W;
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}

ARM void TownDoorAction::setDoorT(int objNo)
{
    TownStageManager::getSingleton()->setSoftErase(objNo);
    tDoor_ObjNo_ = objNo;
}

ARM void TownDoorAction::setDoorS(int objNo)
{
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 dir;
    static const dss::Fix32Vector3 front(0, 0, -1);
    static const dss::Fix32Vector3 side(1, 0, 0);
    dss::Fix32 dot;
    dss::Fix32 unused;
    short rot = TownStageManager::getSingleton()->stage_.getObjectRotIdxY(objNo);
    TownStageManager::getSingleton()->getObjectPos(objNo, 0, &pos);
    TownActionCalculate::getDirByIdx(rot, dir);
    vec = pos - position_;
    dot = vec * dir;
    sDoor_ObjNo_ = objNo;
    if (dot > dss::Fix32(0L)) {
        TownStageManager::getSingleton()->commonAnim(sDoor_ObjNo_, 4);
    } else {
        TownStageManager::getSingleton()->commonAnim(sDoor_ObjNo_, 2);
    }
}

ARM void TownDoorAction::setDoorW(int objNo)
{
    dss::Fix32Vector3 door1;
    dss::Fix32Vector3 door2;
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 dir;
    dss::Fix32Vector3 cross;
    TownActionCalculate::getDirByIdx(dirIdx_, dir);
    int pair = TownActionCalculate::searchPairWdoor(objNo, &door1, &door2);
    wDoor1_ObjNo_ = objNo;
    wDoor2_ObjNo_ = pair;
    vec = door2 - door1;
    cross = vec % dir;
    if (cross.vy > dss::Fix32(0L)) {
        TownStageManager::getSingleton()->commonAnim(wDoor1_ObjNo_, 4);
        TownStageManager::getSingleton()->commonAnim(wDoor2_ObjNo_, 2);
    } else {
        TownStageManager::getSingleton()->commonAnim(wDoor1_ObjNo_, 2);
        TownStageManager::getSingleton()->commonAnim(wDoor2_ObjNo_, 4);
    }
}

ARM int TownDoorAction::getOpenType(int objNo)
{
    int uid = TownStageManager::getSingleton()->getMapObjUid(objNo);
    if (uid == 0) {
        return DOOR_NOT_OPEN_MAP_ID;
    }
    for (int i = 0; i < eventDoorCount_; i++) {
        if (uid == eventDoor_[i].uid) {
            DOOR_OPEN_TYPE type = eventDoor_[i].type;
            switch (type) {
            case DOOR_NOT_OPEN_EVENT:
                message_ = 1;
                return eventDoor_[i].type;
            case DOOR_OPEN_EVENT:
            case DOOR_LOCK:
                return eventDoor_[i].type;
            }
        }
    }
    if (status::g_Party.isHaveItem(0x7d) == true) {
        haveKey_ = KEY_SAIGO;
    } else if (status::g_Party.isHaveItem(0x7c) == true) {
        haveKey_ = KEY_MAHOU;
    } else if (status::g_Party.isHaveItem(0x7b) == true) {
        haveKey_ = KEY_TOUZOKU;
    } else {
        haveKey_ = KEY_NONE;
    }
    if (doorKeyType_ > haveKey_) {
        message_ = 1;
        return DOOR_NOT_OPEN_KEY;
    }
    return DOOR_OPEN_KEY;
}

ARM bool TownDoorAction::checkOpenMessage(int objNo)
{
    bool ret = false;
    switch (doorType_) {
    case DOOR_W:
        if (objNo != prev_w1_objNo_ && objNo != prev_w2_objNo_) {
            ret = true;
        }
        wDoor1_ObjNo_ = objNo;
        break;
    case DOOR_S:
        if (objNo != prev_s_objNo_) {
            ret = true;
        }
        sDoor_ObjNo_ = objNo;
        break;
    case DOOR_T:
        if (objNo != prev_tDoorNo_) {
            ret = true;
        }
        tDoor_ObjNo_ = objNo;
        break;
    }
    return ret;
}

ARM void TownDoorAction::setEventDoor(int uid, DOOR_OPEN_TYPE type)
{
    for (int i = 0; i < eventDoorCount_; i++) {
        if (uid == eventDoor_[i].uid) {
            eventDoor_[i].type = type;
            return;
        }
    }
    eventDoor_[eventDoorCount_].uid = uid;
    eventDoor_[eventDoorCount_].type = type;
    eventDoorCount_++;
}

ARM void TownDoorAction::scriptOpen(int uid1, int uid2, int type)
{
    int obj1;
    int obj2;
    scriptDoor1Uid_ = uid1;
    scriptDoor2Uid_ = uid2;
    obj1 = TownStageManager::getSingleton()->getObjectIDfromMapUid(scriptDoor1Uid_);
    if (scriptDoor2Uid_ != 0) {
        obj2 = TownStageManager::getSingleton()->getObjectIDfromMapUid(scriptDoor2Uid_);
    }
    switch (type) {
    case 0:
        scriptType_ = DOOR_OPEN;
        TownStageManager::getSingleton()->commonAnim(obj1, 2);
        setDoorFlag(scriptDoor1Uid_, scriptType_, true);
        if (uid2 == 0) {
            return;
        }
        setDoorFlag(scriptDoor2Uid_, DOOR_OPEN, true);
        TownStageManager::getSingleton()->commonAnim(obj2, 4);
        break;
    case 1:
        scriptType_ = DOOR_OPEN;
        setDoorFlag(scriptDoor1Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj1, 4);
        if (uid2 == 0) {
            return;
        }
        setDoorFlag(scriptDoor2Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj2, 2);
        break;
    case 2:
        scriptType_ = DOOR_CLOSE;
        setDoorFlag(scriptDoor1Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj1, 7);
        if (uid2 == 0) {
            return;
        }
        setDoorFlag(scriptDoor2Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj2, 6);
        break;
    case 3:
        scriptType_ = DOOR_CLOSE;
        setDoorFlag(scriptDoor1Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj1, 6);
        if (uid2 == 0) {
            return;
        }
        setDoorFlag(scriptDoor2Uid_, scriptType_, true);
        TownStageManager::getSingleton()->commonAnim(obj2, 7);
        break;
    case 4:
        scriptType_ = DOOR_OPEN;
        TownStageManager::getSingleton()->commonAnim(obj1, 1);
        setDoorFlag(scriptDoor1Uid_, scriptType_, false);
        if (scriptDoor2Uid_ != 0) {
            TownStageManager::getSingleton()->commonAnim(obj2, 1);
            setDoorFlag(scriptDoor2Uid_, scriptType_, false);
        }
        scriptDoor1Uid_ = 0;
        scriptDoor2Uid_ = 0;
        break;
    case 5:
        scriptType_ = DOOR_CLOSE;
        TownStageManager::getSingleton()->commonAnim(obj1, 1);
        setDoorFlag(scriptDoor1Uid_, scriptType_, false);
        if (scriptDoor2Uid_ != 0) {
            TownStageManager::getSingleton()->commonAnim(obj2, 1);
            setDoorFlag(scriptDoor2Uid_, scriptType_, false);
        }
        scriptDoor1Uid_ = 0;
        scriptDoor2Uid_ = 0;
        break;
    }
}

ARM bool TownDoorAction::scriptEnd()
{
    bool ret = true;
    if (scriptDoor1Uid_ != 0) {
        if (TownStageManager::getSingleton()->isCommonAnimationEnd(scriptDoor1Uid_) == false) {
            ret = false;
        } else {
            if (scriptType_ == DOOR_OPEN) {
                TownFurnitureControlManager::getSingleton()->setFurnitureFade(scriptDoor1Uid_, 0xf, 1, 0);
            }
            scriptDoor1Uid_ = 0;
        }
    }
    if (scriptDoor2Uid_ != 0) {
        if (TownStageManager::getSingleton()->isCommonAnimationEnd(scriptDoor2Uid_) == false) {
            ret = false;
        } else {
            if (scriptType_ == DOOR_OPEN) {
                TownFurnitureControlManager::getSingleton()->setFurnitureFade(scriptDoor2Uid_, 0xf, 1, 0);
            }
            scriptDoor2Uid_ = 0;
        }
    }
    return ret;
}

ARM void TownDoorAction::setDoorFlag(int uid, int type, bool flag)
{
    TownStageManager::getSingleton()->setCollision(uid, 0);
    int obj = TownStageManager::getSingleton()->getObjectIDfromMapUid(uid);
    if (type == DOOR_OPEN) {
        TownStageManager::getSingleton()->collEraseMapUid(uid);
        TownFurnitureManager::getSingleton()->openDoor(uid);
        TownStageManager::getSingleton()->stage_.setAlpha(obj, 0x1f);
        if (flag == false) {
            TownStageManager::getSingleton()->setCollision(uid, 1);
        }
    }
    if (type == DOOR_CLOSE) {
        if (flag == true) {
            TownFurnitureControlManager::getSingleton()->setFurnitureFade(uid, 0xf, 0, 0);
            TownStageManager::getSingleton()->stage_.setAlpha(obj, 0);
        } else {
            TownStageManager::getSingleton()->stage_.setAlpha(obj, 0x1f);
        }
        TownStageManager::getSingleton()->setCollisionObject(uid);
        TownFurnitureManager::getSingleton()->closeDoor(uid);
    }
}

