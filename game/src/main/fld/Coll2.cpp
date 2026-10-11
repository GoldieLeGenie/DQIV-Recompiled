#include "main/fld/Coll.hpp"
#include "main/dss/DssUtils.hpp"

static inline fx32 _Abs(fx32 x)
{
    return x < 0 ? -x : x;
}

static int _line_hit_Z(fx32 x0, fx32 z0, fx32 dz, COLL_POLY* poly)
{
    fx32 rx = poly->bbox[0].x;
    fx32 rz = poly->bbox[0].z;
    fx32 nx = poly->bbox[1].x;
    fx32 nz = poly->bbox[1].z;
    rx -= x0; nx -= x0;
    rz -= z0; nz -= z0;
    if ((rx > 0 && nx > 0) || (rx < 0 && nx < 0)) {
        return 0;
    }
    nx -= rx;
    nz -= rz;
    rz = FX_Mul(rz, nx) - FX_Mul(rx, nz);
    nx = FX_Mul(dz, nx);
    if (nx > 0) {
        return (rz > 0) & (rz < nx);
    }
    return (rz < 0) & (rz > nx);
}

static int _line_hit_X(fx32 x0, fx32 z0, fx32 dx, COLL_POLY* poly)
{
    fx32 rx = poly->bbox[0].x;
    fx32 rz = poly->bbox[0].z;
    fx32 nx = poly->bbox[1].x;
    fx32 nz = poly->bbox[1].z;
    rx -= x0; nx -= x0;
    rz -= z0; nz -= z0;
    if ((rz > 0 && nz > 0) || (rz < 0 && nz < 0)) {
        return 0;
    }
    nx -= rx;
    nz -= rz;
    rx = FX_Mul(rz, nx) - FX_Mul(rx, nz);
    nz = -FX_Mul(dx, nz);
    if (nz > 0) {
        return (rx > 0) & (rx < nz);
    }
    return (rx < 0) & (rx > nz);
}

static int _check_box(VecFx32* pos, fx32 r, COLL_POLY* poly)
{
    fx32 lxz = r * 2;
    if (_Abs(poly->bbox[0].x - pos->x) < r && _Abs(poly->bbox[0].z - pos->z) < r) {
        return 0x10;
    }
    if (_Abs(poly->bbox[1].x - pos->x) < r && _Abs(poly->bbox[1].z - pos->z) < r) {
        return 0x20;
    }
    if (_line_hit_X(pos->x - r, pos->z - r, lxz, poly)) {
        return 1;
    }
    if (_line_hit_X(pos->x - r, pos->z + r, lxz, poly)) {
        return 2;
    }
    if (_line_hit_Z(pos->x - r, pos->z - r, lxz, poly)) {
        return 4;
    }
    if (_line_hit_Z(pos->x + r, pos->z - r, lxz, poly)) {
        return 8;
    }
    return 0;
}

static int coll_CheckLineBox(VecFx32* posP, fx32 r, COLL_POLY* poly, VecFx32* cross)
{
    fx32 t;
    VecFx32 vec;
    VecFx32 vec1;
    VecFx32 vec2;
    VecFx32 vec3;
    if (poly->bbox[0].y > posP->y || poly->bbox[1].y < posP->y - r) {
        return 0;
    }
    if (!_check_box(posP, r - 1, poly)) {
        return 0;
    }
    vec.x = poly->bbox[1].x - poly->bbox[0].x;
    vec.z = poly->bbox[1].z - poly->bbox[0].z;
    if (vec.x == 0 && vec.z == 0) {
        return 0;
    }
    vec1.x = posP->x - poly->bbox[0].x;
    vec1.z = posP->z - poly->bbox[0].z;
    t = FX_Divide(FX_Mul(vec.x, vec1.x) + FX_Mul(vec.z, vec1.z), FX_Mul(vec.x, vec.x) + FX_Mul(vec.z, vec.z));
    vec2.x = FX_Mul(t, vec.x);
    vec2.z = FX_Mul(t, vec.z);
    cross->x = poly->bbox[0].x + vec2.x;
    cross->z = poly->bbox[0].z + vec2.z;
    cross->y = posP->y;
    if (t <= 0) {
        vec3.x = poly->bbox[0].x - posP->x;
        vec3.z = poly->bbox[0].z - posP->z;
        if (FX_Mul(vec3.x, poly->normal.x) + FX_Mul(vec3.z, poly->normal.z) > 0) {
            return 0;
        }
        cross[1] = poly->bbox[0];
        cross[1].y = 0;
    } else if (t >= 0x1000) {
        vec3.x = poly->bbox[1].x - posP->x;
        vec3.z = poly->bbox[1].z - posP->z;
        if (FX_Mul(vec3.x, poly->normal.x) + FX_Mul(vec3.z, poly->normal.z) > 0) {
            return 0;
        }
        cross[1] = poly->bbox[1];
        cross[1].y = 1;
    } else {
        vec3.x = vec2.x - vec1.x;
        vec3.z = vec2.z - vec1.z;
        if (FX_Mul(vec3.x, poly->normal.x) + FX_Mul(vec3.z, poly->normal.z) > 0) {
            return 0;
        }
        cross[1] = cross[0];
        cross[1].y = 2;
    }
    return 1;
}

int coll_CheckBoxWallNo(COLL_HEADER* header, VecFx32* center, fx32 r, int start, VecFx32* ret)
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
                if (coll_CheckLineBox(center, r, &header->poly[i], ret)) {
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
            if (coll_CheckLineBox(center, r, &header->ext_data->ext_coll[i], ret)) {
                return i + header->poly_size;
            }
        }
    }
    return -1;
}

static int _get_slope_A(VecFx32* v0, VecFx32* n)
{
    VecFx32 v1;
    VecFx32 v2;
    if (v0->z == 0) {
        return n->z < 0 ? 8 : 0;
    }
    if (v0->x == 0) {
        return n->x < 0 ? 4 : 12;
    }
    v2.z = v0->z;
    if (v0->x < 0) {
        v2.z = -v2.z;
    }
    v1.x = _Abs(v0->x);
    v1.z = _Abs(v0->z);
    if (_Abs(v1.x - v1.z) < 8) {
        if (v2.z < 0) {
            return n->x < 0 ? 6 : 14;
        }
        return n->x < 0 ? 2 : 10;
    }
    if (v1.z < v1.x) {
        if (v2.z < 0) {
            return n->z < 0 ? 7 : 15;
        }
        return n->z < 0 ? 9 : 1;
    }
    if (v2.z < 0) {
        return n->x < 0 ? 5 : 13;
    }
    return n->x < 0 ? 3 : 11;
}

static int _get_slope_B(VecFx32* pos, VecFx32* to)
{
    VecFx32 v1;
    VecFx32 v2;
    v1.x = to->x - pos->x;
    v1.z = to->z - pos->z;
    v2.x = _Abs(v1.x);
    v2.z = _Abs(v1.z);
    if (v1.x == 0) {
        return v1.z < 0 ? 0x1 : 0x100;
    }
    if (v1.z == 0) {
        return v1.x < 0 ? 0x1000 : 0x10;
    }
    if (_Abs(v2.x - v2.z) < 8) {
        if (v1.z < 0) {
            return v1.x < 0 ? 0x4000 : 0x4;
        }
        return v1.x < 0 ? 0x400 : 0x40;
    }
    if (v2.z < v2.x) {
        if (v1.z < 0) {
            return v1.x < 0 ? 0x2000 : 0x8;
        }
        return v1.x < 0 ? 0x800 : 0x20;
    }
    if (v1.z < 0) {
        return v1.x < 0 ? 0x8000 : 0x2;
    }
    return v1.x < 0 ? 0x200 : 0x80;
}

static int _check_fix(VecFx32* pos, VecFx32* cross, VecFx32* vec, COLL_POLY* poly)
{
    int id = _get_slope_A(vec, &poly->normal);
    int vd = _get_slope_B(pos, &cross[1]);
    switch (id) {
    case 0:
        return vd & ~0x8003;
    case 8:
        return vd & ~0x380;
    case 4:
        return vd & ~0x38;
    case 12:
        return vd & ~0x3800;
    }
    return vd != (1 << id);
}

static COLL_POLY* get_coll_poly(COLL_HEADER* header, int poly_no)
{
    if (poly_no >= header->poly_size) {
        poly_no -= header->poly_size;
        return &header->ext_data->ext_coll[poly_no];
    }
    return &header->poly[poly_no];
}

static void coll_FixBoxPos(COLL_HEADER* header, VecFx32* pos, fx32 r, fx32 r2, VecFx32* cross, fx32 a)
{
    COLL_POLY* poly = get_coll_poly(header, a);
    VecFx32 normal;
    VecFx32 v1;
    VecFx32 v2;
    VecFx32 v3;
    func_02062f98(pos, cross, &normal);
    if (normal.x == 0 && normal.z == 0) {
        return;
    }
    v1.x = poly->bbox[0].x - poly->bbox[1].x;
    v1.z = poly->bbox[0].z - poly->bbox[1].z;
    if (!_check_fix(pos, cross, &v1, poly)) {
        func_020630ec(&normal, &normal);
        if (v1.x == 0 || v1.z == 0) {
            if (cross[1].y < 2) {
                goto fix;
            }
            func_02063330(r + 8, &normal, cross, pos);
            return;
        }
        v2.x = cross->x < pos->x ? -r : r;
        v2.z = cross->z < pos->z ? -r : r;
        a = FX_Divide(FX_Mul(v2.x, v1.z) - FX_Mul(v1.x, v2.z), FX_Mul(r2, FX_Sqrt(FX_Mul(v1.x, v1.x) + FX_Mul(v1.z, v1.z))));
        r2 = _Abs(FX_Mul(a, r2));
        v2.z = pos->x;
        v1.y = pos->z;
        func_02063330(r2 + 8, &normal, cross, pos);
        v3.x = pos->x + v2.x;
        v2.x = _Abs(v3.x - poly->bbox[0].x);
        v3.z = _Abs(v3.x - poly->bbox[1].x);
        if (v2.x < v3.z) {
            if (v3.z - 8 <= _Abs(v1.x)) {
                return;
            }
            v3.x = poly->bbox[0].x;
            v3.z = poly->bbox[0].z;
        } else {
            if (v2.x - 8 <= _Abs(v1.x)) {
                return;
            }
            v3.x = poly->bbox[1].x;
            v3.z = poly->bbox[1].z;
        }
        if (_Abs(v1.x) < _Abs(v1.z)) {
            v2.z = v3.x + (v2.z < v3.x ? -(r + 8) : r + 8);
        } else {
            v1.y = v3.z + (v1.y < v3.z ? -(r + 8) : r + 8);
        }
        pos->x = v2.z;
        pos->z = v1.y;
        return;
    }
fix:
    if (cross[1].y == 0) {
        v3.x = poly->bbox[0].x;
        v3.z = poly->bbox[0].z;
    } else {
        v3.x = poly->bbox[1].x;
        v3.z = poly->bbox[1].z;
    }
    v2.x = pos->x - v3.x;
    v1.x = _Abs(v2.x);
    v2.z = pos->z - v3.z;
    v1.z = _Abs(v2.z);
    if (v1.x > v1.z) {
        if (v1.x >= r + 8) {
            return;
        }
        if (v1.z >= (r + 8) >> 3 && cross[2].y) {
            if (pos->z < v3.z) {
                pos->z -= cross[2].x >> 3;
            } else {
                pos->z += cross[2].x >> 3;
            }
        }
        pos->x = v3.x + (v2.x < 0 ? -(r + 8) : r + 8);
    } else {
        if (v1.z >= r + 8) {
            return;
        }
        if (v1.x >= (r + 8) >> 3 && cross[2].y) {
            if (pos->x < v3.x) {
                pos->x -= cross[2].z >> 3;
            } else {
                pos->x += cross[2].z >> 3;
            }
        }
        pos->z = v3.z + (v2.z < 0 ? -(r + 8) : r + 8);
    }
}

int coll_GetNextMoveBox(COLL_HEADER* header, VecFx32* old_center, VecFx32* center, fx32 r, VecFx32* ret)
{
    VecFx32 min_cross[3];
    VecFx32 cross[2];
    int hit_poly_no;
    int no;
    fx32 len;
    fx32 r2;
    fx32 min;
    int min_poly_no;
    int counter;
    r2 = FX_Mul(r, 0x16a0);
    min_poly_no = -1;
    hit_poly_no = -1;
    counter = 0;
    if (header == NULL || old_center == NULL || center == NULL || ret == NULL) {
        return -1;
    }
    {
        VecFx32* vp = &min_cross[2];
        vp->x = _Abs(center->x - old_center->x);
        vp->z = _Abs(center->z - old_center->z);
        if (vp->x < vp->z) {
            vp->y = vp->x <= vp->z >> 3;
        } else {
            vp->y = vp->z <= vp->x >> 3;
        }
    }
    *ret = *center;
    while (1) {
        if (counter++ >= 2) {
            return min_poly_no;
        }
        min = 0x7fffffff;
        no = -(counter > 1);
        while (1) {
            no = coll_CheckBoxWallNo(header, ret, r, no, cross);
            if (no == -1) {
                if (min == 0x7fffffff) {
                    return min_poly_no;
                }
                if (hit_poly_no == min_poly_no) {
                    return min_poly_no;
                }
                coll_FixBoxPos(header, ret, r, r2, min_cross, min_poly_no);
                hit_poly_no = min_poly_no;
                break;
            }
            len = FX_Mul(cross[1].x - ret->x, cross[1].x - ret->x) + FX_Mul(cross[1].z - ret->z, cross[1].z - ret->z);
            if (min > len) {
                min = len;
                min_poly_no = no;
                min_cross[0] = cross[0];
                min_cross[1] = cross[1];
            }
            no++;
        }
    }
}

int coll_GetObjWallNo(COLL_HEADER* header, int obj_id, int poly_no)
{
    int n;
    for (n = poly_no; n >= 0; n--) {
        if (header->poly[n].obj_id != obj_id) {
            return poly_no - (n + 1);
        }
    }
    return -1;
}
