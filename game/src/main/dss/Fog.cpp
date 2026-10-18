#include <nitro/native_video.h>
#include "main/dss/Fog.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssMath.hpp"

int data_0211d310;
dss::Fix32 data_0211d314[2];
int data_0211d31c[2];
unsigned int data_0211d324[2][8];

ARM void initFog()
{
    data_0211d310 = 0;
    func_02065350(0, 0, 10, 1000);
    for (signed char i = 0; i < 8; i++) {
        data_0211d324[0][i] = packRgba((unsigned char)(i * 16 + 12), (unsigned char)(i * 16 + 8), (unsigned char)(i * 16 + 4), (unsigned char)(i * 16));
    }
    func_02065408(data_0211d324[0]);
    *VIDEO_REG(volatile unsigned int, 0x04000358) = 0x1f7fff;
}

ARM void setFogOffset(int side, int offset)
{
    data_0211d31c[side] = offset;
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
        data_0211d324[side][i] = packRgba((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    }
}

ARM void setFogColor(int r, int g, int b)
{
    *VIDEO_REG(volatile unsigned int, 0x04000358) = (unsigned short)(r | (g << 5) | (b << 10)) | 0x1f0000;
}

ARM void setFogEnable(int enable)
{
    data_0211d310 = enable;
    func_02065350(0, 0, 10, 1000);
}

ARM void updateFog()
{
    if (data_0211d310 == 0) {
        return;
    }
    if (getDisplayFrame() & 1) {
        func_02065350(1, 0, 10, data_0211d31c[0]);
        func_02065408(data_0211d324[0]);
    } else {
        func_02065350(1, 0, 10, data_0211d31c[1]);
        func_02065408(data_0211d324[1]);
    }
}
