#include "main/dss/UnkDssMath.hpp"

const dss::Fix32 data_020c40f0(0x3244);
const dss::Fix32 data_020c40ec(0x1922);
const dss::Fix32 data_020c40e8(0x168000);
const dss::Fix32 data_020c40e4(0xb4000);
const dss::Fix32 data_020c40e0(0x5a000);
const dss::Fix32 data_020c40dc(0x10000);
const dss::Fix32 data_020c40d8(0x8000);
static const int s_unused[2] = {0};      // not original: unreferenced (stripped) item, fixes the .data order

ARM int unkfunc_02080d80(int r, int g, int b, int a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

ARM int unkfunc_02080d94(dss::Fix32 degree)
{
    return (unsigned short)(degree * data_020c40d8 / data_020c40e4).value;
}
