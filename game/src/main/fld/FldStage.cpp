#include <nitro/native_g3.h>
#pragma ipa file
#include "main/fld/FldStage.hpp"
#include "main/fld/FldCollision.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/data/FileLoader.hpp"
#include "main/text/TextAPI.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/script/sys/ScriptParam.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/object/DSSAObject.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"
#include "nnsys/g3d.hpp"

static int drawTick;
static int data_020f22ac;

ARM FldStage::FldStage()
{
    collisionFlag_ = 0;
    collisionDrawFlag_ = 0;
    unk_8a8 = 1;
    unk_8ac = 1;
    unk_8b0 = 1;
    extraObjectNum_ = 0;
    scale_.set(1, 1, 1);
}

ARM FldStage::~FldStage()
{
}

ARM void FldStage::setRender(Render* render)
{
    m_render = render;
}

ARM void FldStage::terminate()
{
}

ARM void FldStage::setup()
{
    unkfunc_0208532c(m_render);
    m_anim.unkfunc_02083354(m_model.getAddr());
    void* model = m_model.getAddr();
    void* texture = m_texture.getAddr();
    void* coll = m_coll.getAddr();
    m_fld.Setup(model, texture, coll, (int)*unkfunc_0207f88c(&data_0211a60c));
    dss::Fix32Vector3 scale(1, 1, 1);
    setPaletteRgbRate(0, scale);
    int color = m_fld.m_scene->backColor;
    unkfunc_02084c78((color & 0xff) >> 3, ((color >> 8) & 0xff) >> 3, ((color >> 16) & 0xff) >> 3);
    extraObjectNum_ = 0;
}

ARM void FldStage::cleanup()
{
    unkfunc_02084c78(0, 0, 0);
    unkfunc_02085348();
    m_fld.Final();
    m_model.cleanup();
    m_texture.cleanup();
    m_coll.cleanup();
    m_anim.unkfunc_02083360();
    m_data.cleanup();
}

ARM void FldStage::setPath(const char* path)
{
    dss::sprintf(path_, "%s", path);
}

ARM bool FldStage::isExist(const char* name)
{
    char buf[0x80];
    dss::sprintf(buf, "%s/%s.lz", path_, name);
    return dss::g_File.isExist(buf) != 0;
}

ARM void FldStage::load(const char* name)
{
    char buf[0x80];
    dss::sprintf_s(buf, sizeof(buf), "%s/%s.lz", path_, name);
    m_data.setup(buf, 1, 1);
    m_model.setup(unkfunc_0207f8dc(m_data.getAddr(), 0));
    m_texture.setup(unkfunc_0207f8dc(m_data.getAddr(), 1));
    if (unkfunc_0207f8cc(m_data.getAddr(), 2)) {
        m_coll.setup(unkfunc_0207f8dc(m_data.getAddr(), 2));
    }
    if (m_coll.getAddr()) {
        collisionFlag_ = 1;
    }
}

static void resetGlbMatrix();

ARM void FldStage::draw()
{
    drawTick = unkfunc_0207e7e8();
    resetGlbMatrix();
    m_fld.calc();
    resetGlbMatrix();
    if (collisionDrawFlag_ == 0) {
        func_0206ae30((VecFx32*)&scale_);
        m_fld.draw();
    }
    resetGlbMatrix();
    drawTick = unkfunc_0207e7e8() - drawTick;
}

ARM void FldStage::execAnime()
{
    m_anim.unkfunc_0208336c();
}

static void resetGlbMatrix()
{
    VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
    VecFx32 trans = { 0, 0, 0 };
    MtxFx33 rot;
    func_02061b88(&rot);
    func_0206ae30(&scale);
    func_02067940(&rot, &data_0210cf28.prmBaseRot);
    data_0210cf28.flag &= ~0xa4;
    func_0206ae08((dss::Fix32Vector3*)&trans);
    func_0206adcc();
}

ARM dss::Fix32 FldStage::getCameraLimitR()
{
    return dss::Fix32(m_fld.m_scene->cameraLimitR);
}

ARM dss::Fix32 FldStage::getCameraLimitL()
{
    return dss::Fix32(m_fld.m_scene->cameraLimitL);
}

ARM void FldStage::eraseObject(int uid, int flag)
{
    m_fld.CollEraseMapUid(uid);
    m_fld.SetMapUidOnOff(uid, flag);
}

ARM void FldStage::setMapUidOnOff(int uid, int flag)
{
    m_fld.SetMapUidOnOff(uid, flag);
}

ARM int FldStage::eventAnim(int anim, int frame)
{
    if (frame == 0) {
        m_fld.setEventAnimation(anim);
        return 1;
    }
    if (m_fld.getMapObjAnimFrame(frame) != m_fld.getMapObjAnimFrameMax(frame)) {
        return 0;
    }
    m_fld.setEventAnimation(anim);
    return 1;
}

ARM void FldStage::repop(int uid)
{
    int* list = GetMapUidObj(uid);
    for (int i = 0; i < pool_counter; i++) {
        m_fld.SetMapObjOnOff(list[i], 0);
        m_fld.SetCommonAnimation(list[i], 0);
        coll_ResetObjId(m_fld.m_coll, list[i]);
    }
}

ARM void FldStage::commonAnim(int obj, int frame)
{
    m_fld.SetCommonAnimation(obj, frame);
}

ARM void FldStage::animLocation(int id, int frame, int uidFlag)
{
    if (uidFlag == 1) {
        int* list = GetMapUidObj(id);
        for (int i = 0; i < pool_counter; i++) {
            setAnimLocation(list[i], frame);
        }
        return;
    }
    setAnimLocation(id, frame);
}

ARM void FldStage::setAnimLocation(int obj, int frame)
{
    if (m_fld.IsCommonAnimationEnd(obj) == 1) {
        m_fld.SetCommonAnimation(obj, 0);
        coll_ResetObjId(m_fld.m_coll, obj);
    }
    if (frame == 0) {
        coll_EraseObjId(m_fld.m_coll, obj);
        m_fld.SetMapObjOnOff(obj, 1);
        return;
    }
    if (m_fld.GetCommonAnimationNum(obj) < frame) {
        return;
    }
    if (frame == m_fld.GetCommonAnimationNo(obj)) {
        return;
    }
    m_fld.SetCommonAnimation(obj, frame);
}

ARM void FldStage::setAlpha(int obj, int alpha)
{
    m_fld.SetMapObjAlpha(obj, alpha, 0);
}

ARM void FldStage::setFldColl(FldCollision* coll)
{
    coll->m_collisionFlag = &collisionFlag_;
    coll->m_fld = &m_fld;
    coll->m_coll = &m_coll;
}

ARM bool FldStage::collGetPolygonPos(int poly, dss::Fix32Vector3* pos)
{
    COLL_POLY p;
    *pos *= 0;
    if (!coll_GetPoly(m_fld.m_coll, poly, &p)) {
        return false;
    }
    if (p.type & 1) {
        for (int i = 0; i < 4; i++) {
            pos->vx.value += p.vertex[i].x;
            pos->vy.value += p.vertex[i].y;
            pos->vz.value += p.vertex[i].z;
        }
        *pos /= 4;
    } else {
        for (int i = 0; i < 3; i++) {
            pos->vx.value += p.vertex[i].x;
            pos->vy.value += p.vertex[i].y;
            pos->vz.value += p.vertex[i].z;
        }
        *pos /= 3;
    }
    return true;
}

ARM int FldStage::collCrossCheckPoly(dss::Fix32Vector3& start, dss::Fix32Vector3& end, dss::Fix32* dist, int all)
{
    VecFx32 s;
    VecFx32 e;
    fx32 d;
    s.x = start.vx.value;
    s.y = start.vy.value;
    s.z = start.vz.value;
    e.x = end.vx.value;
    e.y = end.vy.value;
    e.z = end.vz.value;

    int poly = 0;
    int ret = -1;
    fx32 min = 0x7ffffff;
    while ((poly = collCrossCheck(s, e, poly, &d)) != -1) {
        if (coll_GetSurface(m_fld.m_coll, poly) != -1 && !all) {
            poly++;
            continue;
        }
        d = absInt(d);
        if (d < min) {
            ret = poly;
            min = d;
        }
        poly++;
    }
    dist->value = min;
    return ret;
}

ARM int FldStage::collCrossCheckOtherNo(dss::Fix32Vector3& start, dss::Fix32Vector3& end, int no, dss::Fix32* dist)
{
    VecFx32 s = getVecFx32(start);
    VecFx32 e = getVecFx32(end);
    int poly = 0;
    int ret = -1;
    fx32 min = 0x7ffffff;
    fx32 d = min;
    while ((poly = collCrossCheck(s, e, poly, &d)) != -1) {
        if (d < min && poly != no) {
            ret = poly;
            min = d;
        }
        poly++;
    }
    if (dist) {
        dist->value = min;
    }
    return ret;
}

ARM void FldStage::setRotObjectUid(int uid, dss::Fix32Vector3& rot)
{
    VecFx32 v = getVecFx32(rot);
    m_fld.setMapUidRotFX32(uid, &v);
}

ARM int FldStage::collGetPoly(int poly, COLL_POLY* out)
{
    return coll_GetPoly(m_fld.m_coll, poly, out);
}

ARM void FldStage::setPosByObjectID(int id, dss::Fix32Vector3& pos)
{
    VecFx32 v = getVecFx32(pos);
    m_fld.SetMapObjPosFX32(id, &v);
}

ARM dss::Fix32Vector3 FldStage::getFx32Vector3(const VecFx32& vec)
{
    dss::Fix32Vector3 ret;
    ret.vx.value = vec.x;
    ret.vy.value = vec.y;
    ret.vz.value = vec.z;
    return ret;
}

ARM VecFx32 FldStage::getVecFx32(const dss::Fix32Vector3& vec)
{
    VecFx32 ret;
    ret.x = vec.vx.value;
    ret.y = vec.vy.value;
    ret.z = vec.vz.value;
    return ret;
}

ARM bool FldStage::getObjectIn(int uid, dss::Fix32Vector3& pos)
{
    int i;
    bool in = false;
    COLL_POLY poly;
    VecFx32 p = getVecFx32(pos);
    int minX = 0x7fffffff;
    int minZ = 0x7fffffff;
    int maxX = 0x80000000;
    int maxZ = 0x80000000;

    i = coll_GetPolyNoByMapObj(m_fld.m_coll, uid, 0);
    if (i != -1) do {
        in = true;
        coll_GetPoly(m_fld.m_coll, i, &poly);
        int lx = poly.bbox[0].x <= poly.bbox[1].x ? poly.bbox[0].x : poly.bbox[1].x;
        int hx = poly.bbox[0].x >= poly.bbox[1].x ? poly.bbox[0].x : poly.bbox[1].x;
        int lz = poly.bbox[0].z <= poly.bbox[1].z ? poly.bbox[0].z : poly.bbox[1].z;
        int hz = poly.bbox[0].z >= poly.bbox[1].z ? poly.bbox[0].z : poly.bbox[1].z;
        if (lx < minX) minX = lx;
        if (hx > maxX) maxX = hx;
        if (lz < minZ) minZ = lz;
        if (hz > maxZ) maxZ = hz;
        i = coll_GetPolyNoByMapObj(m_fld.m_coll, uid, i + 1);
    } while (i != -1);
    if (in == true) {
        if (p.x < minX || p.x > maxX || p.z < minZ || p.z > maxZ) {
            in = false;
        }
    }
    return in;
}

ARM int FldStage::getObjWallNo(int obj, int wall)
{
    if (obj != coll_GetObjId(m_fld.m_coll, wall)) {
        return -1;
    }
    return coll_GetObjWallNo(m_fld.m_coll, obj, wall);
}

ARM int FldStage::getObjWallPolyNo(int obj, int wall)
{
    int poly = coll_GetPolyNoByMapObj(m_fld.m_coll, obj, 0);
    if (poly == -1) {
        return -1;
    }
    poly += wall;
    if (obj != coll_GetObjId(m_fld.m_coll, poly)) {
        poly = -1;
    }
    return poly;
}

ARM short FldStage::getObjectRotIdxY(int obj)
{
    return m_fld.GetMapObjRotFX32(obj)->y;
}

ARM VecFx32 FldStage::getUidPos(int uid)
{
    VecFx32 pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    fld::FLD_MAP* map = m_fld.m_map;
    int* list = GetMapUidObj(uid);
    int count = pool_counter;
    for (int i = 0; i < count; i++) {
        pos.x += map->obj[list[i]].pos.x;
        pos.y += map->obj[list[i]].pos.y;
        pos.z += map->obj[list[i]].pos.z;
    }
    pos.x /= count;
    pos.y /= count;
    pos.z /= count;
    return pos;
}

ARM int* FldStage::GetMapUidObj(int uid)
{
    pool_counter = 0;
    fld::FLD_MAP* map = m_fld.m_map;
    for (int i = 0; i < 128; i++) {
        obj_index[i] = -1;
    }
    if (uid <= 0) {
        return obj_index;
    }
    for (int i = 0; i < map->obj_num; i++) {
        short index = map->index[i];
        if (index >= 0 && uid == map->obj[index].uid) {
            obj_index[pool_counter] = i;
            pool_counter++;
        }
    }
    return obj_index;
}

ARM bool FldStage::IsCommonAnimationEnd(int uid)
{
    int* list = GetMapUidObj(uid);
    for (int i = 0; i < pool_counter; i++) {
        if (m_fld.IsCommonAnimationEnd(list[i]) == 0) {
            return false;
        }
    }
    return true;
}

ARM int fldBoxTest(VecFx32* pos, VecFx32* rot, VecFx32* scale, VecFx32* box, dss::Fix32* rate)
{
    int result;
    G3_PushMtx();
    G3_Translate(pos->x, pos->y, pos->z);
    func_02065c60(FX_SinIdx((unsigned short)rot->y), FX_CosIdx((unsigned short)rot->y));
    func_02065c24(FX_SinIdx((unsigned short)rot->x), FX_CosIdx((unsigned short)rot->x));
    func_02065c9c(FX_SinIdx((unsigned short)rot->z), FX_CosIdx((unsigned short)rot->z));
    G3_Scale(scale->x, scale->y, scale->z);
    G3_Scale(rate->value, rate->value, rate->value);
    G3_WRITE(0x70, box->x);
    G3_WRITE(0x70, box->y);
    G3_WRITE(0x70, box->z);
    while (func_02065544(&result)) {
    }
    G3_PopMtx(1);
    return result;
}

ARM COLL_ADD_RESULT_TYPE FldStage::addBoxCollistion(dss::Fix32Vector3& center, dss::Fix32Vector3& vec, int& extraId, int& allocFlag)
{
    if (extraObjectNum_ == 0) {
        extraObjectNum_ = m_fld.m_map->uid_num + 100;
    }
    short objId;
    if (extraId == -1) {
        objId = extraObjectNum_;
    } else {
        objId = extraId;
    }

    dss::Fix32Vector3 p[4];
    dss::Fix32Vector3 normal;
    dss::Fix32Vector3 work;
    COLL_POLY poly;
    COLL_ADD_RESULT_TYPE ret;

    poly.obj_id = objId;
    poly.type = poly.type | 0x101;
    poly.id = 0;
    poly.flag = 0;

    // face 0
    p[0].vy = p[3].vy = vec.vy * -1;
    p[2].vy = p[1].vy = vec.vy;
    for (int i = 0; i < 4; i++) {
        p[i].vx = vec.vx * -1;
        p[i].vz = vec.vz;
    }
    p[3].vz *= -1;
    p[2].vz *= -1;
    normal.set(-FX32_ONE, 0, 0);
    poly.normal = getVecFx32(normal);
    for (int i = 0; i < 4; i++) {
        work = center + p[i];
        poly.vertex[i] = getVecFx32(work);
    }
    ret = coll_AddCollPoly2(extraId, 0, m_fld.m_coll, &poly, &m_fld.m_allocator, allocFlag);
    if (ret != RESULT_OK) {
        return ret;
    }

    // face 1
    for (int i = 0; i < 4; i++) {
        p[i].vx = vec.vx;
        p[i].vz = vec.vz * -1;
    }
    p[0].vx *= -1;
    p[1].vx *= -1;
    normal.set(0, 0, -FX32_ONE);
    poly.normal = getVecFx32(normal);
    for (int i = 0; i < 4; i++) {
        work = center + p[i];
        poly.vertex[i] = getVecFx32(work);
    }
    ret = coll_AddCollPoly2(extraId, 1, m_fld.m_coll, &poly, &m_fld.m_allocator, allocFlag);
    if (ret != RESULT_OK) {
        return ret;
    }

    // face 2
    for (int i = 0; i < 4; i++) {
        p[i].vx = vec.vx;
        p[i].vz = vec.vz;
    }
    p[0].vz *= -1;
    p[1].vz *= -1;
    normal.set(FX32_ONE, 0, 0);
    poly.normal = getVecFx32(normal);
    for (int i = 0; i < 4; i++) {
        work = center + p[i];
        poly.vertex[i] = getVecFx32(work);
    }
    ret = coll_AddCollPoly2(extraId, 2, m_fld.m_coll, &poly, &m_fld.m_allocator, allocFlag);
    if (ret != RESULT_OK) {
        return ret;
    }

    // face 3
    for (int i = 0; i < 4; i++) {
        p[i].vx = vec.vx;
        p[i].vz = vec.vz;
    }
    p[3].vx *= -1;
    p[2].vx *= -1;
    normal.set(0, 0, FX32_ONE);
    poly.normal = getVecFx32(normal);
    for (int i = 0; i < 4; i++) {
        work = center + p[i];
        poly.vertex[i] = getVecFx32(work);
    }
    ret = coll_AddCollPoly2(extraId, 3, m_fld.m_coll, &poly, &m_fld.m_allocator, allocFlag);
    if (ret != RESULT_OK) {
        return ret;
    }

    extraObjectNum_++;
    extraId = objId;
    return RESULT_OK;
}

ARM int FldStage::getPolyNoBySurfaceId(int surface, int index)
{
    return coll_GetPolyNoBySurface(m_fld.m_coll, surface, index);
}

ARM void FldStage::addMovePosByObjNo(int obj, dss::Fix32Vector3& move)
{
    VecFx32 v = getVecFx32(move);
    m_fld.CollAddPolyPosByMapObj(obj, &v);
}

ARM int FldStage::getCrossPolygonOtherSurface(dss::Fix32Vector3& start, dss::Fix32Vector3& end, short* surface, int count, int* polyOut, dss::Fix32* dist, int all)
{
    int found = 0;
    fx32 d;
    VecFx32 s = getVecFx32(start);
    VecFx32 e = getVecFx32(end);
    int poly = 0;
    int ret = -1;
    fx32 min = 0x7ffffff;
    int floorNum = m_fld.m_coll->floor_poly_size;

    while ((poly = collCrossCheck(s, e, poly, &d)) != -1) {
        if (!all && poly < floorNum) {
            poly++;
            continue;
        }
        int hit = 0;
        for (int i = 0; i < count; i++) {
            short attr = coll_GetSurface(m_fld.m_coll, poly);
            if ((attr & 0xf000) == surface[i]) {
                hit = 1;
            }
        }
        if (hit == 1) {
            poly++;
            continue;
        }
        d = absInt(d);
        if (d < min) {
            ret = poly;
            min = d;
        }
        found++;
        poly++;
    }
    *polyOut = ret;
    if (dist) {
        dist->value = min;
    }
    return found;
}

// Unreferenced storage before the BillboardCharacter globals.
ARM void unkfunc_unused_30()
{
    data_020f22ac = 0;
}
