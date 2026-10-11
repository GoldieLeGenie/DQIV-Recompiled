#pragma ipa file
#include "main/fld/FldCollision.hpp"
#include "main/fld/FldStage.hpp"
#include "main/menu/MenuManager.hpp"

static int g_floor_ret;

ARM FldCollision::FldCollision()
{
    m_polyIndex = -1;
}

ARM FldCollision::~FldCollision()
{
}

ARM bool FldCollision::setExitPosition(dss::Fix32Vector3* pos, int index)
{
    int polyNo = coll_Id2PolyNo(&((COLL_FILE*)m_coll->getAddr())->header, index);
    if (polyNo == -1) {
        pos->set(0, 0, 0);
        return false;
    }
    COLL_POLY polyInfo;
    coll_GetPoly(&((COLL_FILE*)m_coll->getAddr())->header, polyNo, &polyInfo);
    dss::Fix32Vector3* normal = (dss::Fix32Vector3*)&polyInfo.normal;
    dss::Fix32Vector3 axisY(0, 1, 0);
    dss::Fix32 dot = *normal * axisY;
    static const dss::Fix32 fix(0x8cd);
    if (dot.value < 0x165) {
        m_exitType = 0;
        dss::Fix32Vector3 sum;
        sum = *(dss::Fix32Vector3*)&polyInfo.vertex[0];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[1];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[2];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[3];
        sum /= 4;
        sum += *normal * fix;
        *pos = sum;
    } else {
        m_exitType = 1;
        dss::Fix32Vector3 sum;
        sum = *(dss::Fix32Vector3*)&polyInfo.vertex[0];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[1];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[2];
        sum += *(dss::Fix32Vector3*)&polyInfo.vertex[3];
        sum /= 4;
        sum.vy += 0x28;
        *pos = sum;
    }
    return true;
}

ARM dss::Fix32Vector3 FldCollision::compute(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos, dss::Fix32 radius, dss::Fix32 surfaceR, dss::Fix32 preR, dss::Fix32& height)
{
    static const dss::Fix32 judgeLen(0xccd);
    if (*m_collisionFlag == 0) {
        return newPos;
    }
    g_floor_ret = unkfunc_0207e7e8();
    m_polyIndex = -1;
    searchClear();
    dss::Fix32Vector3 retVec;
    searchFloorSurface(oldPos, radius, judgeLen, retVec);
    dss::Fix32Vector3 correctedPosition;
    computeCollWall(oldPos, newPos, radius, surfaceR, preR, correctedPosition);
    correctedPosition.vy = oldPos.vy;
    dss::Fix32Vector3 fixYPos;
    computeCollFloor(correctedPosition, radius, fixYPos);
    correctedPosition.vy = oldPos.vy;
    height = fixYPos.vy - oldPos.vy;
    g_floor_ret = unkfunc_0207e7e8() - g_floor_ret;
    return correctedPosition;
}

ARM void FldCollision::computeCollFloor(dss::Fix32Vector3& pos, dss::Fix32 radius, dss::Fix32Vector3& fixYPos)
{
    int ret = coll_SearchFloorPoly(m_fld->m_coll, (VecFx32*)&pos, radius.value, (VecFx32*)&fixYPos);
    if (ret < 0) {
        return;
    }
    m_floorPolygonNo = ret;
    if (coll_GetSurface(m_fld->m_coll, m_floorPolygonNo) == -1 && m_surfaceType[0] == -1) {
        m_surfaceType[0] = 0;
        m_surfacePolyNo[0] = m_floorPolygonNo;
    }
}

ARM void FldCollision::searchClear()
{
    for (int i = 0; i < 14; i++) {
        m_surfaceType[i] = -1;
    }
    m_searchObjectId = -1;
}

ARM void FldCollision::computeCollWall(const dss::Fix32Vector3& oldPos, const dss::Fix32Vector3& newPos, dss::Fix32 collRad, dss::Fix32 searchRad, dss::Fix32 preRad, dss::Fix32Vector3& retPos)
{
    unsigned short count;
    COLL_HEADER* coll = m_fld->m_coll;
    m_id = -1;
    retPos = oldPos;
    m_searchLen2.value = 0x7fffffff;
    unsigned short start = coll->floor_poly_size;
    count = start + (coll->wall_poly_size + coll->common_poly_size);
    m_newX = newPos.vx.value;
    m_newY = newPos.vy.value + collRad.value;
    m_newZ = newPos.vz.value;
    m_preR = preRad.value;
    m_radB = searchRad.value;
    m_radS = collRad.value;
    dss::Fix32Vector3 dirVec = newPos - oldPos;
    m_dirVec32.x = -dirVec.vx.value;
    m_dirVec32.y = -dirVec.vy.value;
    m_dirVec32.z = -dirVec.vz.value;
    dirVec.normalize();
    m_collCount = 0;
    m_crossCount = 0;
    wallPolyCheck(newPos, coll->poly, start, count);
    wallPolyCheck(newPos, coll->ext_data->ext_coll, 0, coll->ext_data->ext_num);
    if (m_crossCount == 0) {
        retPos = newPos;
        m_id = -1;
        return;
    }
    int min_poly_no = -1;
    fx32 rr = FX_Mul(collRad.value, collRad.value);
    fx32 min;
    fx32 len;
    VecFx32 min_cross;
    VecFx32 cross;
    VecFx32 min_crossN;
    VecFx32 ret = FldStage::getVecFx32(newPos);
    int counter = 0;
    long i;
    int signPolyNo = -1;
    while (true) {
        if (counter > 2) {
            retPos = oldPos;
            if (signPolyNo == -1) {
                signPolyNo = min_poly_no;
            }
            m_id = signPolyNo;
            return;
        }
        m_crossCount = 0;
        min = rr;
        for (i = 0; i < m_collCount; i++) {
            COLL_POLY* poly = m_nextList[i];
            if (poly->flag & 1) {
                continue;
            }
            if (m_newY < poly->bbox[0].y - m_radS || m_newY > m_radS + poly->bbox[1].y) {
                continue;
            }
            if (coll_CheckLinePoint(&ret, m_radS, &poly->bbox[0], &poly->bbox[1], &poly->normal, &cross) != 1) {
                continue;
            }
            len = FX_Mul(cross.x - ret.x, cross.x - ret.x) + FX_Mul(cross.z - ret.z, cross.z - ret.z);
            if (min > len + 2) {
                min = len;
                min_poly_no = m_collPolyNo[i];
                min_cross = cross;
                min_cross.y = oldPos.vy.value;
                m_crossCount++;
                if (m_nextList[i]->id != -1 || m_nextList[i]->obj_id != -1) {
                    signPolyNo = m_collPolyNo[i];
                }
            }
            if (signPolyNo == -1) {
                if (m_nextList[i]->id != -1 || m_nextList[i]->obj_id != -1) {
                    signPolyNo = m_collPolyNo[i];
                }
            }
        }
        if (m_crossCount == 0) {
            retPos = FldStage::getFx32Vector3(ret);
            if (signPolyNo == -1) {
                signPolyNo = min_poly_no;
            }
            m_id = signPolyNo;
            return;
        }
        func_02062f98(&ret, &min_cross, &min_crossN);
        min_crossN.y = 0;
        if (min_crossN.x == 0 && min_crossN.z == 0) {
            ret = min_cross;
        } else {
            func_020630ec(&min_crossN, &min_crossN);
            func_02063330(m_radS, &min_crossN, &min_cross, &ret);
        }
        counter++;
    }
}

ARM bool FldCollision::checkSignPoly(const dss::Fix32Vector3& newPos, dss::Fix32Vector3& cross, int polyNo, COLL_POLY* poly)
{
    dss::Fix32 len2;
    if (poly->obj_id != -1) {
        if (isAnimObject(m_searchObjectId) == true) {
            return true;
        }
        if (isAnimObject(poly->obj_id) == true) {
            m_searchObjectId = poly->obj_id;
            m_searchPolyNo = polyNo;
            m_searchLen2 = len2;
            return true;
        }
        len2 = (cross - newPos).lengthsq();
        if (len2 < m_searchLen2) {
            dss::Fix32Vector3 vec = cross - newPos;
            vec.vy = 0L;
            vec.normalize();
            dss::Fix32 dot = m_playerDir * vec;
            if (dot > dss::Fix32(0x800)) {
                m_searchObjectId = poly->obj_id;
                m_searchPolyNo = polyNo;
                m_searchLen2 = len2;
            }
        }
    }
    if (poly->id != -1) {
        if (poly->id & 0xa000) {
            len2.value = FX_Mul(cross.vx.value - newPos.vx.value, cross.vx.value - newPos.vx.value) + FX_Mul(cross.vz.value - newPos.vz.value, cross.vz.value - newPos.vz.value);
            if (m_surfaceType[10] != -1 && m_surfaceLen < len2) {
                return true;
            }
            m_surfaceLen = len2;
        }
        if (isEraseSurfaceId(poly->id) == true) {
            return true;
        }
        int tempSurfaceID = poly->id;
        int type = (tempSurfaceID >> 12) & 0xf;
        m_surfaceType[type] = tempSurfaceID;
        m_surfacePolyNo[type] = polyNo;
    }
    return true;
}

ARM void FldCollision::characterColl(dss::Fix32Vector3& oldPos, dss::Fix32Vector3& newPos, dss::Fix32 radius, dss::Fix32Vector3* retPos, int type)
{
    if (type & 1) {
        coll_GetNextMove(m_fld->m_coll, (VecFx32*)&oldPos, (VecFx32*)&newPos, radius.value, (VecFx32*)retPos);
    }
    if (type & 2) {
        retPos->vy += radius * 2;
        coll_SearchFloorPoly(m_fld->m_coll, (VecFx32*)retPos, radius.value, (VecFx32*)retPos);
        retPos->vy -= radius;
    }
}

ARM int FldCollision::getSearchObjectId()
{
    return m_searchObjectId;
}

ARM int FldCollision::getSearchPolyNo()
{
    return m_searchPolyNo;
}

ARM int FldCollision::boxCompute(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32 rad, dss::Fix32Vector3* retVec)
{
    fx32 r;
    int polyNo;
    int surfaceId;
    int type;
    int ret;
    VecFx32 tpos, center, old_center, fixYPos;
    for (int i = 0; i < 14; i++) {
        m_surfaceType[i] = -1;
        m_surfacePolyNo[i] = -1;
    }
    m_searchObjectId = -1;
    m_searchPolyNo = -1;
    tpos = FldStage::getVecFx32(nowPos);
    center = FldStage::getVecFx32(nextPos);
    r = rad.value;
    polyNo = coll_GetNextMoveBox(m_fld->m_coll, &tpos, &center, r, &old_center);
    m_id = polyNo;
    int objNo = coll_GetObjId(m_fld->m_coll, polyNo);
    if (objNo != -1) {
        m_searchObjectId = objNo;
        m_searchPolyNo = polyNo;
    }
    surfaceId = coll_GetSurface(m_fld->m_coll, polyNo);
    if (surfaceId != -1) {
        type = (surfaceId >> 12) & 0xf;
        m_surfaceType[type] = surfaceId;
        m_surfacePolyNo[type] = polyNo;
    }
    ret = coll_SearchFloorPoly(m_fld->m_coll, &old_center, r, &fixYPos);
    if (ret >= 0) {
        m_floorPolygonNo = ret;
        if (coll_GetSurface(m_fld->m_coll, m_floorPolygonNo) == -1 && m_surfaceType[0] == -1) {
            m_surfaceType[0] = 0;
            m_surfacePolyNo[0] = m_floorPolygonNo;
        }
    }
    *retVec = FldStage::getFx32Vector3(old_center);
    return polyNo;
}

ARM bool FldCollision::checkCrossPolygon(VecFx32& pos, VecFx32& pos1, int polyNo)
{
    bool ret = false;
    COLL_HEADER* header = m_fld->m_coll;
    if (header->poly[polyNo].flag & 1) {
        return ret;
    }
    VecFx32 dir;
    func_02062f98(&pos1, &pos, &dir);
    func_020630ec(&dir, &dir);
    fx32 len = func_0206338c(&pos, &pos1);
    fx32 tmp_len;
    if (coll_TriangleIntersect(&pos, &dir, &header->poly[polyNo], 0, &tmp_len, NULL, NULL)) {
        if (tmp_len <= len) {
            ret = true;
        }
    } else if ((header->poly[polyNo].type & 1) == 1) {
        if (coll_TriangleIntersect(&pos, &dir, &header->poly[polyNo], 1, &tmp_len, NULL, NULL)) {
            if (tmp_len <= len) {
                ret = true;
            }
        }
    }
    return ret;
}

ARM int FldCollision::getSurfaceByType(int type)
{
    return m_surfaceType[type];
}

ARM int FldCollision::checkCrossNum(VecFx32& pos0, VecFx32& pos1, int notFloor)
{
    int no = 0;
    int count = 0;
    int floorCount = m_fld->m_coll->floor_poly_size;
    fx32 len;
    while ((no = m_fld->CollCrossCheck(&pos0, &pos1, no, &len)) != -1) {
        if (notFloor == true && no < floorCount) {
            no++;
            continue;
        }
        count++;
        no++;
    }
    return count;
}

ARM int FldCollision::checkCrossNumCheckUnder(VecFx32& pos0, VecFx32& pos1, int notFloor)
{
    int no = 0;
    int count = 0;
    int floorCount = m_fld->m_coll->floor_poly_size;
    fx32 len;
    while ((no = m_fld->CollCrossCheck(&pos0, &pos1, no, &len)) != -1) {
        if (notFloor == true && no < floorCount) {
            no++;
            continue;
        }
        if (len < 0) {
            no++;
        } else {
            count++;
            no++;
        }
    }
    return count;
}

ARM int FldCollision::checkCrossNumEraseSurface(VecFx32 pos0, VecFx32 pos1, int surface, int notFloor, int& polyNo)
{
    int no = 0;
    int count = 0;
    int floorCount = m_fld->m_coll->floor_poly_size;
    fx32 len;
    while ((no = m_fld->CollCrossCheck(&pos0, &pos1, no, &len)) != -1) {
        if (notFloor == true && no < floorCount) {
            no++;
            continue;
        }
        int polySurface = coll_GetSurface(m_fld->m_coll, no);
        if ((polySurface & 0xf000) == surface & 0xf000) {
            no++;
            continue;
        }
        if (len < 0) {
            no++;
        } else {
            polyNo = no;
            count++;
            no++;
        }
    }
    return count;
}

ARM bool FldCollision::getObjectPos(int objectID, int polyNo, dss::Fix32Vector3* pos)
{
    pos->vx = 0L;
    pos->vy = 0L;
    pos->vz = 0L;
    dss::Fix32Vector3 tpos[6];
    dss::Fix32Vector3 center;
    COLL_POLY* collPoly;
    int move = 1;
    int next = 0;
    int no = polyNo;
    int count = 0;
    dss::Fix32Vector3* tp;
    COLL_HEADER* header = m_fld->m_coll;
    int size = header->poly_size;
    if (polyNo == 0) {
        no = coll_GetPolyNoByMapObj(header, objectID, polyNo);
        if (no == -1) {
            return false;
        }
    }
    if (header->poly[no].obj_id != objectID || no >= size) {
        return false;
    }
    if (objectID == -1) {
        return false;
    }
    tp = tpos;
    while (true) {
        collPoly = &header->poly[no];
        if (objectID != collPoly->obj_id || no < 0 || no >= size || next == 2 || count == 4) {
            break;
        }
        center.set(0, 0, 0);
        if (collPoly->type & 1) {
            for (int i = 0; i < 4; i++) {
                center.vx.value += collPoly->vertex[i].x;
                center.vy.value += collPoly->vertex[i].y;
                center.vz.value += collPoly->vertex[i].z;
            }
            center /= 4;
        } else {
            for (int i = 0; i < 3; i++) {
                center.vx.value += collPoly->vertex[i].x;
                center.vy.value += collPoly->vertex[i].y;
                center.vz.value += collPoly->vertex[i].z;
            }
            center /= 3;
        }
        *tp = center;
        tp++;
        no += move;
        count++;
        if (no < size) {
            if (header->poly[no].obj_id != objectID) {
                next++;
                no = polyNo - 1;
                move = -1;
            }
        } else {
            next++;
            no = polyNo - 1;
            move = -1;
        }
    }
    if (count != 0) {
        for (int i = 0; i < count; i++) {
            tpos[i].vx /= count;
            tpos[i].vy /= count;
            tpos[i].vz /= count;
            *pos += tpos[i];
        }
    } else {
        return false;
    }
    return true;
}

ARM void FldCollision::searchFloorSurface(dss::Fix32Vector3& pos, dss::Fix32 radius, dss::Fix32 judgeLen, dss::Fix32Vector3& retVec)
{
    int no;
    int type;
    int tempSurfaceID;
    no = coll_SearchFloorPoly2(m_fld->m_coll, (VecFx32*)&pos, radius.value, 0, judgeLen.value, (VecFx32*)&retVec);
    while (no != -1) {
        tempSurfaceID = coll_GetSurface(m_fld->m_coll, no);
        if (tempSurfaceID != -1) {
            if (isEraseSurfaceId(tempSurfaceID) == false) {
                type = (tempSurfaceID >> 12) & 0xf;
                m_surfaceType[type] = tempSurfaceID;
                m_surfacePolyNo[type] = no;
            }
        } else if (m_surfaceType[0] != 0) {
            m_surfaceType[0] = 0;
            m_surfacePolyNo[0] = m_floorPolygonNo;
        }
        no = coll_SearchFloorPoly2(m_fld->m_coll, (VecFx32*)&pos, radius.value, no + 1, judgeLen.value, (VecFx32*)&retVec);
    }
}

ARM bool FldCollision::isAnimObject(int objectId)
{
    switch (m_fld->GetMapObjCommonId(objectId)) {
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0xf9:
    case 0x11b:
    case 0x11c:
    case 0x11d:
    case 0x11e:
    case 0x11f:
    case 0x120:
        return true;
    }
    return false;
}

ARM void FldCollision::setEraseSurface(int surfaceId, bool flag)
{
    for (int i = 0; i < m_eraseSurfaceCount; i++) {
        if (surfaceId == m_eraseSurfaceId[i]) {
            if (flag == false) {
                m_eraseSurfaceId[i] = -1;
                return;
            }
        } else if (m_eraseSurfaceId[i] == -1 && flag == true) {
            m_eraseSurfaceId[i] = surfaceId;
            return;
        }
    }
    if (flag == true) {
        m_eraseSurfaceId[m_eraseSurfaceCount] = surfaceId;
        m_eraseSurfaceCount++;
    }
}

ARM void FldCollision::resetEraseSurface()
{
    m_eraseSurfaceCount = 0;
}

ARM bool FldCollision::isEraseSurfaceId(int surfaceId)
{
    for (int i = 0; i < m_eraseSurfaceCount; i++) {
        if (surfaceId == m_eraseSurfaceId[i]) {
            return true;
        }
    }
    return false;
}

ARM void FldCollision::wallPolyCheck(const dss::Fix32Vector3& newPos, COLL_POLY* poly, int start, int count)
{
    dss::Fix32Vector3 temp;
    VecFx32* bboxA;
    VecFx32* bboxB;
    VecFx32* normal;
    for (int i = start; i < count; i++) {
        COLL_POLY* p = &poly[i];
        bboxA = &p->bbox[0];
        bboxB = &p->bbox[1];
        normal = &p->normal;
        if (p->flag & 1) {
            continue;
        }
        if (m_newX < MATH_MIN(bboxA->x, bboxB->x) - m_preR || m_newX > m_preR + MATH_MAX(bboxA->x, bboxB->x)) {
            continue;
        }
        if (m_newZ < MATH_MIN(bboxA->z, bboxB->z) - m_preR || m_newZ > m_preR + MATH_MAX(bboxA->z, bboxB->z)) {
            continue;
        }
        if (m_newY < bboxA->y - m_radS || m_newY > bboxB->y + m_radS) {
            continue;
        }
        if (m_collCount < 30) {
            m_nextList[m_collCount] = p;
            m_collPolyNo[m_collCount] = i;
        }
        m_collCount++;
        if (func_02062fcc(&m_dirVec32, normal) < 0) {
            continue;
        }
        if (coll_CheckLinePoint((VecFx32*)&newPos, m_radB, bboxA, bboxB, normal, (VecFx32*)&temp) == 0) {
            continue;
        }
        if (checkSignPoly(newPos, temp, i, p)) {
            m_crossCount++;
        }
    }
}

ARM int FldCollision::getFrontPoly(int polyNo, int objNo)
{
    COLL_POLY coll0;
    coll_GetPoly(m_fld->m_coll, polyNo, &coll0);
    dss::Fix32Vector3 normal0 = FldStage::getFx32Vector3(coll0.normal);
    int no = coll_GetPolyNoByMapObj(m_fld->m_coll, objNo, 0);
    while (no != -1) {
        COLL_POLY coll1;
        coll_GetPoly(m_fld->m_coll, no, &coll1);
        dss::Fix32Vector3 normal1 = FldStage::getFx32Vector3(coll1.normal);
        if ((normal0 * normal1).value < -0xf74) {
            return no;
        }
        no = coll_GetPolyNoByMapObj(m_fld->m_coll, objNo, no + 1);
    }
    return -1;
}
