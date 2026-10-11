#include "main/cmn/CommonCalculate.hpp"
#include "main/dss/UnkMatrix43.hpp"

// unreferenced
const short data_020b5fa8 = 0x1000;

inline const long& enterLimitL() { return 0L; }
inline const long& frontLimitL() { return 0L; }

ARM void cmn::CommonCalculate::getDirByIdx(short dir, dss::Fix32Vector3& vec)
{
    vec.set(0, 0, 0x1000);
    dss::UnkMatrix43 rot;
    rot.unkfunc_020886d0(dir);
    vec = rot * vec;
}

ARM bool cmn::CommonCalculate::simpleAreaInCheck(dss::Fix32Vector3& min, dss::Fix32Vector3& max, dss::Fix32Vector3 pos)
{
    if (min.vx <= pos.vx && max.vx >= pos.vx &&
        min.vy <= pos.vy && max.vy >= pos.vy &&
        min.vz <= pos.vz && max.vz >= pos.vz) {
        return true;
    }
    return false;
}

ARM bool cmn::CommonCalculate::areaCheck(dss::Fix32Vector3& pos, short dir, dss::Fix32Vector3& min, dss::Fix32Vector3& max, int check, int type)
{
    if (min.vx < pos.vx && max.vx > pos.vx &&
        min.vy < pos.vy && max.vy > pos.vy &&
        min.vz < pos.vz && max.vz > pos.vz) {
        if (type == 7) {
            return false;
        }
        if (check == 0) {
            return true;
        }
    } else {
        if (type == 6) {
            return false;
        }
        if (check == 0) {
            return true;
        }
    }
    dss::Fix32Vector3 diff = (min + max) / 2 - pos;
    dss::Fix32Vector3 vec;
    switch (check) {
    case 1:
        getDirByIdx(dir, vec);
        break;
    case 2:
        vec.set(0, 0, 0x1000);
        break;
    case 3:
        vec.set(0x1000, 0, 0);
        break;
    case 4:
        vec.set(0, 0, -0x1000);
        break;
    case 5:
        vec.set(-0x1000, 0, 0);
        break;
    }
    dss::Fix32 dot = vec * diff;
    if (check == 1) {
        if ((dot > dss::Fix32(enterLimitL()) && type == 6) || (dot < dss::Fix32(0L) && type == 7)) {
            return true;
        }
    } else {
        if (dot > dss::Fix32(frontLimitL())) {
            return true;
        }
    }
    return false;
}

ARM bool cmn::CommonCalculate::directionCheckByScriptParam(int param, short dir)
{
    static const short checkDir[8] = { 0, 0x2000, 0x4000, 0x6000, (short)0x8000, (short)0xa000, (short)0xc000, (short)0xe000 };
    if (param != 4) {
        short center = checkDir[param];
        int result = 0;
        if (center - 0x1000 <= dir && dir < center + 0x1000) {
            result = 1;
        }
        return result;
    }
    if (dir >= 0x7000 || dir < -0x7000) {
        return true;
    }
    return false;
}

ARM int cmn::CommonCalculate::getFrameByVector(dss::Fix32Vector3& from, dss::Fix32Vector3& to, dss::Fix32 speed)
{
    if (speed == dss::Fix32(0L)) {
        return 0;
    }
    return dss::max<int>((((to - from) / speed)).length().value / 0x1000, 1);
}

ARM dss::Fix32Vector3 cmn::CommonCalculate::setVecByParam(int x, int y, int z)
{
    dss::Fix32Vector3 vec;
    vec.vx.value = x;
    vec.vy.value = y;
    vec.vz.value = z;
    return vec;
}

ARM short cmn::CommonCalculate::getIdxByParam(unsigned char param)
{
    switch (param) {
    case 0:
        return 0;
    case 1:
        return 0x4000;
    case 2:
        return -0x8000;
    case 3:
        return -0x4000;
    }
    return 0;
}

ARM dss::Fix32Vector3 cmn::CommonCalculate::getAxisMoveTargetByParam(unsigned int axis, unsigned int mode, int value, dss::Fix32Vector3& pos)
{
    dss::Fix32Vector3 target = pos;
    dss::Fix32 move;
    move.value = value;
    if (mode == 1) {
        switch (axis) {
        case 0:
            target.vx += move;
            break;
        case 1:
            target.vy += move;
            break;
        case 2:
            target.vz += move;
            break;
        }
    } else if (mode == 0) {
        switch (axis) {
        case 0:
            target.vx = move;
            break;
        case 1:
            target.vy = move;
            break;
        case 2:
            target.vz = move;
            break;
        }
    }
    return target;
}
