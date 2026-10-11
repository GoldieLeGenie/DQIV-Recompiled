#pragma ipa file
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownCamera.hpp"
#include "main/cmn/CommonCalculate.hpp"
#include "main/dss/Camera.hpp"
#include <nitro/fx/fx_atan.h>
#include "main/dss/UnkMatrix43.hpp"

const dss::Fix32 TownActionCalculate::cos_PI_6(0xddb);

ARM bool TownActionCalculate::IntersectRaySphere(dss::Fix32Vector3& p, dss::Fix32Vector3& d, dss::Fix32Vector3& sc, dss::Fix32 r, dss::Fix32& t, dss::Fix32Vector3& q)
{
    dss::Fix32Vector3 m = p - sc;
    dss::Fix32 b = m * d;
    dss::Fix32 c = m * m - r * r;
    if (c > dss::Fix32(0L) && b > dss::Fix32(0L)) {
        return false;
    }
    dss::Fix32 discr = b * b - c;
    if (discr < dss::Fix32(0L)) {
        return false;
    }
    t = 0L;
    t -= b;
    t -= discr.sqrt();
    if (t < dss::Fix32(0L)) {
        t = 0L;
    }
    q = p + d * t;
    return true;
}

ARM bool TownActionCalculate::crossCheck(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32Vector3& target, dss::Fix32 radius)
{
    dss::Fix32Vector3 vec = target - nextPos;
    dss::Fix32 r2;
    r2.value = radius.value << 1;
    static const dss::Fix32 unusedR(0x7b);
    if (vec.lengthsq() < r2 * r2) {
        vec.normalize();
        dss::Fix32Vector3 ret;
        dss::Fix32Vector3 dir = nextPos - nowPos;
        dir.normalize();
        dss::Fix32 dot = dir * vec;
        dss::Fix32 t;
        if (dot.value > 0x92d) {
            if (IntersectRaySphere(nowPos, dir, target, r2, t, ret)) {
                if (nextPos.length(ret) <= radius) {
                    nextPos = ret;
                } else {
                    nextPos = nowPos;
                }
            }
        } else {
            nextPos = target - (vec * radius) * 2;
        }
        return true;
    }
    return false;
}

ARM void TownActionCalculate::townCharaColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 r, int surfaceId, int objectId, int polyNo, dss::Fix32 ctrLen, int menuFlag)
{
    dss::Fix32Vector3 vec;
    int farTalk = 0;
    int frontPoly = -1;
    fld::FLDObject* fld = &TownStageManager::getSingleton()->stage_.m_fld;
    int commonId = -1;
    if (objectId != -1) {
        commonId = fld->GetMapObjCommonId(objectId);
    }
    if (surfaceId != -1) {
        vec = TownStageManager::getSingleton()->getHitSurfaceDirByType(12);
        vec = vec * -1;
        ctrLen.value += 600;
        farTalk = 1;
    } else if (commonId != -1) {
        if (TownStageManager::getSingleton()->getSearchPolyDirection(vec) == 1) {
            vec *= -1;
        } else {
            commonId = -1;
            vec.set(0, 0, 0);
        }
        switch (commonId) {
        case 0x54:
            ctrLen.value += 1500;
            farTalk = 1;
            break;
        case 0x3c:
            ctrLen.value += 1000;
            farTalk = 1;
            break;
        case 0xf9:
            farTalk = 1;
            break;
        case 10:
        case 0x18:
        case 0xf4:
            TownCharacterManager::getSingleton()->checkObjectInTalk(objectId);
            frontPoly = TownStageManager::getSingleton()->coll_.getFrontPoly(polyNo, objectId);
            break;
        case 5:
        case 6:
        case 7:
        case 0x63:
        case 0x76:
        case 0x10d:
        case 0x10e:
            TownCharacterManager::getSingleton()->checkObjectInTalk(objectId);
            break;
        }
    }
    TownCharacterManager::getSingleton()->setCheckArea(1);
    TownCharacterManager::getSingleton()->characterColl(nowPos, nextPos, vec, r, ctrLen, frontPoly, farTalk, menuFlag);
}

ARM int TownActionCalculate::townStageColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 srufaceRad, dss::Fix32 preR)
{
    static const dss::Fix32 dv(0x8f);
    static const dss::Fix32 dvObj(0x17d);
    int ret = -1;
    dss::Fix32 height;
    dss::Fix32Vector3 temp;
    dss::Fix32Vector3 vec;
    temp = TownStageManager::getSingleton()->compute(nowPos, nextPos, radius, srufaceRad, preR, height);
    nextPos = temp;
    if (height < TownPlayerAction::fallH * -1) {
        ret = 3;
    } else {
        nextPos.vy += height;
    }
    vec = nextPos - nowPos;
    if (vec.lengthsq() < dv * dv) {
        nextPos = nowPos;
    } else {
        int collId = TownStageManager::getSingleton()->coll_.m_id;
        int objectId = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, collId);
        int surfaceId = coll_GetSurface(TownStageManager::getSingleton()->stage_.m_fld.m_coll, collId);
        int mapUid = TownStageManager::getSingleton()->stage_.m_fld.GetMapObjUid(objectId);
        if (surfaceId != -1 || mapUid != 0) {
            if (vec.lengthsq() < dvObj * dvObj) {
                nextPos = nowPos;
            }
        }
    }
    return ret;
}

ARM void TownActionCalculate::townShipStageColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 surfaceRad, dss::Fix32 preR)
{
    dss::Fix32 height;
    dss::Fix32Vector3 temp;
    dss::Fix32Vector3 vec;
    static const dss::Fix32 unusedA(0x1000);
    static const dss::Fix32 unusedB(0x400);
    static const dss::Fix32 dv(0x8f);
    static const dss::Fix32 dvObj(0x17d);
    temp = TownStageManager::getSingleton()->compute(nowPos, nextPos, radius, surfaceRad, preR, height);
    nextPos = temp;
    nextPos.vy += height;
    vec = temp - nowPos;
}

ARM bool TownActionCalculate::directionCheckByPosition(dss::Fix32Vector3& pos, dss::Fix32Vector3& target, short idx, int value)
{
    dss::Fix32Vector3 dir;
    dss::Fix32Vector3 vec;
    getDirByIdx(idx, dir);
    vec = target - pos;
    vec.vy = 0L;
    vec.normalize();
    return (vec * dir).value >= value;
}

ARM void TownActionCalculate::getDirByIdx(short dirIdx, dss::Fix32Vector3& dir)
{
    cmn::CommonCalculate::getDirByIdx(dirIdx, dir);
}

ARM void TownActionCalculate::getIdxByVec(short& idx, dss::Fix32Vector3& dir)
{
    if (dir.vx == dss::Fix32(0L) && dir.vz == dss::Fix32(0L)) {
        return;
    }
    idx = FX_Atan2Idx(dir.vx.value, dir.vz.value);
}

ARM int TownActionCalculate::searchPairWdoor(int objectId, dss::Fix32Vector3* door1, dss::Fix32Vector3* door2)
{
    fld::FLDObject* fld;
    bool ret = false;
    static const dss::Fix32 wDoorlength(0x2000);
    int retObjId = -1;
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 posDoor1;
    int objectId2;
    int commonId2;
    int mapUid1;
    int commonId1;
    dss::Fix32Vector3 posDoor2;
    fld = &TownStageManager::getSingleton()->stage_.m_fld;
    commonId1 = -1;
    if (objectId != -1) {
        commonId1 = fld->GetMapObjCommonId(objectId);
    }
    mapUid1 = TownStageManager::getSingleton()->stage_.m_fld.GetMapObjUid(objectId);
    TownStageManager::getSingleton()->getObjectPos(objectId, 0, &posDoor1);
    objectId2 = TownStageManager::getSingleton()->getObjectIDfromMapUid(mapUid1 - 1);
    fld = &TownStageManager::getSingleton()->stage_.m_fld;
    commonId2 = -1;
    if (objectId2 != -1) {
        commonId2 = fld->GetMapObjCommonId(objectId2);
    }
    if (commonId1 == commonId2) {
        TownStageManager::getSingleton()->getObjectPos(objectId2, 0, &posDoor2);
        vec = posDoor2 - posDoor1;
        if (vec.lengthsq() < wDoorlength * wDoorlength) {
            retObjId = objectId2;
            ret = true;
        }
    }
    if (!ret) {
        objectId2 = TownStageManager::getSingleton()->getObjectIDfromMapUid(mapUid1 + 1);
        fld = &TownStageManager::getSingleton()->stage_.m_fld;
        commonId2 = -1;
        if (objectId2 != -1) {
            commonId2 = fld->GetMapObjCommonId(objectId2);
        }
        if (commonId1 == commonId2) {
            TownStageManager::getSingleton()->getObjectPos(objectId2, 0, &posDoor2);
            vec = posDoor2 - posDoor1;
            if (vec.lengthsq() < wDoorlength * wDoorlength) {
                retObjId = objectId2;
            }
        }
    }
    if (door1) {
        *door1 = posDoor1;
    }
    if (door2) {
        *door2 = posDoor2;
    }
    return retObjId;
}

ARM bool TownActionCalculate::checkTalking(dss::Fix32Vector3& pos, short dirIdx, int objectId)
{
    if (TownCamera::getSingleton()->flagRotateL != 0) {
        return false;
    }
    if (TownCamera::getSingleton()->flagRotateR != 0) {
        return false;
    }
    if (TownPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BENRI_BUTTON) {
        if (objectId != -1) {
            int charaNo;
            if (TownExtraCollManager::getSingleton()->isExtraCollChara(objectId, charaNo) == TownCharacterBase::TOWN_CHARACTER_MONSTER) {
                TownCharacterManager::getSingleton()->setTalked(charaNo, 1);
                TownCharacterManager::getSingleton()->setTalkedArea(charaNo, 1);
                TownPlayerManager::getSingleton()->setPlayerCommand(START_TALK_COMMAND);
                return true;
            }
        }
        if (TownCharacterManager::getSingleton()->checkTalkingNearCharacter(pos, dirIdx, -1) == 1) {
            TownPlayerManager::getSingleton()->setPlayerCommand(START_TALK_COMMAND);
            return true;
        }
    }
    return false;
}

ARM bool TownActionCalculate::checkIkadaTalk(dss::Fix32Vector3 pos, short dirIdx, int surfaceId, int polyId, int menuSearch)
{
    static const dss::Fix32 len(0x14cd);
    if (TownCamera::getSingleton()->flagRotateL != 0) {
        return false;
    }
    if (TownCamera::getSingleton()->flagRotateR != 0) {
        return false;
    }
    if (TownPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BENRI_BUTTON || menuSearch == 1) {
        dss::Fix32Vector3 vec;
        dss::Fix32Vector3 target;
        getDirByIdx(dirIdx, vec);
        target = pos + vec * len;
        if (surfaceId != -1) {
            dss::Fix32Vector3 dir;
            TownStageManager::getSingleton()->getPolyDirection(dir, polyId);
            if (((dir * vec) * -1).value < 0xb50) {
                return false;
            }
            if (TownCharacterManager::getSingleton()->checkIkadaTalk(target, dir) == 1) {
                TownPlayerManager::getSingleton()->setPlayerCommand(START_TALK_COMMAND);
                return true;
            }
        }
    }
    return false;
}

ARM void TownActionCalculate::normalMove(dss::Fix32Vector3& position, short& dirIdx, dss::Fix32 speed)
{
    if (TownPlayerManager::getSingleton()->player_.padInput_ == 0) {
        return;
    }
    unsigned short dirInput = TownPlayerManager::getSingleton()->player_.dirInput_;
    dss::Fix32Vector3 dir(TownCamera::getSingleton()->camera_.unk_004.getDirection());
    dir.vy = 0L;
    dir.normalize();
    dss::UnkMatrix43 mtx;
    mtx.unkfunc_020886d0(-dirInput);
    dir = mtx * dir;
    dir.vy = 0L;
    position = position + dir * speed;
    dirIdx = 0x8000 - dirInput + TownCamera::getSingleton()->camera_.unk_004.getAngle().vy;
    dirIdx = (unsigned short&)dirIdx;
}

ARM bool TownActionCalculate::checkGetOnShipAndIkada(const dss::Fix32Vector3& nextPos, const dss::Fix32Vector3& pos, short dirIdx, dss::Fix32 length)
{
    bool ret = false;
    static const dss::Fix32 dot(0xa66);
    dss::Fix32Vector3 vec = pos - nextPos;
    dss::Fix32Vector3 playerDir;
    getDirByIdx(dirIdx, playerDir);
    vec.vy.value = 0;
    if (vec.lengthsq() < length * length) {
        vec.normalize();
        playerDir.normalize();
        if (playerDir * vec > dot) {
            ret = true;
        }
    }
    return ret;
}

ARM bool TownActionCalculate::checkGetDownShipAndIkada(dss::Fix32Vector3& nextPos, short dirIdx, dss::Fix32Vector3& targetPos, dss::Fix32Vector3& surfaceDir, dss::Fix32Vector3& surfacePos, dss::Fix32 downL)
{
    bool ret = false;
    static const dss::Fix32 len(0x2333);
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 playerDir;
    dss::Fix32Vector3 checkPos;
    dss::Fix32 length;
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(10) != -1) {
        getDirByIdx(dirIdx, playerDir);
        int surfacePoly = TownStageManager::getSingleton()->coll_.m_id;
        if (surfacePoly == -1) {
            return false;
        }
        int surfaceId = coll_GetSurface(TownStageManager::getSingleton()->stage_.m_fld.m_coll, surfacePoly);
        if (surfaceId == -1 || (surfaceId & 0xf000) != 0xa000) {
            return false;
        }
        TownStageManager::getSingleton()->getPolyDirection(surfaceDir, surfacePoly);
        TownStageManager::getSingleton()->stage_.collGetPolygonPos(surfacePoly, &surfacePos);
        if (!TownStageManager::getSingleton()->isPolyFacePosition(surfacePoly, nextPos, len)) {
            return false;
        }
        surfaceDir.normalize();
        playerDir.normalize();
        if ((surfaceDir * playerDir).value > -0x800) {
            return false;
        }
        vec = nextPos - surfacePos;
        vec.vy = 0L;
        length = vec * surfaceDir;
        length.value = unkfunc_02031e84(length.value);
        length += downL;
        checkPos = nextPos + (surfaceDir * -1) * length;
        dss::Fix32 fy;
        dss::Fix32Vector3 tempPos;
        dss::Fix32Vector3 tnext;
        tnext = nextPos;
        tnext.vy += TownPlayerAction::collR;
        checkPos.vy += TownPlayerAction::collR;
        dss::Fix32 height(checkPos.vy);
        if (TownStageManager::getSingleton()->checkCrossPolygon(tnext, checkPos, surfacePoly) == 1) {
            targetPos = checkPos;
            tempPos = TownStageManager::getSingleton()->compute(targetPos, targetPos, TownPlayerAction::collR, TownPlayerAction::collR, TownPlayerAction::townCharaPreR, fy);
            tnext = nextPos;
            tnext.vy = tempPos.vy;
            if (tempPos.vx == targetPos.vx && tempPos.vz == targetPos.vz) {
                townCharaColl(tempPos, tempPos, TownPlayerAction::townCharaR, -1, -1, -1, TownPlayerAction::walkCtrLen, 0);
                if (tempPos.vx == targetPos.vx && tempPos.vz == targetPos.vz) {
                    tnext.vy = targetPos.vy = height;
                    int polyNo;
                    if (TownStageManager::getSingleton()->checkCrossNumEraseSurface(tnext, targetPos, 0xc000, 1, polyNo) == 2) {
                        targetPos.vy += fy - TownPlayerAction::collR;
                        ret = true;
                    }
                }
            }
        }
    }
    return ret;
}

ARM int TownActionCalculate::getParamDir4ByIdx(short idx)
{
    if (idx < 0x2000 && idx > -0x2000) {
        return 0;
    }
    if (idx >= 0x2000 && idx < 0x6000) {
        return 1;
    }
    if (idx >= -0x6000 && idx < -0x2000) {
        return 3;
    }
    return 2;
}

ARM dss::Fix32Vector3 TownActionCalculate::getParamVec(unsigned char dir)
{
    dss::Fix32Vector3 retDir;
    retDir.set(0, 0, 0);
    switch (dir) {
    case 0:
        retDir.vz.value = 0x1000;
        break;
    case 1:
        retDir.vx.value = 0x1000;
        break;
    case 2:
        retDir.vz.value = -0x1000;
        break;
    case 3:
        retDir.vx.value = -0x1000;
        break;
    }
    return retDir;
}

ARM short TownActionCalculate::getIdxByParam(unsigned char dir)
{
    return cmn::CommonCalculate::getIdxByParam(dir);
}

ARM bool TownActionCalculate::checkLineOver(dss::Fix32Vector3& pos, dss::Fix32Vector3& linePos, dss::Fix32Vector3 normal)
{
    bool ret = false;
    dss::Fix32Vector3 vec = pos - linePos;
    dss::Fix32 dot = vec * normal;
    if (dot >= dss::Fix32(0L)) {
        ret = true;
    }
    return ret;
}

ARM void TownActionCalculate::setAngle(char axisNo, short idx, dss::Vector3<short>& angle)
{
    angle.set(0, 0, 0);
    switch (axisNo) {
    case 0:
        angle.vx = idx;
        break;
    case 1:
        angle.vy = idx;
        break;
    case 2:
        angle.vz = idx;
        break;
    }
}

ARM bool TownActionCalculate::checkGetDownIkada(dss::Fix32Vector3& nextPos, short dirIdx, dss::Fix32Vector3& targetPos)
{
    bool ret = false;
    static const dss::Fix32 downL(0x6b8);
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 playerDir;
    dss::Fix32Vector3 surfacePos;
    dss::Fix32Vector3 surfaceDir;
    dss::Fix32Vector3 checkPos;
    dss::Fix32 length;
    if (TownStageManager::getSingleton()->getHitSurfaceIdByType(10) != -1) {
        getDirByIdx(dirIdx, playerDir);
        surfaceDir = TownStageManager::getSingleton()->getHitSurfaceDirByType(10);
        surfacePos = TownStageManager::getSingleton()->getHitSurfacePosByType(10);
        int surfacePoly = TownStageManager::getSingleton()->coll_.m_id;
        surfaceDir.normalize();
        playerDir.normalize();
        if ((surfaceDir * -1) * playerDir < cos_PI_6) {
            return false;
        }
        vec = nextPos - surfacePos;
        vec.vy = 0L;
        length = vec * surfaceDir;
        length.value = unkfunc_02031e84(length.value);
        length += downL;
        checkPos = nextPos + (surfaceDir * -1) * length;
        dss::Fix32 fy;
        dss::Fix32Vector3 tempPos;
        dss::Fix32Vector3 tnext;
        tnext = nextPos;
        tnext.vy += TownPlayerAction::collR;
        checkPos.vy += TownPlayerAction::collR;
        dss::Fix32 height(checkPos.vy);
        if (TownStageManager::getSingleton()->checkCrossPolygon(tnext, checkPos, surfacePoly) == 1) {
            targetPos = checkPos;
            tempPos = TownStageManager::getSingleton()->compute(targetPos, targetPos, TownPlayerAction::collR, TownPlayerAction::collR, TownPlayerAction::collR * 2, fy);
            tnext = nextPos;
            tnext.vy = tempPos.vy;
            if ((tempPos == targetPos)) {
                townCharaColl(tempPos, tempPos, TownPlayerAction::townCharaR, -1, -1, -1, TownPlayerAction::walkCtrLen, 0);
                if ((tempPos == targetPos)) {
                    tnext.vy = targetPos.vy = height;
                    int polyNo;
                    if (TownStageManager::getSingleton()->checkCrossNumEraseSurface(tnext, targetPos, 0xc000, 1, polyNo) == 2) {
                        targetPos.vy += fy - TownPlayerAction::collR;
                        ret = true;
                    }
                }
            }
        }
    }
    return ret;
}
