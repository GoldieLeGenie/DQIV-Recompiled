#include "main/fld/Coll.hpp"
#include "main/dss/DssUtils.hpp"
#include "nnsys/fnd.hpp"

int coll_init(COLL_HEADER* header, NNSFndAllocator* allocator)
{
    COLL_LINE* id_list;
    int i;
    if (header == NULL) {
        return 1;
    }
    if ((unsigned int)header->poly < (unsigned int)header) {
        id_list = (COLL_LINE*)((unsigned int)header + (unsigned int)header->ext_data);
        header->ext_data = (COLL_EXT_DATA*)func_02068a1c(allocator, 8);
        if (header->ext_data == NULL) {
            return 0;
        }
        header->ext_data->id_list = id_list;
        header->ext_data->ext_num = 0;
        header->poly = (COLL_POLY*)((unsigned int)header + (unsigned int)header->poly);
        header->x0 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->x0);
        header->x1 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->x1);
        header->y0 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->y0);
        header->y1 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->y1);
        header->z0 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->z0);
        header->z1 = (COLL_LINE*)((unsigned int)header + (unsigned int)header->z1);
        header->check = (char*)((unsigned int)header + (unsigned int)header->check);
        header->check2 = (char*)((unsigned int)header + (unsigned int)header->check2);
    }
    for (i = 0; i < header->poly_size; i++) {
        header->poly[i].flag = 0;
    }
    return 1;
}

int coll_GetPoly(COLL_HEADER* header, int poly_no, COLL_POLY* poly)
{
    if (header == NULL || poly == NULL) {
        return 0;
    }
    if (poly_no < 0 || poly_no >= header->poly_size + header->ext_data->ext_num) {
        return 0;
    }
    if (poly_no >= header->poly_size) {
        *poly = header->ext_data->ext_coll[poly_no - header->poly_size];
    } else {
        *poly = header->poly[poly_no];
    }
    return 1;
}

int coll_Id2PolyNo(COLL_HEADER* header, int surface_id)
{
    int i;
    if (header == NULL) {
        return -1;
    }
    for (i = 0; i < header->id_size; i++) {
        if (surface_id == header->ext_data->id_list[i].val) {
            return header->ext_data->id_list[i].poly_id;
        }
    }
    for (i = 0; i < header->ext_data->ext_num; i++) {
        if (surface_id == header->ext_data->ext_coll[i].id) {
            return i + header->poly_size;
        }
    }
    return -1;
}

fx32 coll_GetCrossPoint3D(VecFx32* point, VecFx32* vertex, VecFx32* normal, VecFx32* cross)
{
    VecFx32 vec;
    fx32 dot;
    fx32 len;
    func_02062f98(vertex, point, &vec);
    dot = func_02062fcc(normal, &vec);
    if (-normal->y == 0) {
        cross->x = point->x;
        cross->y = point->y;
        cross->z = point->z;
        return 0x7fffffff;
    }
    len = FX_Divide(dot, -normal->y);
    cross->x = point->x;
    cross->y = point->y - len;
    cross->z = point->z;
    return len;
}


int coll_CheckPolyPointOne(COLL_POLY* poly, VecFx32* point)
{
    if (((fx64)poly->vertex[1].x - poly->vertex[0].x) * ((fx64)point->z - poly->vertex[0].z) - ((fx64)point->x - poly->vertex[0].x) * ((fx64)poly->vertex[1].z - poly->vertex[0].z) > 0) {
        return 0;
    }
    if (((fx64)poly->vertex[2].x - poly->vertex[1].x) * ((fx64)point->z - poly->vertex[1].z) - ((fx64)point->x - poly->vertex[1].x) * ((fx64)poly->vertex[2].z - poly->vertex[1].z) > 0) {
        return 0;
    }
    if ((poly->type & 1) != 1) {
        if (((fx64)poly->vertex[0].x - poly->vertex[2].x) * ((fx64)point->z - poly->vertex[2].z) - ((fx64)point->x - poly->vertex[2].x) * ((fx64)poly->vertex[0].z - poly->vertex[2].z) > 0) {
            return 0;
        }
    } else {
        if (((fx64)poly->vertex[3].x - poly->vertex[2].x) * ((fx64)point->z - poly->vertex[2].z) - ((fx64)point->x - poly->vertex[2].x) * ((fx64)poly->vertex[3].z - poly->vertex[2].z) > 0) {
            return 0;
        }
        if (((fx64)poly->vertex[0].x - poly->vertex[3].x) * ((fx64)point->z - poly->vertex[3].z) - ((fx64)point->x - poly->vertex[3].x) * ((fx64)poly->vertex[0].z - poly->vertex[3].z) > 0) {
            return 0;
        }
    }
    return 1;
}

int coll_CheckPolyPoint(COLL_POLY* poly, VecFx32* point)
{
    VecFx32 point1;
    if (coll_CheckPolyPointOne(poly, point)) {
        return 1;
    }
    point1 = *point;
    point1.z += 0xcc;
    if (coll_CheckPolyPointOne(poly, &point1)) {
        return 1;
    }
    point1 = *point;
    point1.x -= 0xcc;
    point1.z -= 0xcc;
    if (coll_CheckPolyPointOne(poly, &point1)) {
        return 1;
    }
    point1 = *point;
    point1.x += 0xcc;
    point1.z -= 0xcc;
    if (coll_CheckPolyPointOne(poly, &point1)) {
        return 1;
    }
    return 0;
}

int coll_CheckLinePoint(const VecFx32* posP, fx32 r, const VecFx32* posA, const VecFx32* posB, const VecFx32* nml, VecFx32* cross)
{
    VecFx32 vec;
    VecFx32 vec1;
    VecFx32 vec2;
    VecFx32 vec3;
    fx32 rr;
    fx32 t;
    if (posA->y > posP->y || posB->y < posP->y - r) {
        return 0;
    }
    rr = FX_Mul(r, r);
    vec.x = posB->x - posA->x;
    vec.z = posB->z - posA->z;
    if (vec.x == 0 && vec.z == 0) {
        return 0;
    }
    vec1.x = posP->x - posA->x;
    vec1.y = 0;
    vec1.z = posP->z - posA->z;
    t = FX_Divide(FX_Mul(vec.x, vec1.x) + FX_Mul(vec.z, vec1.z), FX_Mul(vec.x, vec.x) + FX_Mul(vec.z, vec.z));
    if (t < 0) {
        if (FX_Mul(vec1.x, vec1.x) + FX_Mul(vec1.z, vec1.z) <= rr) {
            *cross = *posA;
        } else {
            return 0;
        }
        if (FX_Mul(vec1.x, nml->x) + FX_Mul(vec1.z, nml->z) < 0) {
            return 0;
        }
    } else if (t > 0x1000) {
        func_02062f98(posP, posB, &vec2);
        if (FX_Mul(vec2.x, vec2.x) + FX_Mul(vec2.z, vec2.z) <= rr) {
            *cross = *posB;
        } else {
            return 0;
        }
        if (FX_Mul(vec2.x, nml->x) + FX_Mul(vec2.z, nml->z) < 0) {
            return 0;
        }
    } else {
        vec2.x = FX_Mul(t, vec.x);
        vec2.y = 0;
        vec2.z = FX_Mul(t, vec.z);
        func_02062f64(posA, &vec2, cross);
        cross->y = posP->y;
        func_02062f98(&vec2, &vec1, &vec3);
        if (FX_Mul(vec3.x, vec3.x) + FX_Mul(vec3.z, vec3.z) > rr) {
            return 0;
        }
        if (FX_Mul(vec3.x, nml->x) + FX_Mul(vec3.z, nml->z) > 0) {
            return 0;
        }
    }
    return 1;
}

int coll_PreSearchWallPoly(COLL_HEADER* header, VecFx32* point0, VecFx32* point1)
{
    char* ptr;
    unsigned short* cl_ptr;
    unsigned short* cl_end;
    int size;
    int ret;
    if (header == NULL) {
        return 0;
    }
    size = header->poly_size;
    ptr = header->check;
    MI_CpuSet(ptr, 0, size);
    ret = coll_GetCollLinePosL(header->x0, size, point0->x >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x0[ret].poly_id;
    cl_end = &header->x0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 1;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->x1, size, point1->x >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x1[ret].poly_id;
    cl_end = &header->x1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 2;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosL(header->z0, size, point0->z >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z0[ret].poly_id;
    cl_end = &header->z0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 4;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->z1, size, point1->z >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z1[ret].poly_id;
    cl_end = &header->z1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 8;
        cl_ptr += 2;
    }
    return 1;
}

int coll_CheckWallNo(COLL_HEADER* header, VecFx32* center, fx32 r, int start, VecFx32* ret)
{
    int FloorPolySize;
    int Size;
    int i;
    char* ptr;
    if (header == NULL) {
        return -1;
    }
    if (start == 0) {
        VecFx32 p0;
        VecFx32 p1;
        p0.x = center->x - r;
        p1.x = center->x + r;
        p0.z = center->z - r;
        p1.z = center->z + r;
        if (header->check_point[0].x > p0.x || header->check_point[0].z > p0.z || header->check_point[1].x < p1.x || header->check_point[1].z < p1.z) {
            p0.x -= r;
            p1.x += r;
            p0.z -= r;
            p1.z += r;
            p0.y = 0;
            p1.y = 0;
            header->check_point[0] = p0;
            header->check_point[1] = p1;
            coll_PreSearchWallPoly(header, &p0, &p1);
        }
    }
    if (start < 0) {
        start = 0;
    }
    FloorPolySize = header->floor_poly_size;
    Size = FloorPolySize + (header->wall_poly_size + header->common_poly_size);
    if (FloorPolySize < start) {
        FloorPolySize = start;
    }
    ptr = &header->check[FloorPolySize];
    for (i = FloorPolySize; i < Size; i++) {
        if ((*ptr++ & 0xf) == 0xf || (header->poly[i].flag & 2)) {
            if (!(header->poly[i].flag & 1)) {
                if (coll_CheckLinePoint(center, r, &header->poly[i].bbox[0], &header->poly[i].bbox[1], &header->poly[i].normal, ret)) {
                    return i;
                }
            }
        }
    }
    i = FloorPolySize - header->poly_size;
    if (i < 0) {
        i = 0;
    }
    for (; i < header->ext_data->ext_num; i++) {
        if (!(header->ext_data->ext_coll[i].flag & 1)) {
            if (coll_CheckLinePoint(center, r, &header->ext_data->ext_coll[i].bbox[0], &header->ext_data->ext_coll[i].bbox[1], &header->ext_data->ext_coll[i].normal, ret)) {
                return i + header->poly_size;
            }
        }
    }
    return -1;
}

int coll_GetCollLinePosL(COLL_LINE* line, int size, short val)
{
    int step = size / 2;
    int pos = step;
    do {
        if (val > line[pos].val) {
            if (step == -1) {
                return pos + 1;
            }
            if (step < -1) {
                step = ((step ^ -1) + 1) >> 1;
            } else {
                step >>= 1;
                if (step == 0) {
                    step = 1;
                }
            }
        } else if (val < line[pos].val) {
            if (step == 1) {
                return pos;
            }
            if (step > 1) {
                step = ((step ^ -1) + 2) >> 1;
            } else {
                step = (step + 1) >> 1;
                if (step == 0) {
                    step = -1;
                }
            }
        } else {
            step = -1;
        }
        pos += step;
        if (pos < 0) {
            return 0;
        }
    } while (pos < size);
    return -1;
}

int coll_GetCollLinePosG(COLL_LINE* line, int size, short val)
{
    int step = size / 2;
    int pos = step;
    do {
        if (val < line[pos].val) {
            if (step == -1) {
                return pos + 1;
            }
            if (step < -1) {
                step = ((step ^ -1) + 1) >> 1;
            } else {
                step >>= 1;
                if (step == 0) {
                    step = 1;
                }
            }
        } else if (val > line[pos].val) {
            if (step == 1) {
                return pos;
            }
            if (step > 1) {
                step = ((step ^ -1) + 2) >> 1;
            } else {
                step = (step + 1) >> 1;
                if (step == 0) {
                    step = -1;
                }
            }
        } else {
            step = -1;
        }
        pos += step;
        if (pos < 0) {
            return 0;
        }
    } while (pos < size);
    return -1;
}

int coll_PreSearchFloorPoly(COLL_HEADER* header, VecFx32* point)
{
    char* ptr;
    unsigned short* cl_ptr;
    unsigned short* cl_end;
    int size;
    int ret;
    short x;
    short z;
    if (header == NULL) {
        return 0;
    }
    size = header->poly_size;
    x = point->x >> 7;
    z = point->z >> 7;
    ptr = header->check;
    MI_CpuSet(ptr, 0, size);
    ret = coll_GetCollLinePosL(header->x0, size, x);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x0[ret].poly_id;
    cl_end = &header->x0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 1;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->x1, size, x);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x1[ret].poly_id;
    cl_end = &header->x1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 2;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosL(header->z0, size, z);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z0[ret].poly_id;
    cl_end = &header->z0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 4;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->z1, size, z);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z1[ret].poly_id;
    cl_end = &header->z1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 8;
        cl_ptr += 2;
    }
    return 1;
}

int coll_SearchFloorPoly(COLL_HEADER* header, VecFx32* point, fx32 height, VecFx32* ret)
{
    char* ptr;
    int no;
    int size;
    int i;
    fx32 min;
    fx32 len;
    VecFx32 cross;
    VecFx32 org_point;
    min = 0x7fffffff;
    if (header == NULL) {
        return -1;
    }
    if (point == NULL) {
        return -1;
    }
    if (ret != NULL) {
        *ret = *point;
    }
    org_point = *point;
    if (header->check_point[0].x > point->x || header->check_point[0].z > point->z || header->check_point[1].x < point->x || header->check_point[1].z < point->z) {
        header->check_point[0] = *point;
        header->check_point[1] = *point;
        coll_PreSearchFloorPoly(header, &org_point);
    }
    size = header->floor_poly_size;
    ptr = header->check;
    for (i = 0; i < size; i++) {
        if (header->poly[i].flag & 1) {
            ptr++;
        } else if ((*ptr++ & 0xf) == 0xf || (header->poly[i].flag & 2)) {
            if (coll_CheckPolyPoint(&header->poly[i], &org_point)) {
                len = coll_GetCrossPoint3D(&org_point, &header->poly[i].vertex[0], &header->poly[i].normal, &cross);
                if (len >= 0 && min > len) {
                    min = len;
                    no = i;
                    if (ret != NULL) {
                        *ret = cross;
                    }
                }
            }
        }
    }
    if (min == 0x7fffffff) {
        return -1;
    }
    if (ret != NULL) {
        ret->y += height;
    }
    return no;
}

int coll_GetNextMove(COLL_HEADER* header, VecFx32* old_center, VecFx32* center, fx32 r, VecFx32* ret)
{
    VecFx32 cross;
    VecFx32 normal;
    VecFx32 min_cross;
    fx32 rr;
    fx32 len;
    int start;
    fx32 min;
    int counter;
    int no;
    int min_poly_no;
    min_poly_no = -1;
    counter = 0;
    if (header == NULL || old_center == NULL || center == NULL || ret == NULL) {
        return -1;
    }
    *ret = *center;
    rr = FX_Mul(r, r);
    while (1) {
        if (counter++ >= 2) {
            if ((rr - min) >> 4) {
                *ret = *old_center;
            }
            return min_poly_no;
        }
        min = rr;
        start = 0;
        if (counter > 1) {
            start--;
        }
        while (1) {
            no = coll_CheckWallNo(header, ret, r, start, &cross);
            if (no == -1) {
                if (start == 0 || min == rr) {
                    return min_poly_no;
                }
                func_02062f98(ret, &min_cross, &normal);
                normal.y = 0;
                if (normal.x == 0 && normal.z == 0) {
                    *ret = min_cross;
                } else {
                    func_020630ec(&normal, &normal);
                    func_02063330(r, &normal, &min_cross, ret);
                }
                break;
            }
            len = FX_Mul(cross.x - ret->x, cross.x - ret->x) + FX_Mul(cross.z - ret->z, cross.z - ret->z);
            if (min > len) {
                min = len;
                min_poly_no = no;
                min_cross = cross;
                min_cross.y = old_center->y;
            }
            start = no + 1;
        }
    }
}

int coll_GetObjId(COLL_HEADER* header, int poly_no)
{
    if (header == NULL) {
        return -1;
    }
    if (poly_no < 0 || poly_no >= header->poly_size + header->ext_data->ext_num) {
        return -1;
    }
    if (poly_no >= header->poly_size) {
        return header->ext_data->ext_coll[poly_no - header->poly_size].obj_id;
    }
    return header->poly[poly_no].obj_id;
}

int coll_GetSurface(COLL_HEADER* header, int poly_no)
{
    if (header == NULL) {
        return -1;
    }
    if (poly_no < 0 || poly_no >= header->poly_size + header->ext_data->ext_num) {
        return -1;
    }
    if (poly_no >= header->poly_size) {
        return header->ext_data->ext_coll[poly_no - header->poly_size].id;
    }
    return header->poly[poly_no].id;
}

void coll_EraseObjId(COLL_HEADER* header, int obj_id)
{
    int Size;
    int i;
    if (header == NULL) {
        return;
    }
    if (obj_id < 0) {
        return;
    }
    Size = header->poly_size;
    for (i = 0; i < Size; i++) {
        if (obj_id == header->poly[i].obj_id) {
            header->poly[i].flag |= 1;
        }
    }
    Size = header->ext_data->ext_num;
    for (i = 0; i < Size; i++) {
        if (obj_id == header->ext_data->ext_coll[i].obj_id) {
            header->ext_data->ext_coll[i].flag |= 1;
        }
    }
}

void coll_ResetObjId(COLL_HEADER* header, int obj_id)
{
    int Size;
    int i;
    if (header == NULL) {
        return;
    }
    if (obj_id < 0) {
        return;
    }
    Size = header->poly_size;
    for (i = 0; i < Size; i++) {
        if (obj_id == header->poly[i].obj_id) {
            header->poly[i].flag &= ~1;
        }
    }
    Size = header->ext_data->ext_num;
    for (i = 0; i < Size; i++) {
        if (obj_id == header->ext_data->ext_coll[i].obj_id) {
            header->ext_data->ext_coll[i].flag &= ~1;
        }
    }
}

int coll_GetPolyNoBySurface(COLL_HEADER* header, int surface_id, int start)
{
    int i;
    if (header == NULL || surface_id == -1 || surface_id == 0) {
        return -1;
    }
    if (start < 0) {
        start = 0;
    }
    for (i = start; i < header->poly_size; i++) {
        if (surface_id == header->poly[i].id) {
            return i;
        }
    }
    i = start - header->poly_size;
    if (i < 0) {
        i = 0;
    }
    for (; i < header->ext_data->ext_num; i++) {
        if (surface_id == header->ext_data->ext_coll[i].id) {
            return i + header->poly_size;
        }
    }
    return -1;
}

int coll_GetPolyNoByMapObj(COLL_HEADER* header, int obj_id, int start)
{
    int i;
    if (header == NULL) {
        return -1;
    }
    if (obj_id < 0) {
        return -1;
    }
    if (start < 0) {
        start = 0;
    }
    for (i = start; i < header->poly_size; i++) {
        if (obj_id == header->poly[i].obj_id) {
            return i;
        }
    }
    i = start - header->poly_size;
    if (i < 0) {
        i = 0;
    }
    for (; i < header->ext_data->ext_num; i++) {
        if (obj_id == header->ext_data->ext_coll[i].obj_id) {
            return i + header->poly_size;
        }
    }
    return -1;
}

fx32 get_xz_len(VecFx32* a, VecFx32* b)
{
    fx32 x = a->x - b->x;
    fx32 z = a->z - b->z;
    return FX_Sqrt(FX_Mul(x, x) + FX_Mul(z, z));
}

void coll_MovePolyPos(COLL_HEADER* header, int poly_no, COLL_POLY* new_poly)
{
    COLL_POLY* move_poly;
    int k;
    fx32 min_x, min_y, min_z;
    fx32 max_x, max_y, max_z;
    if (header == NULL) {
        return;
    }
    if (poly_no < 0) {
        return;
    }
    if (poly_no >= header->poly_size + header->ext_data->ext_num) {
        return;
    }
    if (poly_no >= header->poly_size) {
        move_poly = &header->ext_data->ext_coll[poly_no - header->poly_size];
    } else {
        move_poly = &header->poly[poly_no];
    }
    if (!(move_poly->type & 1)) {
        if (move_poly->type & 0x300) {
            fx32 len01 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[1]);
            fx32 len12 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[2]);
            fx32 len20 = get_xz_len(&new_poly->vertex[2], &new_poly->vertex[0]);
            if (len01 >= len12 && len01 >= len20) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[1].x;
                max_z = new_poly->vertex[1].z;
            } else if (len12 >= len20) {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else {
                min_x = new_poly->vertex[2].x;
                min_z = new_poly->vertex[2].z;
                max_x = new_poly->vertex[0].x;
                max_z = new_poly->vertex[0].z;
            }
            if (min_x > max_x) {
                min_y = min_x;
                min_x = max_x;
                max_x = min_y;
                min_y = min_z;
                min_z = max_z;
                max_z = min_y;
            }
            max_y = min_y = new_poly->vertex[0].y;
            for (int k = 1; k < 3; k++) {
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                }
                if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
            }
        } else {
            max_x = min_x = new_poly->vertex[0].x;
            max_y = min_y = new_poly->vertex[0].y;
            max_z = min_z = new_poly->vertex[0].z;
            for (k = 1; k < 3; k++) {
                if (min_x > new_poly->vertex[k].x) {
                    min_x = new_poly->vertex[k].x;
                } else if (max_x < new_poly->vertex[k].x) {
                    max_x = new_poly->vertex[k].x;
                }
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                } else if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
                if (min_z > new_poly->vertex[k].z) {
                    min_z = new_poly->vertex[k].z;
                } else if (max_z < new_poly->vertex[k].z) {
                    max_z = new_poly->vertex[k].z;
                }
            }
        }
    } else {
        if (move_poly->type & 0x300) {
            fx32 len01 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[1]);
            fx32 len12 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[2]);
            fx32 len23 = get_xz_len(&new_poly->vertex[2], &new_poly->vertex[3]);
            fx32 len30 = get_xz_len(&new_poly->vertex[3], &new_poly->vertex[0]);
            fx32 len02 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[2]);
            fx32 len13 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[3]);
            if (len01 >= len12 && len01 >= len23 && len01 >= len30 && len01 >= len02 && len01 >= len13) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[1].x;
                max_z = new_poly->vertex[1].z;
            } else if (len12 >= len23 && len12 >= len30 && len12 >= len02 && len12 >= len13) {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else if (len23 >= len30 && len23 >= len02 && len23 >= len13) {
                min_x = new_poly->vertex[2].x;
                min_z = new_poly->vertex[2].z;
                max_x = new_poly->vertex[3].x;
                max_z = new_poly->vertex[3].z;
            } else if (len30 >= len02 && len30 >= len13) {
                min_x = new_poly->vertex[3].x;
                min_z = new_poly->vertex[3].z;
                max_x = new_poly->vertex[0].x;
                max_z = new_poly->vertex[0].z;
            } else if (len02 >= len13) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[3].x;
                max_z = new_poly->vertex[3].z;
            }
            if (min_x > max_x) {
                min_y = min_x;
                min_x = max_x;
                max_x = min_y;
                min_y = min_z;
                min_z = max_z;
                max_z = min_y;
            }
            max_y = min_y = new_poly->vertex[0].y;
            for (int k = 1; k < 4; k++) {
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                }
                if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
            }
        } else {
            max_x = min_x = new_poly->vertex[0].x;
            max_y = min_y = new_poly->vertex[0].y;
            max_z = min_z = new_poly->vertex[0].z;
            for (k = 1; k < 4; k++) {
                if (min_x > new_poly->vertex[k].x) {
                    min_x = new_poly->vertex[k].x;
                } else if (max_x < new_poly->vertex[k].x) {
                    max_x = new_poly->vertex[k].x;
                }
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                } else if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
                if (min_z > new_poly->vertex[k].z) {
                    min_z = new_poly->vertex[k].z;
                } else if (max_z < new_poly->vertex[k].z) {
                    max_z = new_poly->vertex[k].z;
                }
            }
        }
    }
    move_poly->bbox[0].x = min_x;
    move_poly->bbox[0].y = min_y;
    move_poly->bbox[0].z = min_z;
    move_poly->bbox[1].x = max_x;
    move_poly->bbox[1].y = max_y;
    move_poly->bbox[1].z = max_z;
    {
        VecFx32 vec01;
        VecFx32 vec12;
        func_02062f98(&new_poly->vertex[1], &new_poly->vertex[0], &vec01);
        func_02062f98(&new_poly->vertex[2], &new_poly->vertex[1], &vec12);
        func_02063008(&vec01, &vec12, &move_poly->normal);
        func_020630ec(&move_poly->normal, &move_poly->normal);
    }
    header->check_point[0].x = 0x7fffffff;
    header->check_point[0].y = 0x7fffffff;
    header->check_point[0].z = 0x7fffffff;
    header->check_point[1].x = 0x7fffffff;
    header->check_point[1].y = 0x7fffffff;
    header->check_point[1].z = 0x7fffffff;
    move_poly->flag |= 2;
    CpuFastSet(new_poly, move_poly, 12);
}

void coll_AddPolyPos(COLL_HEADER* header, int poly_no, VecFx32* add_vec)
{
    COLL_POLY new_poly;
    COLL_POLY* org_poly;
    if (header == NULL) {
        return;
    }
    if (poly_no < 0) {
        return;
    }
    if (poly_no >= header->poly_size + header->ext_data->ext_num) {
        return;
    }
    if (poly_no >= header->poly_size) {
        org_poly = &header->ext_data->ext_coll[poly_no - header->poly_size];
    } else {
        org_poly = &header->poly[poly_no];
    }
    func_02062f64(&org_poly->vertex[0], add_vec, &new_poly.vertex[0]);
    func_02062f64(&org_poly->vertex[1], add_vec, &new_poly.vertex[1]);
    func_02062f64(&org_poly->vertex[2], add_vec, &new_poly.vertex[2]);
    if (org_poly->type & 1) {
        func_02062f64(&org_poly->vertex[3], add_vec, &new_poly.vertex[3]);
    }
    coll_MovePolyPos(header, poly_no, &new_poly);
}

int coll_PreSearchPoly(COLL_HEADER* header, VecFx32* point0, VecFx32* point1)
{
    char* ptr;
    unsigned short* cl_ptr;
    unsigned short* cl_end;
    int size;
    int ret;
    if (header == NULL) {
        return 0;
    }
    size = header->poly_size;
    ptr = header->check2;
    MI_CpuSet(ptr, 0, size);
    ret = coll_GetCollLinePosL(header->x0, size, point0->x >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x0[ret].poly_id;
    cl_end = &header->x0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 1;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->x1, size, point1->x >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->x1[ret].poly_id;
    cl_end = &header->x1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 2;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosL(header->z0, size, point0->z >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z0[ret].poly_id;
    cl_end = &header->z0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 4;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->z1, size, point1->z >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->z1[ret].poly_id;
    cl_end = &header->z1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 8;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosL(header->y0, size, point0->y >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->y0[ret].poly_id;
    cl_end = &header->y0[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 0x10;
        cl_ptr += 2;
    }
    ret = coll_GetCollLinePosG(header->y1, size, point1->y >> 7);
    if (ret < 0) {
        return 0;
    }
    cl_ptr = &header->y1[ret].poly_id;
    cl_end = &header->y1[size].poly_id;
    while (cl_ptr != cl_end) {
        ptr[*cl_ptr] |= 0x20;
        cl_ptr += 2;
    }
    return 1;
}

int coll_TriangleIntersect(VecFx32* pos, VecFx32* dir, COLL_POLY* poly, int flag, fx32* ret_t, fx32* ret_u, fx32* ret_v)
{
    VecFx32 e1, e2, pvec, tvec, qvec;
    VecFx32* v0;
    VecFx32* v1;
    VecFx32* v2;
    fx32 det;
    fx32 u;
    fx32 v;
    fx32 inv_det;
    if (flag == 0) {
        v0 = &poly->vertex[0];
        v1 = &poly->vertex[1];
        v2 = &poly->vertex[2];
    } else {
        v0 = &poly->vertex[2];
        v1 = &poly->vertex[3];
        v2 = &poly->vertex[0];
    }
    func_02062f98(v1, v0, &e1);
    func_02062f98(v2, v0, &e2);
    func_02063008(dir, &e2, &pvec);
    det = func_02062fcc(&e1, &pvec);
    if (det > 4) {
        func_02062f98(pos, v0, &tvec);
        u = func_02062fcc(&tvec, &pvec);
        if (u < 0 || u > det) {
            return 0;
        }
        func_02063008(&tvec, &e1, &qvec);
        v = func_02062fcc(dir, &qvec);
        if (v < 0 || u + v > det) {
            return 0;
        }
    } else if (det < -4) {
        func_02062f98(pos, v0, &tvec);
        u = func_02062fcc(&tvec, &pvec);
        if (u > 0 || u < det) {
            return 0;
        }
        func_02063008(&tvec, &e1, &qvec);
        v = func_02062fcc(dir, &qvec);
        if (v > 0 || u + v < det) {
            return 0;
        }
    } else {
        return 0;
    }
    inv_det = FX_Divide(0x1000, det);
    if (ret_t != NULL) {
        *ret_t = FX_Mul(func_02062fcc(&e2, &qvec), inv_det);
    }
    if (ret_u != NULL) {
        *ret_u = FX_Mul(u, inv_det);
    }
    if (ret_v != NULL) {
        *ret_v = FX_Mul(v, inv_det);
    }
    return 1;
}

int coll_CrossCheck(COLL_HEADER* header, VecFx32* pos, VecFx32* dir, fx32 len, int start, fx32* ret_len)
{
    fx32 tmp_len;
    char* ptr;
    int Size;
    int i;
    if (header == NULL || pos == NULL || dir == NULL) {
        return -1;
    }
    if (start == 0) {
        VecFx32 p0;
        VecFx32 p1;
        fx32 tmp;
        p0 = *pos;
        func_02063330(len, dir, pos, &p1);
        if (p0.x > p1.x) {
            tmp = p0.x;
            p0.x = p1.x;
            p1.x = tmp;
        }
        if (p0.y > p1.y) {
            tmp = p0.y;
            p0.y = p1.y;
            p1.y = tmp;
        }
        if (p0.z > p1.z) {
            tmp = p0.z;
            p0.z = p1.z;
            p1.z = tmp;
        }
        coll_PreSearchPoly(header, &p0, &p1);
    }
    if (start < 0) {
        start = 0;
    }
    Size = header->poly_size;
    ptr = &header->check2[start];
    for (i = start; i < Size; i++) {
        if ((*ptr++ & 0x3f) == 0x3f || (header->poly[i].flag & 2)) {
            if (!(header->poly[i].flag & 1)) {
                if (coll_TriangleIntersect(pos, dir, &header->poly[i], 0, &tmp_len, NULL, NULL)) {
                    if (tmp_len <= len) {
                        if (ret_len != NULL) {
                            *ret_len = tmp_len;
                        }
                        return i;
                    }
                } else if ((header->poly[i].type & 1) == 1) {
                    if (coll_TriangleIntersect(pos, dir, &header->poly[i], 1, &tmp_len, NULL, NULL)) {
                        if (tmp_len <= len) {
                            if (ret_len != NULL) {
                                *ret_len = tmp_len;
                            }
                            return i;
                        }
                    }
                }
            }
        }
    }
    i = start - header->poly_size;
    if (i < 0) {
        i = 0;
    }
    for (; i < header->ext_data->ext_num; i++) {
        if (!(header->ext_data->ext_coll[i].flag & 1)) {
            if (coll_TriangleIntersect(pos, dir, &header->ext_data->ext_coll[i], 0, &tmp_len, NULL, NULL)) {
                if (tmp_len <= len) {
                    if (ret_len != NULL) {
                        *ret_len = tmp_len;
                    }
                    return i + header->poly_size;
                }
            } else if ((header->ext_data->ext_coll[i].type & 1) == 1) {
                if (coll_TriangleIntersect(pos, dir, &header->ext_data->ext_coll[i], 1, &tmp_len, NULL, NULL)) {
                    if (tmp_len <= len) {
                        if (ret_len != NULL) {
                            *ret_len = tmp_len;
                        }
                        return i + header->poly_size;
                    }
                }
            }
        }
    }
    if (ret_len != NULL) {
        *ret_len = 0x7fffffff;
    }
    return -1;
}

int coll_SearchFloorPoly2(COLL_HEADER* header, VecFx32* point, fx32 height, int start, fx32 judgeLen, VecFx32* ret)
{
    VecFx32 cross;
    VecFx32 org_point;
    fx32 len;
    int size;
    if (header == NULL) {
        return -1;
    }
    if (point == NULL) {
        return -1;
    }
    if (ret != NULL) {
        *ret = *point;
    }
    org_point = *point;
    size = header->floor_poly_size;
    for (int i = start; i < size; i++) {
        if (collCheckA(&header->poly[i].bbox[0], &header->poly[i].bbox[1], point)) {
            if (!(header->poly[i].flag & 1)) {
                len = coll_GetCrossPoint3D(&org_point, &header->poly[i].vertex[0], &header->poly[i].normal, &cross);
                if (coll_CheckPolyPoint(&header->poly[i], &org_point) && len >= 0 && len <= height) {
                    if (ret != NULL) {
                        *ret = cross;
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

int collCheckA(VecFx32* bboxA, VecFx32* bboxB, VecFx32* point)
{
    if (point->x < MATH_MIN(bboxA->x, bboxB->x) || point->x > MATH_MAX(bboxA->x, bboxB->x) || point->z < MATH_MIN(bboxA->z, bboxB->z) || point->z > MATH_MAX(bboxA->z, bboxB->z)) {
        return 0;
    }
    return 1;
}

COLL_ADD_RESULT_TYPE coll_AddCollPoly2(int extraNo, int polyNo, COLL_HEADER* header, COLL_POLY* new_poly, NNSFndAllocator* allocator, int& allocFlag)
{
    COLL_EXT_DATA* ext_data;
    COLL_POLY* add_poly;
    int no;
    int k;
    int l;
    fx32 min_x, min_y, min_z;
    fx32 max_x, max_y, max_z;
    if (allocFlag == 1) {
        ext_data = (COLL_EXT_DATA*)func_02068a1c(allocator, 0xc08);
        if (ext_data == NULL) {
            return MEMORY_ALLOC_ERROR;
        }
        for (int i = 0; i < 32; i++) {
            ext_data->ext_coll[i].flag |= 1;
        }
        ext_data->ext_num = 0;
        func_02068a30(allocator, header->ext_data);
        header->ext_data = ext_data;
        allocFlag = 0;
    }
    no = -1;
    if (extraNo == -1) {
        header->ext_data->ext_num++;
        if (header->ext_data->ext_num >= 32) {
            return ALLOC_MAX_OVER_ERROR;
        }
        header->ext_data->ext_coll[header->ext_data->ext_num - 1] = *new_poly;
        add_poly = &header->ext_data->ext_coll[header->ext_data->ext_num - 1];
    } else {
        for (int i = 0; i < header->ext_data->ext_num; i++) {
            if (extraNo == header->ext_data->ext_coll[i].obj_id) {
                no = i + polyNo;
                if (header->ext_data->ext_num <= no) {
                    return NUM_MAX_OVER_ERROR;
                }
                break;
            }
        }
        if (no != -1) {
            header->ext_data->ext_coll[no] = *new_poly;
            add_poly = &header->ext_data->ext_coll[no];
        } else {
            header->ext_data->ext_num++;
            if (header->ext_data->ext_num >= 32) {
                return ALLOC_MAX_OVER_ERROR;
            }
            header->ext_data->ext_coll[header->ext_data->ext_num - 1] = *new_poly;
            add_poly = &header->ext_data->ext_coll[header->ext_data->ext_num - 1];
        }
    }
    if (!(add_poly->type & 1)) {
        l = 3;
        if (add_poly->type & 0x300) {
            fx32 len01 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[1]);
            fx32 len12 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[2]);
            fx32 len20 = get_xz_len(&new_poly->vertex[2], &new_poly->vertex[0]);
            if (len01 >= len12 && len01 >= len20) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[1].x;
                max_z = new_poly->vertex[1].z;
            } else if (len12 >= len20) {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else {
                min_x = new_poly->vertex[2].x;
                min_z = new_poly->vertex[2].z;
                max_x = new_poly->vertex[0].x;
                max_z = new_poly->vertex[0].z;
            }
            if (min_x > max_x) {
                min_y = min_x;
                min_x = max_x;
                max_x = min_y;
                min_y = min_z;
                min_z = max_z;
                max_z = min_y;
            }
            max_y = min_y = new_poly->vertex[0].y;
            for (int k = 1; k < 3; k++) {
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                }
                if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
            }
        } else {
            max_x = min_x = new_poly->vertex[0].x;
            max_y = min_y = new_poly->vertex[0].y;
            max_z = min_z = new_poly->vertex[0].z;
            for (k = 1; k < 3; k++) {
                if (min_x > new_poly->vertex[k].x) {
                    min_x = new_poly->vertex[k].x;
                } else if (max_x < new_poly->vertex[k].x) {
                    max_x = new_poly->vertex[k].x;
                }
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                } else if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
                if (min_z > new_poly->vertex[k].z) {
                    min_z = new_poly->vertex[k].z;
                } else if (max_z < new_poly->vertex[k].z) {
                    max_z = new_poly->vertex[k].z;
                }
            }
        }
    } else {
        l = 4;
        if (add_poly->type & 0x300) {
            fx32 len01 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[1]);
            fx32 len12 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[2]);
            fx32 len23 = get_xz_len(&new_poly->vertex[2], &new_poly->vertex[3]);
            fx32 len30 = get_xz_len(&new_poly->vertex[3], &new_poly->vertex[0]);
            fx32 len02 = get_xz_len(&new_poly->vertex[0], &new_poly->vertex[2]);
            fx32 len13 = get_xz_len(&new_poly->vertex[1], &new_poly->vertex[3]);
            if (len01 >= len12 && len01 >= len23 && len01 >= len30 && len01 >= len02 && len01 >= len13) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[1].x;
                max_z = new_poly->vertex[1].z;
            } else if (len12 >= len23 && len12 >= len30 && len12 >= len02 && len12 >= len13) {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else if (len23 >= len30 && len23 >= len02 && len23 >= len13) {
                min_x = new_poly->vertex[2].x;
                min_z = new_poly->vertex[2].z;
                max_x = new_poly->vertex[3].x;
                max_z = new_poly->vertex[3].z;
            } else if (len30 >= len02 && len30 >= len13) {
                min_x = new_poly->vertex[3].x;
                min_z = new_poly->vertex[3].z;
                max_x = new_poly->vertex[0].x;
                max_z = new_poly->vertex[0].z;
            } else if (len02 >= len13) {
                min_x = new_poly->vertex[0].x;
                min_z = new_poly->vertex[0].z;
                max_x = new_poly->vertex[2].x;
                max_z = new_poly->vertex[2].z;
            } else {
                min_x = new_poly->vertex[1].x;
                min_z = new_poly->vertex[1].z;
                max_x = new_poly->vertex[3].x;
                max_z = new_poly->vertex[3].z;
            }
            if (min_x > max_x) {
                min_y = min_x;
                min_x = max_x;
                max_x = min_y;
                min_y = min_z;
                min_z = max_z;
                max_z = min_y;
            }
            max_y = min_y = new_poly->vertex[0].y;
            for (int k = 1; k < 4; k++) {
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                }
                if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
            }
        } else {
            max_x = min_x = new_poly->vertex[0].x;
            max_y = min_y = new_poly->vertex[0].y;
            max_z = min_z = new_poly->vertex[0].z;
            for (k = 1; k < 4; k++) {
                if (min_x > new_poly->vertex[k].x) {
                    min_x = new_poly->vertex[k].x;
                } else if (max_x < new_poly->vertex[k].x) {
                    max_x = new_poly->vertex[k].x;
                }
                if (min_y > new_poly->vertex[k].y) {
                    min_y = new_poly->vertex[k].y;
                } else if (max_y < new_poly->vertex[k].y) {
                    max_y = new_poly->vertex[k].y;
                }
                if (min_z > new_poly->vertex[k].z) {
                    min_z = new_poly->vertex[k].z;
                } else if (max_z < new_poly->vertex[k].z) {
                    max_z = new_poly->vertex[k].z;
                }
            }
        }
    }
    add_poly->bbox[0].x = min_x;
    add_poly->bbox[0].y = min_y;
    add_poly->bbox[0].z = min_z;
    add_poly->bbox[1].x = max_x;
    add_poly->bbox[1].y = max_y;
    add_poly->bbox[1].z = max_z;
    if (add_poly->type & 0x300) {
        max_z = min_z = new_poly->vertex[0].z;
        for (k = 1; k < l; k++) {
            if (min_z > new_poly->vertex[k].z) {
                min_z = new_poly->vertex[k].z;
            } else if (max_z < new_poly->vertex[k].z) {
                max_z = new_poly->vertex[k].z;
            }
        }
    }
    return RESULT_OK;
}
