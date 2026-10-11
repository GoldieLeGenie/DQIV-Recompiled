#pragma ipa file
#include "ov001/window/UnkFieldMap.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/dss/DssUtils.hpp"

ARM UnkFieldMap_0212cff8::UnkFieldMap_0212cff8()
{
}

ARM UnkFieldMap_0212cff8::~UnkFieldMap_0212cff8()
{
}

ARM void UnkFieldMap_0212cff8::cleanup()
{
    unk_150.unkfunc_02057e34();
    window::GlobalMap::cleanup();
}

ARM void UnkFieldMap_0212cff8::setup(Render* render)
{
    window::GlobalMap::setup(render);
}

ARM void UnkFieldMap_0212cff8::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/point_s.tex");
    unk_150.unkfunc_02057d60(path, 0);
    unk_150.unkfunc_02057ee8();
    unk_150.unkfunc_02057f18(0x34);
    unk_150.unkfunc_02057f30(0xa);
    unk_150.unkfunc_02057f00(0);
    unk_150.unkfunc_02057ed4(1);
    unk_150.unkfunc_02057ea8(8, 7, 0x18, 0x18);
    unk_150.unkfunc_02057e98(0x10, 0x10);
    window::GlobalMap::setup();
}

ARM void UnkFieldMap_0212cff8::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    unk_150.unkfunc_02057e88(mapPos.vx, mapPos.vy - 0xe);
}

ARM void UnkFieldMap_0212cff8::setAlpha(unsigned char alpha)
{
    unk_150.unkfunc_02057f00(alpha);
    window::GlobalMap::setAlpha(alpha);
}

ARM void UnkFieldMap_0212cff8::draw()
{
    window::GlobalMap::draw();
    unk_150.unkfunc_02057ec0();
}

ARM void UnkFieldMap_0212cff8::load()
{
    unk_150.unkfunc_02057d2c();
    window::GlobalMap::load();
}

ARM void UnkFieldMap_0212cff8::clear()
{
    window::GlobalMap::clear();
    unk_150.unkfunc_02057d1c();
}
