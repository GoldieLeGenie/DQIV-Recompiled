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
    unk_100.unload();
    unk_08.unload();
    unk_a8.unload();
    render_ = NULL;
}

ARM void window::GlobalMap::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01.tex");
    unk_08.loadFile(path, 0);
    unk_08.setSubScreen();
    unk_08.setPolygonID(0x3e);
    unk_08.setDepth(0);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01m.tex");
    unk_100.loadFile(path, 0);
    unk_100.setSubScreen();
    unk_100.setPolygonID(0x3e);
    unk_100.setDepth(8);
    unk_08.setAlpha(0);
    unk_100.setAlpha(0);
    unk_08.freeVram();
    unk_100.freeVram();
    dss::sprintf_s(path, sizeof(path), "data/field/2d/wmap_x.tex");
    unk_a8.loadFile(path, 0);
    unk_a8.setSubScreen();
    unk_a8.setPolygonID(0x3e);
    unk_a8.setDepth(9);
    unk_a8.setAlpha(0);
    unk_a8.setEnabled(1);
    unk_a8.setTexCoords(0, 0, 0x10, 0x10);
    unk_58.setSubScreen();
    unk_58.setPolygonID(0x3e);
    unk_58.setDepth(4);
    unk_58.setAlpha(0);
    unk_58.setSize(3, 3);
}

ARM void window::GlobalMap::draw()
{
    setupOrtho2D();
    if (g_AreaFlag.check(0x140)) {
        unk_a8.draw();
    }
    drawVeilMap();
    drawSymbols(0);
}

ARM void window::GlobalMap::drawVeilMap()
{
    unk_08.setSubScreen();
    unk_08.setDepth(0);
    unk_08.setTexCoords(0, 0, 0x30, 0xc0);
    unk_08.setPosition(0, 0);
    unk_08.setSize(0x30, 0xc0);
    unk_08.draw();
    unk_08.setSubScreen();
    unk_08.setDepth(0);
    unk_08.setTexCoords(0xd0, 0, 0x100, 0xc0);
    unk_08.setPosition(0xd0, 0);
    unk_08.setSize(0x30, 0xc0);
    unk_08.draw();
    unk_08.setSubScreen();
    unk_08.setDepth(0);
    unk_08.setTexCoords(0x30, 0, 0xd0, 0x10);
    unk_08.setPosition(0x30, 0);
    unk_08.setSize(0xa0, 0x10);
    unk_08.draw();
    unk_08.setSubScreen();
    unk_08.setDepth(0);
    unk_08.setTexCoords(0x30, 0xb0, 0xd0, 0xc0);
    unk_08.setPosition(0x30, 0xb0);
    unk_08.setSize(0xa0, 0x10);
    unk_08.draw();
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            veilDraw(x, y, g_Stage.isMapVeil(x, y));
        }
    }
}

ARM void window::GlobalMap::veilDraw(int x, int y, int flag)
{
    if (flag) {
        unk_100.setSubScreen();
        unk_100.setDepth(8);
        unk_100.setTexCoords(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
        unk_100.setPosition(x * 10 + 0x30, y * 10 + 0x10);
        unk_100.setSize(10, 10);
        unk_100.draw();
    }
    unk_08.setSubScreen();
    unk_08.setDepth(0);
    unk_08.setTexCoords(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
    unk_08.setPosition(x * 10 + 0x30, y * 10 + 0x10);
    unk_08.setSize(10, 10);
    unk_08.draw();
}

ARM void window::GlobalMap::load()
{
    OS_Wait();
    unk_08.allocVram();
    unk_08.transferToVram();
    unk_100.allocVram();
    unk_100.transferToVram();
}

ARM void window::GlobalMap::clear()
{
    unk_08.freeVram();
    unk_100.freeVram();
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
    unk_08.setAlpha(alpha);
    unk_100.setAlpha(alpha);
    unk_58.setAlpha(alpha);
    unk_a8.setAlpha(alpha);
}

ARM void window::GlobalMap::playerMapPosition()
{
}
