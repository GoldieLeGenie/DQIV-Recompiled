#pragma ipa file
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/status/StageStatus.hpp"
#include "main/global/Global.hpp"
#include "main/encount/Encount.hpp"
#include "main/text/TextAPI.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/ScriptSystem.hpp"
#include "ov000/town/TownExtraMapObjManager.hpp"


// reproduces an extra 0.43f item of the original .data layout.
ARM dss::Fix32 TownStageManager::unkfunc_dataPad0()
{
    return dss::Fix32(0.43f);
}

static dss::Fix32 radius(1.5f);
static const char* TOWN_MAP_PATH = "data/map";

ARM TownStageManager::TownStageManager()
{
}

ARM TownStageManager::~TownStageManager()
{
}

ARM TownStageManager* TownStageManager::getSingleton()
{
    static TownStageManager m_singleton;
    return &m_singleton;
}

ARM void TownStageManager::initialize()
{
    coll_.resetEraseSurface();
    stage_.setRender(&TownSystem::getSingleton()->render_);
    stage_.setPath(TOWN_MAP_PATH);
    stage_.load(g_Global.getMapName());
    allocFlag_ = 1;
    stopScript_ = 0;
    TownExtraCollManager::getSingleton()->setup();
    g_Stage.setup(g_Global.getMapName());
    stage_.setup();
    stage_.setFldColl(&coll_);
    TownCamera::getSingleton()->setLimitL(stage_.getCameraLimitL());
    TownCamera::getSingleton()->setLimitR(stage_.getCameraLimitR());
    if (!g_Stage.isCameraIcon()) {
        TownCamera::getSingleton()->resetAngle();
    }
    townData_.initialize();
    loadStage(g_Global.getMapName());
    for (int i = 0; i < 4; i++) {
        softEraseObjId_[i] = -1;
    }
    softEraseNum_ = 0;
    setClipping(1);
    cmn::CommonEffectLocation::getSingleton()->initialize();
}

ARM void TownStageManager::terminate()
{
    cmn::CommonEffectLocation::getSingleton()->terminate();
    stage_.cleanup();
    stage_.terminate();
    g_Stage.cleanup();
    townData_.terminate();
    mapEffect_.cleanup();
}

ARM void TownStageManager::execute()
{
    mapEffect_.execute();
    cmn::CommonEffectLocation::getSingleton()->execute();
    execSoftErase();
    if (stopScript_ == 1) {
        stopScript_ = 0;
        TownSystem::getSingleton()->scriptLock_ = 0;
    }
    if (encount::Encount::getSingleton()->disableFlag_) {
        if (TownWindowSystem::getSingleton()->isOpen() == 1) {
            return;
        }
        encount::Encount::getSingleton()->disableFlag_ = 0;
        switch (encount::Encount::getSingleton()->disableAction_) {
        case 0xa3:
            TextAPI::setMACRO0(10, 0x40000000, 0x71);
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openMessage(0xc3d90, 1);
            TownSystem::getSingleton()->scriptLock_ = 1;
            stopScript_ = 1;
            break;
        case 0xcf:
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openMessage(0xc3d92, 1);
            TownSystem::getSingleton()->scriptLock_ = 1;
            stopScript_ = 1;
            break;
        case 0xd7:
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openMessage(0xc3d94, 1);
            TownSystem::getSingleton()->scriptLock_ = 1;
            stopScript_ = 1;
            break;
        }
    }
    if (encount::Encount::getSingleton()->easyFlag_ == 0) {
        return;
    }
    encount::Encount::getSingleton()->easyFlag_ = 0;
    if (encount::Encount::getSingleton()->disableAction_ != 0xa6) {
        return;
    }
    TextAPI::setMACRO0(10, 0x40000000, 0x74);
    ui_MsgSndSet(0x30);
    TownWindowSystem::getSingleton()->openMessage(0xc3d90, 1);
    TownSystem::getSingleton()->scriptLock_ = 1;
    stopScript_ = 1;
}

ARM void TownStageManager::draw()
{
    mapEffect_.draw();
}

ARM bool TownStageManager::isStageExist(char* name)
{
    char path[128];
    dss::sprintf_s(path, sizeof(path), "%s/%s.lz", TOWN_MAP_PATH, name);
    return dss::g_File.isExist(path) != 0;
}

ARM dss::Fix32Vector3 TownStageManager::compute(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 surfaceRad, dss::Fix32 preR, dss::Fix32& height)
{
    short dirIdx = TownPlayerManager::getSingleton()->getDirection();
    dss::Fix32Vector3 dir;
    TownActionCalculate::getDirByIdx(dirIdx, dir);
    coll_.m_playerDir = dir;
    return coll_.compute(nowPos, nextPos, radius, surfaceRad, preR, height);
}

ARM char* TownStageManager::getLinkMapName()
{
    if (getExitIndex() != -1) {
        char* name = StageLink::getName(g_Global.getMapName(), getExitIndex());
        if (name) {
            return name;
        }
    }
    return 0;
}

ARM void TownStageManager::setExitPosition(dss::Fix32Vector3* pos, int index)
{
    coll_.setExitPosition(pos, index);
}

ARM dss::Fix32Vector3 TownStageManager::getMapUidPos(int uid)
{
    dss::Fix32Vector3 ret;
    VecFx32 pos = stage_.getUidPos(uid);
    ret.vx.value = pos.x;
    ret.vy.value = pos.y;
    ret.vz.value = pos.z;
    return ret;
}

ARM void TownStageManager::addMapUidPosFX32(int uid, dss::Fix32Vector3& pos)
{
    VecFx32 vec;
    vec.x = pos.vx.value;
    vec.y = pos.vy.value;
    vec.z = pos.vz.value;
    stage_.m_fld.CollAddPolyPosByMapUid(uid, &vec);
    stage_.m_fld.AddMapUidPosFX32(uid, &vec);
}

ARM void TownStageManager::setMapUidPosFX32(int uid, dss::Fix32Vector3& pos)
{
    VecFx32 vec;
    vec.x = pos.vx.value;
    vec.y = pos.vy.value;
    vec.z = pos.vz.value;
    stage_.m_fld.SetMapUidPosFX32(uid, &vec);
}

ARM dss::Fix32Vector3 TownStageManager::getRiseupPos(int uid, int type)
{
    dss::Fix32Vector3 ret;
    int* obj = stage_.GetMapUidObj(uid);
    switch (type) {
    case 7:
        for (int i = 0; i < stage_.pool_counter; i++) {
            int commonId = obj[i] == -1 ? -1 : stage_.m_fld.GetMapObjCommonId(obj[i]);
            if (commonId == 0xf5) {
                getObjectPos(obj[i], 0, &ret);
                return ret;
            }
        }
        break;
    case 0x1c:
        for (int i = 0; i < stage_.pool_counter; i++) {
            int commonId = obj[i] == -1 ? -1 : stage_.m_fld.GetMapObjCommonId(obj[i]);
            if (commonId == 0xe9) {
                getObjectPos(obj[i], 0, &ret);
                return ret;
            }
        }
        break;
    case 0x11:
        for (int i = 0; i < stage_.pool_counter; i++) {
            int commonId = obj[i] == -1 ? -1 : stage_.m_fld.GetMapObjCommonId(obj[i]);
            if (commonId == 0x61) {
                getObjectPos(obj[i], 0, &ret);
                return ret;
            }
        }
        break;
    case 0x2c:
        TownExtraMapObjManager::getSingleton()->getPosition(uid, ret);
        break;
    default:
        ret = TownStageManager::getSingleton()->getMapUidPos(uid);
        break;
    }
    return ret;
}

ARM void TownStageManager::loadStage(char* name)
{
    townData_.loadStage(name);
    if (data_0211d434 != 0) {
        return;
    }
    VecFx32 rate;
    rate.x = townData_.rate_.vx.value;
    rate.y = townData_.rate_.vy.value;
    rate.z = townData_.rate_.vz.value;
    stage_.m_fld.SetRGBRate(&rate, 0);
}

ARM void TownStageManager::setObjectDraw(int uid, int flag, int frame)
{
    if (flag == 1) {
        stage_.repop(uid);
    }
    stage_.animLocation(uid, flag, frame);
}

ARM int TownStageManager::getObjectIDfromMapUid(int mapUid)
{
    return *stage_.GetMapUidObj(mapUid);
}

ARM void TownStageManager::rotObjectUid(int uid, short rot)
{
    dss::Fix32Vector3 vec;
    vec *= 0;
    vec.vy.value = rot;
    stage_.setRotObjectUid(uid, vec);
}

ARM void TownStageManager::SetRGBRate(dss::Fix32Vector3& rate, int flag)
{
    VecFx32 vec;
    vec.x = rate.vx.value;
    vec.y = rate.vy.value;
    vec.z = rate.vz.value;
    stage_.m_fld.SetRGBRate(&vec, flag);
    townData_.rate_ = rate;
}

ARM void TownStageManager::setTextureScaling(fx32 scaleX, fx32 scaleY)
{
    mapEffect_.unkfunc_02142850(scaleX);
    mapEffect_.unkfunc_02142854(scaleY);
}

ARM void TownStageManager::getTextureScaling(fx32& scaleX, fx32& scaleY)
{
    scaleX = mapEffect_.scaleX_;
    scaleY = mapEffect_.scaleY_;
}

ARM void TownStageManager::setEffect(TownMapEffect::EFFECT_TYPE type)
{
    mapEffect_.setup(type);
}

ARM bool TownStageManager::addBoxCollision(dss::Fix32Vector3& center, dss::Fix32Vector3& vec, int& extraId)
{
    switch (stage_.addBoxCollistion(center, vec, extraId, allocFlag_)) {
    case MEMORY_ALLOC_ERROR:
    case ALLOC_MAX_OVER_ERROR:
    case NUM_MAX_OVER_ERROR:
        return false;
    }
    return true;
}

ARM int TownStageManager::getOtherPolyNoBySurfaceId(int surface, int polyNo)
{
    int no = stage_.getPolyNoBySurfaceId(surface, 0);
    while (no != -1) {
        if (no != polyNo) {
            return no;
        }
        no = stage_.getPolyNoBySurfaceId(surface, no + 1);
    }
    return -1;
}

ARM dss::Fix32Vector3 TownStageManager::getSurfaceDir(int surface)
{
    dss::Fix32Vector3 ret;
    COLL_POLY poly;
    stage_.collGetPoly(coll_GetPolyNoBySurface((COLL_HEADER*)stage_.m_fld.m_coll, surface, 0), &poly);
    ret = FldStage::getFx32Vector3(poly.normal);
    return ret;
}

ARM bool TownStageManager::checkCrossPolygon(dss::Fix32Vector3 pos0, dss::Fix32Vector3 pos1, int polyNo)
{
    VecFx32 vec0;
    vec0 = FldStage::getVecFx32(pos0);
    VecFx32 vec1;
    vec1 = FldStage::getVecFx32(pos1);
    return coll_.checkCrossPolygon(vec0, vec1, polyNo);
}

ARM int TownStageManager::checkCrossNumCheckUnder(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int flag)
{
    VecFx32 vec0;
    vec0 = FldStage::getVecFx32(pos0);
    VecFx32 vec1;
    vec1 = FldStage::getVecFx32(pos1);
    return coll_.checkCrossNumCheckUnder(vec0, vec1, flag);
}

ARM int TownStageManager::checkCrossNum(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int flag)
{
    VecFx32 vec0;
    vec0 = FldStage::getVecFx32(pos0);
    VecFx32 vec1;
    vec1 = FldStage::getVecFx32(pos1);
    return coll_.checkCrossNum(vec0, vec1, flag);
}

ARM int TownStageManager::checkCrossNumEraseSurface(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int surface, int flag, int& polyNo)
{
    VecFx32 vec0;
    vec0 = FldStage::getVecFx32(pos0);
    VecFx32 vec1;
    vec1 = FldStage::getVecFx32(pos1);
    return coll_.checkCrossNumEraseSurface(vec0, vec1, surface, flag, polyNo);
}

ARM bool TownStageManager::getSearchPolyDirection(dss::Fix32Vector3& dir)
{
    return getPolyDirection(dir, coll_.getSearchPolyNo());
}

ARM bool TownStageManager::getPolyDirection(dss::Fix32Vector3& dir, int polyNo)
{
    COLL_POLY poly;
    if (TownStageManager::getSingleton()->stage_.collGetPoly(polyNo, &poly) == -1) {
        return false;
    }
    dir.vx.value = poly.normal.x;
    dir.vy.value = poly.normal.y;
    dir.vz.value = poly.normal.z;
    return true;
}

ARM int TownStageManager::getHitSurfaceIdByType(int type)
{
    return coll_.getSurfaceByType(type);
}

ARM dss::Fix32Vector3 TownStageManager::getHitSurfaceDirByType(int type)
{
    dss::Fix32Vector3 ret;
    ret.set(0, 0, 0);
    getPolyDirection(ret, coll_.m_surfacePolyNo[type]);
    return ret;
}

ARM void TownStageManager::setSoftErase(int objectId)
{
    if (softEraseNum_ < 4) {
        softEraseObjId_[softEraseNum_] = objectId;
        softEraseNum_++;
    }
}

ARM void TownStageManager::execSoftErase()
{
    for (int i = 0; i < 4; i++) {
        if (softEraseObjId_[i] != -1) {
            int alpha = stage_.m_fld.GetMapObjAlpha(softEraseObjId_[i]) - 2;
            if (alpha <= 0) {
                stage_.eraseObject(stage_.m_fld.GetMapObjUid(softEraseObjId_[i]), 1);
                softEraseObjId_[i] = -1;
                softEraseNum_--;
            } else {
                stage_.setAlpha(softEraseObjId_[i], alpha);
            }
        }
    }
}

ARM int TownStageManager::isEndSoftErase(int objectId)
{
    int ret = 1;
    for (int i = 0; i < 4; i++) {
        if (objectId == softEraseObjId_[i]) {
            ret = 0;
        }
    }
    return ret;
}

ARM dss::Fix32Vector3 TownStageManager::getHitSurfacePosByType(int type)
{
    dss::Fix32Vector3 ret;
    ret.set(0, 0, 0);
    stage_.collGetPolygonPos(coll_.m_surfacePolyNo[type], &ret);
    return ret;
}

ARM int TownStageManager::getExitIndex()
{
    int polyNo = -1;
    int id = coll_.getSurfaceByType(5);
    if (id != -1) {
        return id;
    }
    id = coll_.getSurfaceByType(1);
    if (id != -1) {
        polyNo = coll_.m_surfacePolyNo[1];
    } else {
        id = coll_.getSurfaceByType(7);
        if (id != -1) {
            polyNo = coll_.m_surfacePolyNo[7];
        }
    }
    if (id == -1) {
        return -1;
    }
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    if (isPolyFacePosition(polyNo, pos) != 1) {
        id = -1;
    }
    return id;
}

// reproduces an extra 0L item of the original .data layout.
ARM dss::Fix32 TownStageManager::unkfunc_dataPad1()
{
    return dss::Fix32(0L);
}

ARM bool TownStageManager::isPolyFacePosition(int polyNo, dss::Fix32Vector3& playerPos)
{
    dss::Fix32Vector3 pos(playerPos);
    COLL_POLY poly;
    if (stage_.collGetPoly(polyNo, &poly) == 1) {
        if (poly.normal.y != 0) {
            return true;
        }
        dss::Fix32Vector3 v0 = FldStage::getFx32Vector3(poly.bbox[0]);
        dss::Fix32Vector3 v1 = FldStage::getFx32Vector3(poly.bbox[1]);
        v0.vy = v1.vy = 0L;
        dss::Fix32Vector3 vec = v0 - v1;
        if (vec.lengthsq().value > 0x1000) {
            return true;
        }
        pos.vy = 0L;
        if ((pos - v0) * (v0 - v1) > dss::Fix32(0L)) {
            return false;
        }
        if ((pos - v1) * (v1 - v0) > dss::Fix32(0L)) {
            return false;
        }
    }
    return true;
}

ARM bool TownStageManager::isPolyFacePosition(int polyNo, dss::Fix32Vector3& playerPos, dss::Fix32 len)
{
    dss::Fix32Vector3 pos(playerPos);
    COLL_POLY poly;
    if (stage_.collGetPoly(polyNo, &poly) == 1) {
        dss::Fix32Vector3 v0 = FldStage::getFx32Vector3(poly.bbox[0]);
        dss::Fix32Vector3 v1 = FldStage::getFx32Vector3(poly.bbox[1]);
        v0.vy = v1.vy = 0L;
        dss::Fix32Vector3 vec = v0 - v1;
        pos.vy = 0L;
        if ((pos - v0) * (v0 - v1) > dss::Fix32(0L)) {
            return false;
        }
        if ((pos - v1) * (v1 - v0) > dss::Fix32(0L)) {
            return false;
        }
        dss::Fix32Vector3 normal = FldStage::getFx32Vector3(poly.normal);
        if ((pos - v0) * normal > len) {
            return false;
        }
        return !((pos - v0) * normal < dss::Fix32(0L));
    }
    return false;
}

ARM int TownStageManager::getObjectPos(int objectId, int index, dss::Fix32Vector3* pos)
{
    return coll_.getObjectPos(objectId, index, pos);
}

ARM void TownStageManager::addMovePosByObjNo(int objNo, dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos)
{
    dss::Fix32Vector3 move = nextPos - nowPos;
    stage_.addMovePosByObjNo(objNo, move);
}

ARM int TownStageManager::getCrossPolygonOtherSurface(dss::Fix32Vector3& start, dss::Fix32Vector3& end, short* surface, int count, int* poly, dss::Fix32* dist, int all)
{
    return stage_.getCrossPolygonOtherSurface(start, end, surface, count, poly, dist, all);
}

ARM void TownStageManager::collEraseMapUid(int uid)
{
    TownPlayerManager::getSingleton()->searchMapUid_ = 0;
    TownActionWalk::getSingleton()->searchObjectId_ = -1;
    stage_.m_fld.CollEraseMapUid(uid);
}

ARM bool TownStageManager::isRozariStage()
{
    switch (g_Global.getMapName()[0]) {
    case 'm':
        if (g_Global.getMapName()[1] == 'q') {
            return true;
        }
        break;
    case 'c':
        switch (g_Global.getMapName()[1]) {
        case 'j':
        case 'k':
        case 'l':
            return true;
        }
        break;
    case 'd':
        if (g_Global.getMapName()[1] == 'n') {
            return true;
        }
        break;
    case 's':
        switch (g_Global.getMapName()[1]) {
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
            return true;
        }
        break;
    case 't':
        if (g_Global.getMapName()[1] == 'd') {
            return true;
        }
        break;
    }
    return false;
}
