#pragma ipa file
#include "ov000/town/riseup/TownRiseup.hpp"
#include "main/dss/DssUtils.hpp"

dss::Camera* TownRiseupBase::camera_;
static dss::Fix32 unusedScale(2.0f);
RiseupParam TownRiseupBase::defaultParam[4] = {
    { 0x1a00, 0x200, 0x10, 0x20, 0x146, 0xc, 1 },
    { 0x1a00, 0x200, 0x10, 0x20, 0x144, 0xc, 1 },
    { 0x1a00, 0x200, 0x10, 0x20, 0x145, 0xc, 1 },
    { 0x1a00, 0x200, 0x10, 0xb4, 0x146, 0xc, 1 },
};

THUMB TownRiseupBase::TownRiseupBase()
{
    enable_ = 0;
}

THUMB TownRiseupBase::~TownRiseupBase()
{
}

THUMB void TownRiseupBase::setup(int type)
{
    enable_ = 1;
    index_ = type;
    setGarbageCorrect(true);
}

THUMB void TownRiseupBase::setType(int type)
{
}

THUMB void TownRiseupBase::cleanup()
{
    enable_ = 0;
}

THUMB void TownRiseupBase::execute()
{
}

THUMB void TownRiseupBase::setPosition(dss::Fix32Vector3 pos)
{
    position_ = pos;
}

THUMB void TownRiseupBase::setCamera(dss::Camera* camera)
{
    camera_ = camera;
}

THUMB dss::Camera* TownRiseupBase::getCamera()
{
    return camera_;
}

THUMB void TownRiseupBase::calcNearPos(dss::Fix32Vector3& pos, dss::Fix32 scale)
{
    dss::Fix32Vector3 camera = getCamera()->getPosition();
    dss::Fix32Vector3 position = pos;
    dss::Fix32Vector3 dir = camera - position;
    dss::Fix32 length = dir.length();
    dir.normalize();
    position += dir;
    dss::Fix32 rate = length - dss::Fix32(1L);
    scale = scale * rate / length;
    pos = position;
}

THUMB void TownRiseupBase::setGarbageCorrect(bool flag)
{
    if (flag) {
        flag_.flag_ |= FALG_GARBAGE_CORRECTION;
    } else {
        flag_.flag_ &= ~FALG_GARBAGE_CORRECTION;
    }
}

THUMB void TownRiseupBase::setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame)
{
}

THUMB void TownRiseupBase::setScriptFade(dss::Fix32Vector3 pos, int frame, int flag)
{
}

THUMB void TownRiseupBase::setupNear(int flag)
{
}
