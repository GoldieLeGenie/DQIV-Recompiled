#include "main/dss/DssUtils.hpp"

ARM dss::Fix32::Fix32()
{
    value = 0;
}

ARM dss::Fix32::Fix32(fx32 v)
{
    value = v;
}

ARM dss::Fix32::Fix32(const long& v)
{
    value = v << FX32_SHIFT;
}

ARM dss::Fix32::Fix32(const float& v)
{
    value = (fx32)(v * 4096.0f);
}

ARM dss::Fix32::Fix32(const Fix32& other)
{
    value = other.value;
}

ARM dss::Fix32& dss::Fix32::operator=(long v)
{
    value = v << FX32_SHIFT;
    return *this;
}

ARM dss::Fix32& dss::Fix32::operator=(fx32 v)
{
    value = v;
    return *this;
}

ARM dss::Fix32& dss::Fix32::operator=(float v)
{
    value = (fx32)(v * 4096.0f);
    return *this;
}

ARM dss::Fix32& dss::Fix32::operator=(const Fix32& other)
{
    value = other.value;
    return *this;
}

ARM dss::Fix32 dss::Fix32::operator+(int v) const
{
    Fix32 ret;
    ret.value = value + v;
    return ret;
}

ARM dss::Fix32 dss::Fix32::operator+(const Fix32& o) const
{
    Fix32 ret;
    ret.value = value + o.value;
    return ret;
}

ARM void dss::Fix32::operator+=(int v)
{
    value += v;
}

ARM void dss::Fix32::operator+=(const Fix32& o)
{
    value += o.value;
}

ARM dss::Fix32 dss::Fix32::operator-(int v) const
{
    Fix32 ret;
    ret.value = value - v;
    return ret;
}

ARM dss::Fix32 dss::Fix32::operator-(const Fix32& o) const
{
    Fix32 ret;
    ret.value = value - o.value;
    return ret;
}

ARM void dss::Fix32::operator-=(const Fix32& o)
{
    value -= o.value;
}

ARM dss::Fix32 dss::Fix32::operator*(int v) const
{
    Fix32 ret;
    ret.value = (fx32)(((long long)value * (v << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
    return ret;
}

ARM dss::Fix32 dss::Fix32::operator*(const Fix32& o) const
{
    Fix32 ret;
    ret.value = (fx32)(((long long)value * o.value + 0x800) >> FX32_SHIFT);
    return ret;
}

ARM void dss::Fix32::operator*=(int v)
{
    value = (fx32)(((long long)value * (v << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
}

ARM void dss::Fix32::operator*=(const Fix32& o)
{
    value = (fx32)(((long long)value * o.value + 0x800) >> FX32_SHIFT);
}

ARM dss::Fix32 dss::Fix32::operator/(int v) const
{
    Fix32 ret;
    ret.value = FX_Divide(value, v << FX32_SHIFT);
    return ret;
}

ARM dss::Fix32 dss::Fix32::operator/(const Fix32& o) const
{
    Fix32 ret;
    ret.value = FX_Divide(value, o.value);
    return ret;
}

ARM void dss::Fix32::operator/=(int v)
{
    value = FX_Divide(value, v << FX32_SHIFT);
}

ARM void dss::Fix32::operator/=(const Fix32& o)
{
    value = FX_Divide(value, o.value);
}

ARM bool dss::Fix32::operator==(const Fix32& o) const
{
    return value == o.value;
}

ARM bool dss::Fix32::operator!=(const Fix32& o) const
{
    return !(*this == o);
}

ARM bool dss::Fix32::operator>(const Fix32& o) const
{
    return value > o.value;
}

ARM bool dss::Fix32::operator>=(const Fix32& o) const
{
    return value >= o.value;
}

ARM bool dss::Fix32::operator<(const Fix32& o) const
{
    return value < o.value;
}

ARM bool dss::Fix32::operator<=(const Fix32& o) const
{
    return value <= o.value;
}

ARM dss::Fix32 dss::Fix32::sqrt()
{
    Fix32 ret;
    ret.value = FX_Sqrt(value);
    return ret;
}

ARM float dss::Fix32::getfloat()
{
    return (float)value / 4096.0f;
}

ARM dss::Fix16::Fix16(const long& v)
{
    value = static_cast<fx16>(v << FX32_SHIFT);
}

ARM dss::Fix16::Fix16(const Fix16& other)
{
    value = other.value;
}

ARM dss::Fix16& dss::Fix16::operator=(long v)
{
    value = static_cast<fx16>(v << FX32_SHIFT);
    return *this;
}

ARM dss::Fix16& dss::Fix16::operator=(float v)
{
    value = (fx16)(v * 4096.0f);
    return *this;
}

ARM dss::Fix16& dss::Fix16::operator=(const Fix16& other)
{
    value = other.value;
    return *this;
}

ARM dss::Fix16& dss::Fix16::operator=(const Fix32& other)
{
    value = static_cast<fx16>(other.value);
    return *this;
}

ARM dss::Fix16 dss::Fix16::operator*(int v)
{
    Fix16 ret;
    ret.value = (fx16)(((long long)value * (v << FX32_SHIFT) + 0x800) >> FX32_SHIFT);
    return ret;
}

ARM dss::Fix16 dss::Fix16::operator/(int v)
{
    Fix16 ret;
    ret.value = static_cast<fx16>(FX_Divide(value, v << FX32_SHIFT));
    return ret;
}
