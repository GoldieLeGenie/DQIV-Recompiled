#include "ov000/town/TownCharacter.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "ov000/town/TownCamera.hpp"
#include "DualScreen.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/dss/UnkPaletteEffect.hpp"

ARM TownMonsterDraw::TownMonsterDraw()
{
}

ARM TownMonsterDraw::~TownMonsterDraw()
{
}

ARM void TownMonsterDraw::setup(TOWN_CHARACTER& data)
{
    TownCharacterBase::setup(data);
    monster_.setup(data_.charaIndex);
    monster_.setPosition(data_.position);
    monster_.setCameraType(DSSAObjectWithCamera::Standard);
    display_ = 1;
    monster_.pause(0);
    defaultIndex_ = data_.charaIndex;
}

ARM void TownMonsterDraw::cleanup()
{
    if (data_.enable) {
        data_.enable = 0;
        monster_.cleanup();
    }
}

ARM void TownMonsterDraw::execute()
{
    TownCharacterBase::execute();
}

ARM void TownMonsterDraw::draw()
{
    if (!display_) {
        return;
    }
    DSSAObject::calcType_ = 0;
    bool even = (unkfunc_02081254() & 1) == 0;
    TownCamera* camera = TownCamera::getSingleton();
    unkfunc_0212ed58(!even || pc::unifiedFrame() ? &camera->camera_.unk_004 : &camera->camera_.unk_068);
    monster_.draw();
    dss::Fix32Vector3 pos = *monster_.getPosition();
    DSSAObject::calcType_ = 1;
}

ARM void TownMonsterDraw::unkfunc_0212ed58(dss::Camera* camera)
{
    DSSAObjectWithCamera::camera_ = camera;
}

ARM void TownMonsterDraw::setDisplay(int flag)
{
    display_ = flag;
}

ARM int TownMonsterDraw::isDisplay()
{
    return display_;
}

ARM void TownMonsterDraw::setNearCharacter(int flag)
{
    if (flag == 1) {
        monster_.setCameraType(DSSAObjectWithCamera::Normal2);
    } else {
        monster_.setCameraType(DSSAObjectWithCamera::Standard);
        monster_.setScale(DSSAObject::getDefaultScale2());
    }
}

ARM void TownMonsterDraw::setAnimation(int flag)
{
    monster_.pause(flag == 0);
}

ARM void TownMonsterDraw::setPosition(dss::Fix32Vector3& pos)
{
    data_.position = pos;
    monster_.setPosition(pos);
}

ARM void TownMonsterDraw::setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b)
{
    void* palette = monster_.texture_;
    dss::Fix32 rate[3];
    rate[0] = r;
    rate[1] = g;
    rate[2] = b;
    unkfunc_02086034((TextureObject*)palette, rate);
}

ARM void TownMonsterDraw::setMotion(int motion, int loop)
{
    monster_.start(motion, loop);
}

ARM bool TownMonsterDraw::isMotion()
{
    return monster_.currentAnimationIndex_ == 0;
}

ARM void TownMonsterDraw::setDir(int dir)
{
}

ARM int TownMonsterDraw::getDir()
{
    return 0;
}

ARM void TownMonsterDraw::changePose(int pose)
{
    monster_.cleanup();
    data_.charaIndex = pose;
    monster_.setup(pose);
    monster_.setPosition(data_.position);
    monster_.setCameraType(DSSAObjectWithCamera::Standard);
    display_ = 1;
    monster_.pause(0);
}

ARM void TownMonsterDraw::restorePose()
{
    monster_.cleanup();
    data_.charaIndex = defaultIndex_;
    monster_.setup(data_.charaIndex);
    monster_.setPosition(data_.position);
    monster_.setCameraType(DSSAObjectWithCamera::Standard);
    display_ = 1;
    monster_.pause(0);
}

ARM void TownMonsterDraw::requestReload()
{
    int anim = monster_.currentAnimationIndex_;
    monster_.cleanup();
    monster_.setup(data_.charaIndex);
    monster_.setPosition(data_.position);
    monster_.setCameraType(DSSAObjectWithCamera::Standard);
    monster_.pause(0);
    monster_.start(anim, 1);
}

ARM void TownMonsterDraw::setAlpha(unsigned char alpha)
{
    monster_.setAlpha(alpha);
}
