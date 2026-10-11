#pragma ipa file
#include "ov000/town/TownShopListMap.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/status/StageStatus.hpp"

ARM TownShopListMap::TownShopListMap()
{
    isEnable_ = 0;
    frame_ = 0;
    phase_ = 0;
}

ARM TownShopListMap::~TownShopListMap()
{
}

ARM void TownShopListMap::initialize()
{
    checkData();
    if (isEnable_) {
        shopSprite_.unkfunc_02057d60("data/2d/map/shop.tex", 0);
        shopSprite_.unkfunc_02057ee8();
        shopSprite_.unkfunc_02057f00(0);
        shopSprite_.unkfunc_02057f18(0x3f);
        shopSprite_.sprite_.unk_32 = 1;
        blackSprite_.unkfunc_02057e98(0x100, 0xc0);
        blackSprite_.unkfunc_02057f00(0);
        blackSprite_.unkfunc_02057ee8();
        blackSprite_.unkfunc_02057f40(0, 0, 0);
        blackSprite_.unkfunc_02057f18(0x3f);
    }
    phase_ = 0;
}

ARM void TownShopListMap::cleanup()
{
    if (isEnable_) {
        shopSprite_.unkfunc_02057e34();
    }
    isEnable_ = 0;
}

ARM void TownShopListMap::execute()
{
    if (!isEnable_) {
        return;
    }
    switch (phase_) {
    case 0:
        frame_ = 0;
        break;
    case 1:
        shopSprite_.unkfunc_02057ed4(1);
        shopSprite_.unkfunc_02057f00(frame_);
        frame_++;
        if (frame_ == 31) {
            phase_ = 2;
        }
        break;
    case 2:
        frame_ = 31;
        shopSprite_.unkfunc_02057f00(31);
        shopSprite_.unkfunc_02057ed4(1);
        break;
    case 3:
        frame_ = dss::clamp<int>(frame_, 0, 31);
        shopSprite_.unkfunc_02057f00(frame_);
        frame_--;
        if (frame_ <= 0) {
            shopSprite_.unkfunc_02057ed4(0);
            phase_ = 0;
            cleanup();
        }
        break;
    case 4:
        shopSprite_.unkfunc_02057ed4(0);
        blackSprite_.unkfunc_02057f00(31);
        frame_++;
        if (frame_ == 31) {
            initialize();
            phase_ = 5;
        }
        break;
    case 5:
        shopSprite_.unkfunc_02057ed4(1);
        shopSprite_.unkfunc_02057f00(31 - frame_);
        blackSprite_.unkfunc_02057f00(31);
        if (--frame_ == 0) {
            shopSprite_.unkfunc_02057ed4(1);
            blackSprite_.unkfunc_02057f00(0);
            phase_ = 2;
        }
        break;
    }
}

ARM void TownShopListMap::checkData()
{
    isEnable_ = g_Stage.isShopIcon() != 0;
}

ARM void TownShopListMap::draw()
{
    if (isEnable_) {
        unkfunc_020847e8();
        shopSprite_.unkfunc_02057ec0();
    }
}

ARM void TownShopListMap::open()
{
    initialize();
    phase_ = 1;
}

ARM void TownShopListMap::openBlack()
{
    checkData();
    phase_ = 4;
}

ARM void TownShopListMap::close()
{
    phase_ = 3;
}

ARM int TownShopListMap::isOpen()
{
    return phase_ == 2;
}

ARM int TownShopListMap::isClose()
{
    return phase_ == 0;
}

ARM int TownShopListMap::isEnable()
{
    return isEnable_;
}
