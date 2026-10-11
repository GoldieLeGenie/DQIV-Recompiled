#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "main/fld/FLDObject.hpp"
#include "main/fld/Coll.hpp"

struct FldCollision {
    DataObject* m_coll;                         // 0x00 collision file (COLL_FILE)
    fld::FLDObject* m_fld;                      // 0x04
    int* m_collisionFlag;                       // 0x08
    int m_id;                                   // 0x0C
    int m_exitType;                             // 0x10
    int m_polyIndex;                            // 0x14
    int m_polyIndexBak;                         // 0x18
    int m_floorPolygonNo;                       // 0x1C
    int m_surfaceType[14];                      // 0x20
    int m_surfacePolyNo[14];                    // 0x58
    int m_eraseSurfaceId[15];                   // 0x90
    int m_eraseSurfaceCount;                    // 0xCC
    int m_searchObjectId;                       // 0xD0
    int m_searchPolyNo;                         // 0xD4
    dss::Fix32 m_searchLen2;                    // 0xD8
    dss::Fix32 m_surfaceLen;                    // 0xDC
    COLL_POLY* m_nextList[30];                  // 0xE0
    int m_collPolyNo[30];                       // 0x158
    int m_collCount;                            // 0x1D0
    int m_crossCount;                           // 0x1D4
    dss::Fix32Vector3 m_playerDir;              // 0x1D8
    fx32 m_newX;                                // 0x1E4
    fx32 m_newY;                                // 0x1E8
    fx32 m_newZ;                                // 0x1EC
    fx32 m_preR;                                // 0x1F0
    fx32 m_radB;                                // 0x1F4
    fx32 m_radS;                                // 0x1F8
    VecFx32 m_dirVec32;                         // 0x1FC

    FldCollision();
    ~FldCollision();
    bool setExitPosition(dss::Fix32Vector3* pos, int index);
    dss::Fix32Vector3 compute(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos, dss::Fix32 radius, dss::Fix32 surfaceR, dss::Fix32 preR, dss::Fix32& height);
    void computeCollFloor(dss::Fix32Vector3& pos, dss::Fix32 radius, dss::Fix32Vector3& fixYPos);
    void searchClear();
    void computeCollWall(const dss::Fix32Vector3& oldPos, const dss::Fix32Vector3& newPos, dss::Fix32 collRad, dss::Fix32 searchRad, dss::Fix32 preRad, dss::Fix32Vector3& retPos);
    bool checkSignPoly(const dss::Fix32Vector3& newPos, dss::Fix32Vector3& cross, int polyNo, COLL_POLY* poly);
    void characterColl(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos, dss::Fix32 radius, dss::Fix32Vector3* retPos, int type);
    int getSearchObjectId();
    int getSearchPolyNo();
    int boxCompute(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 rad, dss::Fix32Vector3* retVec);
    bool checkCrossPolygon(VecFx32& pos, VecFx32& pos1, int polyNo);
    int getSurfaceByType(int type);
    int checkCrossNum(VecFx32& pos0, VecFx32& pos1, int notFloor);
    int checkCrossNumCheckUnder(VecFx32& pos0, VecFx32& pos1, int notFloor);
    int checkCrossNumEraseSurface(VecFx32 pos0, VecFx32 pos1, int surface, int notFloor, int& polyNo);
    bool getObjectPos(int objectID, int polyNo, dss::Fix32Vector3* pos);
    void searchFloorSurface(dss::Fix32Vector3& pos, dss::Fix32 radius, dss::Fix32 judgeLen, dss::Fix32Vector3& retVec);
    bool isAnimObject(int objectId);
    void setEraseSurface(int surfaceId, bool flag);
    void resetEraseSurface();
    bool isEraseSurfaceId(int surfaceId);
    void wallPolyCheck(const dss::Fix32Vector3& newPos, COLL_POLY* poly, int start, int count);
    int getFrontPoly(int polyNo, int objNo);
};
