#include <nitro/native_video.h>
#include "main/dss/Fog.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssMath.hpp"

int g_fogEnabled;
dss::Fix32 data_0211d314[2];
int g_fogOffsets[2];
unsigned int g_fogTables[2][8];

ARM void initFog()
{
    g_fogEnabled = 0;
    func_02065350(0, 0, 10, 1000);
    for (signed char i = 0; i < 8; i++) {
        g_fogTables[0][i] = packRgba((unsigned char)(i * 16 + 12), (unsigned char)(i * 16 + 8), (unsigned char)(i * 16 + 4), (unsigned char)(i * 16));
    }
    func_02065408(g_fogTables[0]);
    *VIDEO_REG(volatile unsigned int, 0x04000358) = 0x1f7fff;
}

ARM void setFogOffset(int side, int offset)
{
    g_fogOffsets[side] = offset;
}

ARM void setFogRate(int side, dss::Fix32 rate)
{
    for (int i = 0; i < 8; i++) {
        int a = rate.value * (i * 16) >> 12;
        a = dss::clamp<int>(a, 0, 0x7f);
        int b = dss::clamp<int>(a, 0, 0x7f);
        int g = dss::clamp<int>(a, 0, 0x7f);
        int r = dss::clamp<int>(a, 0, 0x7f);
        if (i == 7) {
            r = 0x7f;
        }
        g_fogTables[side][i] = packRgba((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    }
}

ARM void setFogColor(int r, int g, int b)
{
    *VIDEO_REG(volatile unsigned int, 0x04000358) = (unsigned short)(r | (g << 5) | (b << 10)) | 0x1f0000;
}

ARM void setFogEnable(int enable)
{
    g_fogEnabled = enable;
    func_02065350(0, 0, 10, 1000);
}

ARM void updateFog()
{
    if (g_fogEnabled == 0) {
        return;
    }
    if (getDisplayFrame() & 1) {
        func_02065350(1, 0, 10, g_fogOffsets[0]);
        func_02065408(g_fogTables[0]);
    } else {
        func_02065350(1, 0, 10, g_fogOffsets[1]);
        func_02065408(g_fogTables[1]);
    }
}
