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
    unk_100.loadFile(path, 0);
    unk_100.attachRender(render_);
    unk_100.setSubScreen();
    unk_100.setPolygonID(0x34);
    unk_100.setTexCoords(8, 7, 0x18, 0x18);
    unk_100.setDepth(0xa);
    unk_100.setAlpha(0);
    unk_100.setEnabled(1);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/gw.tex");
    unk_08.loadFile(path, 0);
    unk_08.setSubScreen();
    unk_08.setPolygonID(0x3e);
    unk_08.setDepth(0);
    unk_08.setAlpha(0);
    unk_08.freeVram();
    unk_100.freeVram();
    unk_58.setSubScreen();
    unk_58.setPolygonID(0x3e);
    unk_58.setDepth(4);
    unk_58.setAlpha(0);
    unk_58.setSize(3, 3);
    unk_150 = 1;
}
