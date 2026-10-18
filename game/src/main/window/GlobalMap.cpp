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
    blinkCounter_ = 0;
    setup();
}

ARM void window::GlobalMap::cleanup()
{
    veilSprite_.unload();
    mapSprite_.unload();
    markSprite_.unload();
    render_ = NULL;
}

ARM void window::GlobalMap::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01.tex");
    mapSprite_.loadFile(path, 0);
    mapSprite_.setSubScreen();
    mapSprite_.setPolygonID(0x3e);
    mapSprite_.setDepth(0);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/dsdq4wmap01m.tex");
    veilSprite_.loadFile(path, 0);
    veilSprite_.setSubScreen();
    veilSprite_.setPolygonID(0x3e);
    veilSprite_.setDepth(8);
    mapSprite_.setAlpha(0);
    veilSprite_.setAlpha(0);
    mapSprite_.freeVram();
    veilSprite_.freeVram();
    dss::sprintf_s(path, sizeof(path), "data/field/2d/wmap_x.tex");
    markSprite_.loadFile(path, 0);
    markSprite_.setSubScreen();
    markSprite_.setPolygonID(0x3e);
    markSprite_.setDepth(9);
    markSprite_.setAlpha(0);
    markSprite_.setEnabled(1);
    markSprite_.setTexCoords(0, 0, 0x10, 0x10);
    symbolSprite_.setSubScreen();
    symbolSprite_.setPolygonID(0x3e);
    symbolSprite_.setDepth(4);
    symbolSprite_.setAlpha(0);
    symbolSprite_.setSize(3, 3);
}

ARM void window::GlobalMap::draw()
{
    setupOrtho2D();
    if (g_AreaFlag.check(0x140)) {
        markSprite_.draw();
    }
    drawVeilMap();
    drawSymbols(0);
}

ARM void window::GlobalMap::drawVeilMap()
{
    mapSprite_.setSubScreen();
    mapSprite_.setDepth(0);
    mapSprite_.setTexCoords(0, 0, 0x30, 0xc0);
    mapSprite_.setPosition(0, 0);
    mapSprite_.setSize(0x30, 0xc0);
    mapSprite_.draw();
    mapSprite_.setSubScreen();
    mapSprite_.setDepth(0);
    mapSprite_.setTexCoords(0xd0, 0, 0x100, 0xc0);
    mapSprite_.setPosition(0xd0, 0);
    mapSprite_.setSize(0x30, 0xc0);
    mapSprite_.draw();
    mapSprite_.setSubScreen();
    mapSprite_.setDepth(0);
    mapSprite_.setTexCoords(0x30, 0, 0xd0, 0x10);
    mapSprite_.setPosition(0x30, 0);
    mapSprite_.setSize(0xa0, 0x10);
    mapSprite_.draw();
    mapSprite_.setSubScreen();
    mapSprite_.setDepth(0);
    mapSprite_.setTexCoords(0x30, 0xb0, 0xd0, 0xc0);
    mapSprite_.setPosition(0x30, 0xb0);
    mapSprite_.setSize(0xa0, 0x10);
    mapSprite_.draw();
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            veilDraw(x, y, g_Stage.isMapVeil(x, y));
        }
    }
}

ARM void window::GlobalMap::veilDraw(int x, int y, int flag)
{
    if (flag) {
        veilSprite_.setSubScreen();
        veilSprite_.setDepth(8);
        veilSprite_.setTexCoords(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
        veilSprite_.setPosition(x * 10 + 0x30, y * 10 + 0x10);
        veilSprite_.setSize(10, 10);
        veilSprite_.draw();
    }
    mapSprite_.setSubScreen();
    mapSprite_.setDepth(0);
    mapSprite_.setTexCoords(x * 10 + 0x30, y * 10 + 0x10, (x + 1) * 10 + 0x30, (y + 1) * 10 + 0x10);
    mapSprite_.setPosition(x * 10 + 0x30, y * 10 + 0x10);
    mapSprite_.setSize(10, 10);
    mapSprite_.draw();
}

ARM void window::GlobalMap::load()
{
    OS_Wait();
    mapSprite_.allocVram();
    mapSprite_.transferToVram();
    veilSprite_.allocVram();
    veilSprite_.transferToVram();
}

ARM void window::GlobalMap::clear()
{
    mapSprite_.freeVram();
    veilSprite_.freeVram();
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
    mapSprite_.setAlpha(alpha);
    veilSprite_.setAlpha(alpha);
    symbolSprite_.setAlpha(alpha);
    markSprite_.setAlpha(alpha);
}

ARM void window::GlobalMap::playerMapPosition()
{
}
