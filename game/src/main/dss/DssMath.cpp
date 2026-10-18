#include "main/dss/DssMath.hpp"

const dss::Fix32 g_fix32Pi(0x3244);
const dss::Fix32 g_fix32HalfPi(0x1922);
const dss::Fix32 g_fix32Deg360(0x168000);
const dss::Fix32 g_fix32Deg180(0xb4000);
const dss::Fix32 g_fix32Deg90(0x5a000);
const dss::Fix32 g_fix32Sixteen(0x10000);
const dss::Fix32 g_fix32Eight(0x8000);
static const int s_unused[2] = {0};      // not original: unreferenced (stripped) item, fixes the .data order

ARM int packRgba(int r, int g, int b, int a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

ARM int degreeToAngle(dss::Fix32 degree)
{
    return (unsigned short)(degree * g_fix32Eight / g_fix32Deg180).value;
}
