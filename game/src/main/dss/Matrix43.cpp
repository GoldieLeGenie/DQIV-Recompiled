#include "main/dss/Matrix43.hpp"
#include <nitro/fx/fx_trig.h>

ARM dss::Matrix43::Matrix43()
{
    func_02061f58(this);
}

ARM void dss::Matrix43::unkfunc_0208860c()
{
    func_02061f58(this);
}

ARM dss::Matrix43& dss::Matrix43::operator=(const Matrix43& o)
{
    *(MtxFx43*)this = o;
    return *this;
}

ARM dss::Matrix43 dss::Matrix43::operator*(const Matrix43& o) const
{
    Matrix43 m;
    func_020623cc(this, &o, &m);
    return m;
}

ARM dss::Fix32Vector3 dss::Matrix43::operator*(const Fix32Vector3& v) const
{
    Fix32Vector3 r;
    func_020626a0((const VecFx32*)&v, this, (VecFx32*)&r);
    return r;
}

ARM void dss::Matrix43::unkfunc_02088698(short angle)
{
    func_02061fe8(this, FX_SinIdx((unsigned short)angle), FX_CosIdx((unsigned short)angle));
}

ARM void dss::Matrix43::unkfunc_020886d0(short angle)
{
    func_02062008(this, FX_SinIdx((unsigned short)angle), FX_CosIdx((unsigned short)angle));
}

ARM void dss::Matrix43::unkfunc_02088708(short angle)
{
    func_02062024(this, FX_SinIdx((unsigned short)angle), FX_CosIdx((unsigned short)angle));
}
