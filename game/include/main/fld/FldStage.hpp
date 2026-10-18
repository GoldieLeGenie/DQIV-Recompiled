#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "main/fld/FLDObject.hpp"
#include "main/fld/Coll.hpp"
#include "main/object/ModelObject.hpp"
#include "main/dss/Render.hpp"
#include "main/dss/PaletteEffect.hpp"

struct FldStage;
struct FldCollision;

int fldBoxTest(VecFx32* pos, VecFx32* rot, VecFx32* scale, VecFx32* box, dss::Fix32* rate);

struct FldStage : RenderScene {
    Render* m_render;                           // 0x008
    LZDataObject m_data;                        // 0x00C
    DataObject m_model;                         // 0x01C
    DataObject m_texture;                       // 0x02C
    DataObject m_coll;                          // 0x03C
    ModelResource m_anim;                      // 0x04C
    fld::FLDObject m_fld;                       // 0x058
    dss::Fix32Vector3 scale_;                   // 0x668
    int collisionFlag_;                         // 0x674
    int collisionDrawFlag_;                     // 0x678
    int exitType_;                              // 0x67C
    char path_[32];                             // 0x680
    int obj_index[128];                         // 0x6A0
    int pool_counter;                           // 0x8A0
    int extraObjectNum_;                        // 0x8A4
    int unk_8a8;                                // 0x8A8
    int unk_8ac;                                // 0x8AC
    int unk_8b0;                                // 0x8B0

    FldStage();
    ~FldStage();
    void setRender(Render* render);
    void terminate();
    virtual void setup();
    virtual void cleanup();
    void setPath(const char* path);
    bool isExist(const char* name);
    void load(const char* name);
    virtual void draw();
    void execAnime();
    dss::Fix32 getCameraLimitR();
    dss::Fix32 getCameraLimitL();
    void eraseObject(int uid, int flag);
    void setMapUidOnOff(int uid, int flag);
    int eventAnim(int anim, int frame);
    void repop(int uid);
    void commonAnim(int obj, int frame);
    void animLocation(int id, int frame, int uidFlag);
    void setAnimLocation(int obj, int frame);
    void setAlpha(int obj, int alpha);
    void setFldColl(FldCollision* coll);
    bool collGetPolygonPos(int poly, dss::Fix32Vector3* pos);
    int collCrossCheckPoly(dss::Fix32Vector3& start, dss::Fix32Vector3& end, dss::Fix32* dist, int all);
    int collCrossCheckOtherNo(dss::Fix32Vector3& start, dss::Fix32Vector3& end, int no, dss::Fix32* dist);
    void setRotObjectUid(int uid, dss::Fix32Vector3& rot);
    int collGetPoly(int poly, COLL_POLY* out);
    void setPosByObjectID(int id, dss::Fix32Vector3& pos);
    static dss::Fix32Vector3 getFx32Vector3(const VecFx32& vec);
    static VecFx32 getVecFx32(const dss::Fix32Vector3& vec);
    bool getObjectIn(int uid, dss::Fix32Vector3& pos);
    int getObjWallNo(int obj, int wall);
    int getObjWallPolyNo(int obj, int wall);
    short getObjectRotIdxY(int obj);
    VecFx32 getUidPos(int uid);
    int* GetMapUidObj(int uid);
    bool IsCommonAnimationEnd(int uid);
    COLL_ADD_RESULT_TYPE addBoxCollistion(dss::Fix32Vector3& center, dss::Fix32Vector3& vec, int& extraId, int& allocFlag);
    int getPolyNoBySurfaceId(int surface, int index);
    void addMovePosByObjNo(int obj, dss::Fix32Vector3& move);
    int getCrossPolygonOtherSurface(dss::Fix32Vector3& start, dss::Fix32Vector3& end, short* surface, int count, int* poly, dss::Fix32* dist, int all);

    int collCrossCheck(VecFx32& start, VecFx32& end, int poly, fx32* dist) {
        if (poly == 0) {
            m_fld.m_cross_pos = start;
            func_02062f98(&end, &start, &m_fld.m_cross_dir);
            func_020630ec(&m_fld.m_cross_dir, &m_fld.m_cross_dir);
            m_fld.m_cross_len = func_0206338c(&start, &end);
        }
        return coll_CrossCheck(m_fld.m_coll, &m_fld.m_cross_pos, &m_fld.m_cross_dir, m_fld.m_cross_len, poly, dist);
    }
};

