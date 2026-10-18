#pragma ipa file
#include "ov001/window/UnkFieldMap.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/dss/DssUtils.hpp"

ARM FieldGlobalMap::FieldGlobalMap()
{
}

ARM FieldGlobalMap::~FieldGlobalMap()
{
}

ARM void FieldGlobalMap::cleanup()
{
    playerSprite_.unload();
    window::GlobalMap::cleanup();
}

ARM void FieldGlobalMap::setup(Render* render)
{
    window::GlobalMap::setup(render);
}

ARM void FieldGlobalMap::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/point_s.tex");
    playerSprite_.loadFile(path, 0);
    playerSprite_.setSubScreen();
    playerSprite_.setPolygonID(0x34);
    playerSprite_.setDepth(0xa);
    playerSprite_.setAlpha(0);
    playerSprite_.setEnabled(1);
    playerSprite_.setTexCoords(8, 7, 0x18, 0x18);
    playerSprite_.setSize(0x10, 0x10);
    window::GlobalMap::setup();
}

ARM void FieldGlobalMap::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    playerSprite_.setPosition(mapPos.vx, mapPos.vy - 0xe);
}

ARM void FieldGlobalMap::setAlpha(unsigned char alpha)
{
    playerSprite_.setAlpha(alpha);
    window::GlobalMap::setAlpha(alpha);
}

ARM void FieldGlobalMap::draw()
{
    window::GlobalMap::draw();
    playerSprite_.draw();
}

ARM void FieldGlobalMap::load()
{
    playerSprite_.transferToVram();
    window::GlobalMap::load();
}

ARM void FieldGlobalMap::clear()
{
    window::GlobalMap::clear();
    playerSprite_.freeVram();
}
