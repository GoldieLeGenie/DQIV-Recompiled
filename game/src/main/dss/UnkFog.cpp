#include <nitro/native_video.h>
#include "main/dss/UnkFog.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/UnkDssMath.hpp"

int data_0211d310;
dss::Fix32 data_0211d314[2];
int data_0211d31c[2];
unsigned int data_0211d324[2][8];

ARM void unkfunc_0208313c()
{
    data_0211d310 = 0;
    func_02065350(0, 0, 10, 1000);
    for (signed char i = 0; i < 8; i++) {
        data_0211d324[0][i] = unkfunc_02080d80((unsigned char)(i * 16 + 12), (unsigned char)(i * 16 + 8), (unsigned char)(i * 16 + 4), (unsigned char)(i * 16));
    }
    func_02065408(data_0211d324[0]);
    *VIDEO_REG(volatile unsigned int, 0x04000358) = 0x1f7fff;
}

ARM void unkfunc_020831d8(int side, int offset)
{
    data_0211d31c[side] = offset;
}

ARM void unkfunc_020831e8(int side, dss::Fix32 rate)
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
        data_0211d324[side][i] = unkfunc_02080d80((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);
    }
}

ARM void unkfunc_0208328c(int r, int g, int b)
{
    *VIDEO_REG(volatile unsigned int, 0x04000358) = (unsigned short)(r | (g << 5) | (b << 10)) | 0x1f0000;
}

ARM void unkfunc_020832b0(int enable)
{
    data_0211d310 = enable;
    func_02065350(0, 0, 10, 1000);
}

ARM void unkfunc_020832d8()
{
    if (data_0211d310 == 0) {
        return;
    }
    if (unkfunc_02081254() & 1) {
        func_02065350(1, 0, 10, data_0211d31c[0]);
        func_02065408(data_0211d324[0]);
    } else {
        func_02065350(1, 0, 10, data_0211d31c[1]);
        func_02065408(data_0211d324[1]);
    }
}
