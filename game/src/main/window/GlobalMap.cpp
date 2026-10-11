#pragma ipa file
#include "main/window/GlobalMap.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/StageStatus.hpp"
#include "nitro/os.hpp"

ARM window::GlobalMap::GlobalMap()
{
}

ARM window::GlobalMap::~GlobalMap()
{
}

ARM void window::GlobalMap::setup(Render* render)
{
    render_ = render;
    unk_f8 = 0;
    setup();
}

ARM void window::GlobalMap::cleanup()
{
    unk_100.unkfunc_02057e34();
    unk_08.unkfunc_02057e34();
    unk_a8.unkfunc_02057e34();
    render_ = NULL;
}

ARM void window::GlobalMap::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01.tex");
    unk_08.unkfunc_02057d60(path, 0);
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f18(0x3e);
    unk_08.unkfunc_02057f30(0);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01m.tex");
    unk_100.unkfunc_02057d60(path, 0);
    unk_100.unkfunc_02057ee8();
    unk_100.unkfunc_02057f18(0x3e);
    unk_100.unkfunc_02057f30(8);
    unk_08.unkfunc_02057f00(0);
    unk_100.unkfunc_02057f00(0);
    unk_08.unkfunc_02057d1c();
    unk_100.unkfunc_02057d1c();
    dss::sprintf_s(path, sizeof(path), "data/field/2d/wmap_x.tex");
    unk_a8.unkfunc_02057d60(path, 0);
    unk_a8.unkfunc_02057ee8();
    unk_a8.unkfunc_02057f18(0x3e);
    unk_a8.unkfunc_02057f30(9);
    unk_a8.unkfunc_02057f00(0);
    unk_a8.unkfunc_02057ed4(1);
    unk_a8.unkfunc_02057ea8(0, 0, 0x10, 0x10);
    unk_58.unkfunc_02057ee8();
    unk_58.unkfunc_02057f18(0x3e);
    unk_58.unkfunc_02057f30(4);
    unk_58.unkfunc_02057f00(0);
    unk_58.unkfunc_02057e98(3, 3);
}

ARM void window::GlobalMap::draw()
{
    unkfunc_020847e8();
    if (g_AreaFlag.check(0x140)) {
        unk_a8.unkfunc_02057ec0();
    }
    drawVeilMap();
    unkfunc_0203822c(0);
}

ARM void window::GlobalMap::drawVeilMap()
{
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057ea8(0, 0, 0x30, 0xc0);
    unk_08.unkfunc_02057e88(0, 0);
    unk_08.unkfunc_02057e98(0x30, 0xc0);
    unk_08.unkfunc_02057ec0();
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057ea8(0xd0, 0, 0x100, 0xc0);
    unk_08.unkfunc_02057e88(0xd0, 0);
    unk_08.unkfunc_02057e98(0x30, 0xc0);
    unk_08.unkfunc_02057ec0();
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057ea8(0x30, 0, 0xd0, 0x10);
    unk_08.unkfunc_02057e88(0x30, 0);
    unk_08.unkfunc_02057e98(0xa0, 0x10);
    unk_08.unkfunc_02057ec0();
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057ea8(0x30, 0xb0, 0xd0, 0xc0);
    unk_08.unkfunc_02057e88(0x30, 0xb0);
    unk_08.unkfunc_02057e98(0xa0, 0x10);
    unk_08.unkfunc_02057ec0();
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            veilDraw(x, y, g_Stage.isMapVeil(x, y));
        }
    }
}

ARM void window::GlobalMap::veilDraw(int x, int y, int flag)
{
    if (flag) {
        unk_100.unkfunc_02057ee8();
        unk_100.unkfunc_02057f30(8);
        unk_100.unkfunc_02057ea8(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
        unk_100.unkfunc_02057e88(x * 10 + 0x30, y * 10 + 0x10);
        unk_100.unkfunc_02057e98(10, 10);
        unk_100.unkfunc_02057ec0();
    }
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057ea8(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
    unk_08.unkfunc_02057e88(x * 10 + 0x30, y * 10 + 0x10);
    unk_08.unkfunc_02057e98(10, 10);
    unk_08.unkfunc_02057ec0();
}

ARM void window::GlobalMap::load()
{
    OS_Wait();
    unk_08.unkfunc_02057d40();
    unk_08.unkfunc_02057d2c();
    unk_100.unkfunc_02057d40();
    unk_100.unkfunc_02057d2c();
}

ARM void window::GlobalMap::clear()
{
    unk_08.unkfunc_02057d1c();
    unk_100.unkfunc_02057d1c();
}

ARM dss::Vector2<int> window::GlobalMap::convertMapPos(int x, int y)
{
    dss::Vector2<int> pos;
    pos.vx = x / 0x19999 + 0x30;
    pos.vy = y / 0x19999 + 0x10;
    return pos;
}

ARM void window::GlobalMap::setAlpha(unsigned char alpha)
{
    unk_08.unkfunc_02057f00(alpha);
    unk_100.unkfunc_02057f00(alpha);
    unk_58.unkfunc_02057f00(alpha);
    unk_a8.unkfunc_02057f00(alpha);
}

ARM void window::GlobalMap::playerMapPosition()
{
}
