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
    unk_100.unload();
    unk_08.unload();
}

ARM void UnkFieldMap_0212d214::draw()
{
    setupOrtho2D();
    unk_08.draw();
    drawSymbols(unk_150);
}

ARM void UnkFieldMap_0212d214::load()
{
    unk_100.allocVram();
    unk_100.transferToVram();
    unk_08.allocVram();
    unk_08.transferToVram();
}

ARM void UnkFieldMap_0212d214::clear()
{
    unk_08.freeVram();
    unk_100.freeVram();
}

ARM void UnkFieldMap_0212d214::setAlpha(unsigned char alpha)
{
    unk_08.setAlpha(alpha);
    unk_100.setAlpha(alpha);
    unk_58.setAlpha(alpha);
    unk_a8.setAlpha(alpha);
}

ARM void UnkFieldMap_0212d214::playerMapPosition()
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    dss::Vector2<int> mapPos = convertMapPos(pos.vx.value, pos.vy.value);
    unk_100.setSize(0x10, 0x10);
    unk_100.setPosition(mapPos.vx, mapPos.vy - 0x10);
}

ARM dss::Vector2<int> UnkFieldMap_0212d214::convertMapPos(int x, int y)
{
    dss::Vector2<int> pos;
    pos.vx = (x - 0x400000) / 0x6666 + 0x30;
    pos.vy = (y - 0x400000) / 0x6666 + 0x10;
    return pos;
}
