#include "ov001/window/UnkFieldMap.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/dss/DssUtils.hpp"

ARM FieldSubWorldMap::FieldSubWorldMap()
{
}

ARM FieldSubWorldMap::~FieldSubWorldMap()
{
}

ARM void FieldSubWorldMap::setup(Render* render)
{
    render_ = render;
    setup();
}

ARM void FieldSubWorldMap::cleanup()
{
    playerSprite_.unload();
    mapSprite_.unload();
}

ARM void FieldSubWorldMap::draw()
{
    setupOrtho2D();
    mapSprite_.draw();
    drawSymbols(world_);
}

ARM void FieldSubWorldMap::load()
{
    playerSprite_.allocVram();
    playerSprite_.transferToVram();
    mapSprite_.allocVram();
    mapSprite_.transferToVram();
}

ARM void FieldSubWorldMap::clear()
{
    mapSprite_.freeVram();
    playerSprite_.freeVram();
}

ARM void FieldSubWorldMap::setAlpha(unsigned char alpha)
{
    mapSprite_.setAlpha(alpha);
    playerSprite_.setAlpha(alpha);
    symbolSprite_.setAlpha(alpha);
    markSprite_.setAlpha(alpha);
}

ARM void FieldSubWorldMap::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    playerSprite_.setSize(0x10, 0x10);
    playerSprite_.setPosition(mapPos.vx, mapPos.vy - 0x10);
}

ARM dss::Vector2<int> FieldSubWorldMap::convertMapPos(int x, int y)
{
    dss::Vector2<int> pos;
    pos.vx = (x - 0x400000) / 0x6666 + 0x30;
    pos.vy = (y - 0x400000) / 0x6666 + 0x10;
    return pos;
}
