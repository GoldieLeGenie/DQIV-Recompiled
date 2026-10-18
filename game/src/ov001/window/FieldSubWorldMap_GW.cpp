#pragma ipa file
#include "ov001/window/UnkFieldMap.hpp"
#include "main/dss/DssUtils.hpp"

ARM FieldSubWorldMap_GW::FieldSubWorldMap_GW()
{
}

ARM FieldSubWorldMap_GW::~FieldSubWorldMap_GW()
{
}

ARM void FieldSubWorldMap_GW::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/point_s.tex");
    playerSprite_.loadFile(path, 0);
    playerSprite_.attachRender(render_);
    playerSprite_.setSubScreen();
    playerSprite_.setPolygonID(0x34);
    playerSprite_.setTexCoords(8, 7, 0x18, 0x18);
    playerSprite_.setDepth(0xa);
    playerSprite_.setAlpha(0);
    playerSprite_.setEnabled(1);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/gw.tex");
    mapSprite_.loadFile(path, 0);
    mapSprite_.setSubScreen();
    mapSprite_.setPolygonID(0x3e);
    mapSprite_.setDepth(0);
    mapSprite_.setAlpha(0);
    mapSprite_.freeVram();
    playerSprite_.freeVram();
    symbolSprite_.setSubScreen();
    symbolSprite_.setPolygonID(0x3e);
    symbolSprite_.setDepth(4);
    symbolSprite_.setAlpha(0);
    symbolSprite_.setSize(3, 3);
    world_ = 1;
}
