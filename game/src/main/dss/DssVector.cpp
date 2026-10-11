#include "main/dss/DssUtils.hpp"

ARM dss::Fix32Vector3::Fix32Vector3() : vx(0L), vy(0L), vz(0L)
{
}

ARM dss::Fix32Vector3::Fix32Vector3(const int x, const int y, const int z)
{
    vx = (long)x;
    vy = (long)y;
    vz = (long)z;
}

ARM dss::Fix32Vector3::Fix32Vector3(float x, float y, float z)
{
    vx = x;
    vy = y;
    vz = z;
}

ARM void dss::Fix32Vector3::set(const Fix32& x, const Fix32& y, const Fix32& z)
{
    vx = x;
    vy = y;
    vz = z;
}

ARM void dss::Fix32Vector3::set(fx32 x, fx32 y, fx32 z)
{
    vx.value = x;
    vy.value = y;
    vz.value = z;
}

ARM void dss::Fix32Vector3::set(float x, float y, float z)
{
    vx = x;
    vy = y;
    vz = z;
}

ARM void dss::Fix32Vector3::setFix32(int x, int y, int z)
{
    vx = (long)x;
    vy = (long)y;
    vz = (long)z;
}

ARM void dss::Fix32Vector3::operator=(const Fix32Vector3& o)
{
    vx = o.vx;
    vy = o.vy;
    vz = o.vz;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator+(const Fix32Vector3& o) const
{
    Fix32Vector3 ret;
    ret.vx = vx + o.vx;
    ret.vy = vy + o.vy;
    ret.vz = vz + o.vz;
    return ret;
}

ARM void dss::Fix32Vector3::operator+=(const Fix32Vector3& o)
{
    vx += o.vx;
    vy += o.vy;
    vz += o.vz;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator-(const Fix32Vector3& o) const
{
    Fix32Vector3 ret;
    ret.vx = vx - o.vx;
    ret.vy = vy - o.vy;
    ret.vz = vz - o.vz;
    return ret;
}

ARM void dss::Fix32Vector3::operator-=(const Fix32Vector3& o)
{
    vx -= o.vx;
    vy -= o.vy;
    vz -= o.vz;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator*(const Fix32& s) const
{
    Fix32Vector3 ret;
    ret.vx = vx * s;
    ret.vy = vy * s;
    ret.vz = vz * s;
    return ret;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator*(int s) const
{
    Fix32Vector3 ret;
    ret.vx = vx * s;
    ret.vy = vy * s;
    ret.vz = vz * s;
    return ret;
}

ARM void dss::Fix32Vector3::operator*=(const Fix32& s)
{
    vx *= s;
    vy *= s;
    vz *= s;
}

ARM void dss::Fix32Vector3::operator*=(int s)
{
    vx *= s;
    vy *= s;
    vz *= s;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator/(const Fix32& s) const
{
    Fix32Vector3 ret;
    ret.vx = vx / s;
    ret.vy = vy / s;
    ret.vz = vz / s;
    return ret;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator/(int s) const
{
    Fix32Vector3 ret;
    ret.vx = vx / s;
    ret.vy = vy / s;
    ret.vz = vz / s;
    return ret;
}

ARM void dss::Fix32Vector3::operator/=(const Fix32& s)
{
    vx /= s;
    vy /= s;
    vz /= s;
}

ARM void dss::Fix32Vector3::operator/=(int s)
{
    vx /= s;
    vy /= s;
    vz /= s;
}

ARM bool dss::Fix32Vector3::operator==(const Fix32Vector3& o) const
{
    if (vx == o.vx && vy == o.vy && vz == o.vz) {
        return true;
    }
    return false;
}

ARM bool dss::Fix32Vector3::operator!=(const Fix32Vector3& o) const
{
    if (vx == o.vx && vy == o.vy && vz == o.vz) {
        return false;
    }
    return true;
}

ARM dss::Fix32 dss::Fix32Vector3::operator*(const Fix32Vector3& o) const
{
    Fix32 ret;
    ret = vx * o.vx + vy * o.vy + vz * o.vz;
    return ret;
}

ARM dss::Fix32Vector3 dss::Fix32Vector3::operator%(const Fix32Vector3& o) const
{
    Fix32Vector3 ret;
    ret.vx = vy * o.vz - vz * o.vy;
    ret.vy = vz * o.vx - vx * o.vz;
    ret.vz = vx * o.vy - vy * o.vx;
    return ret;
}

ARM dss::Fix32 dss::Fix32Vector3::length() const
{
    Fix32 ret;
    ret = vx * vx + vy * vy + vz * vz;
    ret = ret.sqrt();
    return ret;
}

ARM dss::Fix32 dss::Fix32Vector3::lengthsq() const
{
    Fix32 ret;
    ret = vx * vx + vy * vy + vz * vz;
    return ret;
}

ARM dss::Fix32 dss::Fix32Vector3::length(const Fix32Vector3& o) const
{
    Fix32 ret;
    ret = (vx - o.vx) * (vx - o.vx) + (vy - o.vy) * (vy - o.vy) + (vz - o.vz) * (vz - o.vz);
    ret = ret.sqrt();
    return ret;
}

ARM dss::Fix32 dss::Fix32Vector3::lengthsq(const Fix32Vector3& o) const
{
    Fix32 ret;
    ret = (vx - o.vx) * (vx - o.vx) + (vy - o.vy) * (vy - o.vy) + (vz - o.vz) * (vz - o.vz);
    return ret;
}

ARM void dss::Fix32Vector3::normalize()
{
    Fix32 len;
    len = vx * vx + vy * vy + vz * vz;
    len = len.sqrt();
    if (len.value != 0) {
        *this /= len;
    }
}
