#pragma once
#include "globaldefs.h"
#include "main/fld/FldCollision.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/global/StageLink.hpp"
#include "main/fld/FLDObject.hpp"
#include "main/fld/FldStage.hpp"
#include "ov000/town/TownDataManager.hpp"
#include "ov000/town/TownMapEffect.hpp"

struct TownStageManager;

extern int data_0211d434;
extern "C" {
}

struct TownStageManager {
    FldStage stage_;                                                                // 0x000
    FldCollision coll_;                                                             // 0x8B4
    TownDataManager townData_;                                                      // 0xABC
    int allocFlag_;                                                                 // 0xAF0
    int softEraseObjId_[4];                                                         // 0xAF4
    int softEraseNum_;                                                              // 0xB04
    TownMapEffect mapEffect_;                                                       // 0xB08
    int stopScript_;                                                                // 0xBD4

    TownStageManager();
    ~TownStageManager();
    static TownStageManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    bool isStageExist(char* name);
    char* getLinkMapName();
    void setExitPosition(dss::Fix32Vector3* pos, int index);
    dss::Fix32Vector3 getMapUidPos(int uid);
    void addMapUidPosFX32(int uid, dss::Fix32Vector3& pos);
    void setMapUidPosFX32(int uid, dss::Fix32Vector3& pos);
    dss::Fix32Vector3 getRiseupPos(int uid, int type);
    void loadStage(char* name);
    void setObjectDraw(int uid, int flag, int frame);
    void rotObjectUid(int uid, short rot);
    void SetRGBRate(dss::Fix32Vector3& rate, int flag);
    void setTextureScaling(fx32 scaleX, fx32 scaleY);
    void getTextureScaling(fx32& scaleX, fx32& scaleY);
    void setEffect(TownMapEffect::EFFECT_TYPE type);
    int getOtherPolyNoBySurfaceId(int surface, int polyNo);
    dss::Fix32Vector3 getSurfaceDir(int surface);
    void setSoftErase(int objectId);
    void execSoftErase();
    int isEndSoftErase(int objectId);
    int getExitIndex();
    bool isPolyFacePosition(int polyNo, dss::Fix32Vector3& playerPos);
    int getCrossPolygonOtherSurface(dss::Fix32Vector3& start, dss::Fix32Vector3& end, short* surface, int count, int* poly, dss::Fix32* dist, int all);
    void collEraseMapUid(int uid);
    bool isRozariStage();
    static dss::Fix32 unkfunc_dataPad0();
    static dss::Fix32 unkfunc_dataPad1();

    void setCollision(int id, int flag) { stage_.setMapUidOnOff(id, flag); }
    void setCollisionObject(int id) { stage_.m_fld.CollResetMapUid(id); }
    void setCameraNo(int channel, int screen) { stage_.m_fld.SetCameraNo(channel, screen); }
    void eraseObject(int uid, int flag) { stage_.eraseObject(uid, flag); }
    void setAlpha(int obj, int alpha) { stage_.setAlpha(obj, alpha); }
    int getMapObjUid(int obj) { return stage_.m_fld.GetMapObjUid(obj); }
    int getMapObjCommonId(int obj) { return obj == -1 ? -1 : stage_.m_fld.GetMapObjCommonId(obj); }
    int getObjWallNo(int obj, int wall) { return stage_.getObjWallNo(obj, wall); }
    void collEraseObject(int obj) { coll_EraseObjId(stage_.m_fld.m_coll, obj); }
    void collResetObject(int obj) { coll_ResetObjId(stage_.m_fld.m_coll, obj); }
    void commonAnim(int obj, int frame) { stage_.commonAnim(obj, frame); }
    void setPosByObjectID(int id, dss::Fix32Vector3& pos) { stage_.setPosByObjectID(id, pos); }
    void searchFloorSurface(dss::Fix32Vector3& pos, dss::Fix32 r, dss::Fix32 len, dss::Fix32Vector3& out) { coll_.searchFloorSurface(pos, r, len, out); }
    void computeCollFloor(dss::Fix32Vector3& pos, dss::Fix32 r, dss::Fix32Vector3 next) { coll_.computeCollFloor(pos, r, next); }
    void eventAnim(int id, int flag) { stage_.eventAnim(id, flag); }
    void setMapTexture(int texture) { mapEffect_.m_enable = texture; }
    void setClipping(int clip) { stage_.m_fld.clipping = clip; }
    void setBoxTest(int flag) { stage_.m_fld.m_box_test = flag; }
    VecFx32& GetCameraCentFX32(int no) { return stage_.m_fld.m_x32_camera_pos[no]; }
    void setNextBackColor(int index) { townData_.setNextBackColor(index); }
    int getNextBackColor() { return townData_.getNextBackColor(); }
    void setClipDistance(dss::Fix32 dist) { stage_.m_fld.m_bbox_far = dist.value; }
    bool isCommonAnimationEnd(int uid) { return stage_.IsCommonAnimationEnd(uid); }
    void setMapUidAlpha(int uid, int alpha, int priority) { stage_.m_fld.SetMapUidAlpha(uid, alpha, priority); }
    void boxCompute(dss::Fix32Vector3& pos, dss::Fix32Vector3& next, dss::Fix32 r, dss::Fix32Vector3* out) { coll_.boxCompute(pos, next, r, out); }
    dss::Fix32Vector3 compute(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 radius, dss::Fix32 surfaceRad, dss::Fix32 preR, dss::Fix32& height);
    int getObjectIDfromMapUid(int mapUid);
    int getHitSurfaceIdByType(int type);
    bool checkCrossPolygon(dss::Fix32Vector3 pos0, dss::Fix32Vector3 pos1, int polyNo);
    int checkCrossNumEraseSurface(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int surface, int flag, int& polyNo);
    bool getSearchPolyDirection(dss::Fix32Vector3& dir);
    dss::Fix32Vector3 getHitSurfaceDirByType(int type);
    dss::Fix32Vector3 getHitSurfacePosByType(int type);
    int getObjectPos(int objectId, int index, dss::Fix32Vector3* pos);
    bool addBoxCollision(dss::Fix32Vector3& center, dss::Fix32Vector3& vec, int& extraId);
    void addMovePosByObjNo(int objNo, dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos);
    bool isPolyFacePosition(int polyNo, dss::Fix32Vector3& playerPos, dss::Fix32 len);
    bool getPolyDirection(dss::Fix32Vector3& dir, int polyNo);
    int checkCrossNum(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int flag);
    int checkCrossNumCheckUnder(dss::Fix32Vector3& pos0, dss::Fix32Vector3& pos1, int flag);
    void characoterColl(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos, dss::Fix32 rad, dss::Fix32Vector3* retVec, int flag)
    {
        coll_.characterColl(oldPos, newPos, rad, retVec, flag);
    }
};
