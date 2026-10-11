#include "ov001/window/UnkFieldMap.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/dss/DssUtils.hpp"

ARM UnkFieldMap_0212d214::UnkFieldMap_0212d214()
{
}

ARM UnkFieldMap_0212d214::~UnkFieldMap_0212d214()
{
}

ARM void UnkFieldMap_0212d214::setup(Render* render)
{
    render_ = render;
    setup();
}

ARM void UnkFieldMap_0212d214::cleanup()
{
    unk_100.unkfunc_02057e34();
    unk_08.unkfunc_02057e34();
}

ARM void UnkFieldMap_0212d214::draw()
{
    unkfunc_020847e8();
    unk_08.unkfunc_02057ec0();
    unkfunc_0203822c(unk_150);
}

ARM void UnkFieldMap_0212d214::load()
{
    unk_100.unkfunc_02057d40();
    unk_100.unkfunc_02057d2c();
    unk_08.unkfunc_02057d40();
    unk_08.unkfunc_02057d2c();
}

ARM void UnkFieldMap_0212d214::clear()
{
    unk_08.unkfunc_02057d1c();
    unk_100.unkfunc_02057d1c();
}

ARM void UnkFieldMap_0212d214::setAlpha(unsigned char alpha)
{
    unk_08.unkfunc_02057f00(alpha);
    unk_100.unkfunc_02057f00(alpha);
    unk_58.unkfunc_02057f00(alpha);
    unk_a8.unkfunc_02057f00(alpha);
}

ARM void UnkFieldMap_0212d214::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    unk_100.unkfunc_02057e98(0x10, 0x10);
    unk_100.unkfunc_02057e88(mapPos.vx, mapPos.vy - 0x10);
}

ARM dss::Vector2<int> UnkFieldMap_0212d214::convertMapPos(int x, int y)
{
    dss::Vector2<int> pos;
    pos.vx = (x - 0x400000) / 0x6666 + 0x30;
    pos.vy = (y - 0x400000) / 0x6666 + 0x10;
    return pos;
}
