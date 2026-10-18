#include "ov000/town/TownGlobalMap.hpp"
#include "ov000/town/TownSystem.hpp"
#include "main/dss/Camera.hpp"

static dss::Camera camera;

ARM TownGlobalMap::TownGlobalMap()
{
    isEnable_ = 0;
    frame_ = 0;
    phase_ = 0;
}

ARM TownGlobalMap::~TownGlobalMap()
{
}

ARM void TownGlobalMap::setup(TownSystem* system)
{
    window_.setup(&system->render_);
    window_.load();
}

ARM void TownGlobalMap::cleanup()
{
    if (isEnable_) {
        window_.cleanup();
    }
}

ARM void TownGlobalMap::execute()
{
    if (!isEnable_) {
        return;
    }
    switch (phase_) {
    case 0:
        frame_ = 0;
        break;
    case 1:
        if (++frame_ == 0x1f) {
            phase_ = 2;
        }
        break;
    case 2:
        frame_ = 0x1f;
        break;
    case 3:
        if (--frame_ <= 0) {
            phase_ = 0;
            cleanup();
            isEnable_ = 0;
        }
        break;
    }
    window_.setAlpha(frame_);
}

ARM void TownGlobalMap::draw()
{
    if (isEnable_) {
        window_.draw();
    }
}

ARM void TownGlobalMap::open()
{
    isEnable_ = 1;
    setup(TownSystem::getSingleton());
    phase_ = 1;
}

ARM void TownGlobalMap::close()
{
    phase_ = 3;
}

ARM int TownGlobalMap::isOpen()
{
    return phase_ == 2;
}

ARM int TownGlobalMap::isClose()
{
    return phase_ == 0;
}

ARM int TownGlobalMap::isEnable()
{
    return isEnable_;
}
