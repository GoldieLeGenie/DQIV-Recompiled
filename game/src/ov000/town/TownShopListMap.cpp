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
        shopSprite_.loadFile("data/2d/map/shop.tex", 0);
        shopSprite_.setSubScreen();
        shopSprite_.setAlpha(0);
        shopSprite_.setPolygonID(0x3f);
        shopSprite_.sprite_.color0Opaque_ = 1;
        blackSprite_.setSize(0x100, 0xc0);
        blackSprite_.setAlpha(0);
        blackSprite_.setSubScreen();
        blackSprite_.setColor(0, 0, 0);
        blackSprite_.setPolygonID(0x3f);
    }
    phase_ = 0;
}

ARM void TownShopListMap::cleanup()
{
    if (isEnable_) {
        shopSprite_.unload();
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
        shopSprite_.setEnabled(1);
        shopSprite_.setAlpha(frame_);
        frame_++;
        if (frame_ == 31) {
            phase_ = 2;
        }
        break;
    case 2:
        frame_ = 31;
        shopSprite_.setAlpha(31);
        shopSprite_.setEnabled(1);
        break;
    case 3:
        frame_ = dss::clamp<int>(frame_, 0, 31);
        shopSprite_.setAlpha(frame_);
        frame_--;
        if (frame_ <= 0) {
            shopSprite_.setEnabled(0);
            phase_ = 0;
            cleanup();
        }
        break;
    case 4:
        shopSprite_.setEnabled(0);
        blackSprite_.setAlpha(31);
        frame_++;
        if (frame_ == 31) {
            initialize();
            phase_ = 5;
        }
        break;
    case 5:
        shopSprite_.setEnabled(1);
        shopSprite_.setAlpha(31 - frame_);
        blackSprite_.setAlpha(31);
        if (--frame_ == 0) {
            shopSprite_.setEnabled(1);
            blackSprite_.setAlpha(0);
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
        setupOrtho2D();
        shopSprite_.draw();
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
