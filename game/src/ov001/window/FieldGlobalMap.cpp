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
    unk_150.unload();
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
    unk_150.loadFile(path, 0);
    unk_150.setSubScreen();
    unk_150.setPolygonID(0x34);
    unk_150.setDepth(0xa);
    unk_150.setAlpha(0);
    unk_150.setEnabled(1);
    unk_150.setTexCoords(8, 7, 0x18, 0x18);
    unk_150.setSize(0x10, 0x10);
    window::GlobalMap::setup();
}

ARM void FieldGlobalMap::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    unk_150.setPosition(mapPos.vx, mapPos.vy - 0xe);
}

ARM void FieldGlobalMap::setAlpha(unsigned char alpha)
{
    unk_150.setAlpha(alpha);
    window::GlobalMap::setAlpha(alpha);
}

ARM void FieldGlobalMap::draw()
{
    window::GlobalMap::draw();
    unk_150.draw();
}

ARM void FieldGlobalMap::load()
{
    unk_150.transferToVram();
    window::GlobalMap::load();
}

ARM void FieldGlobalMap::clear()
{
    window::GlobalMap::clear();
    unk_150.freeVram();
}
