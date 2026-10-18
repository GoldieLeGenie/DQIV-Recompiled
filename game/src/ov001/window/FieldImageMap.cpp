#include "ov001/window/FieldImageMap.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "ov001/fld/FieldSystem.hpp"
#include "main/global/Global.hpp"
#include "main/dss/Camera.hpp"

ARM FieldImageMap::FieldImageMap()
{
    alpha_ = 0;
    phase_ = 0;
}

ARM FieldImageMap::~FieldImageMap()
{
}

ARM void FieldImageMap::setup(FieldSystem* system)
{
    system_ = system;
    switch (g_Global.getFieldType()) {
    case 0:
        map_ = &unk_0ac;
        break;
    case 1:
        map_ = &unk_24c;
        break;
    case 2:
        map_ = &unk_3a0;
        break;
    }
    map_->setup(&system_->render_);
    unk_008.setup(system_);
    phase_ = 0;
}

ARM void FieldImageMap::cleanup()
{
    map_->cleanup();
    unk_008.cleanup();
    system_ = NULL;
}

ARM void FieldImageMap::execute()
{
    switch (phase_) {
    case 2:
        alpha_ = 0x1f;
        map_->playerMapPosition();
        map_->setAlpha(alpha_);
        break;
    case 3:
        if (--alpha_ < 0) {
            map_->clear();
            phase_ = 4;
            alpha_ = 0;
        }
        map_->playerMapPosition();
        map_->setAlpha(alpha_);
        break;
    case 4:
        if (unkfunc_02081254() & 1) {
            return;
        }
        phase_ = 5;
        unk_008.load();
        break;
    case 5:
        if (++alpha_ >= 0x1f) {
            phase_ = 7;
            alpha_ = 0x1f;
        }
        unk_008.setAlpha(alpha_);
        break;
    case 6:
        break;
    case 7:
        alpha_ = 0x1f;
        unk_008.setAlpha(alpha_);
        break;
    case 8:
        if (--alpha_ < 0) {
            unk_008.clear();
            phase_ = 0;
            alpha_ = 0;
        }
        unk_008.setAlpha(alpha_);
        break;
    case 0:
        if (unkfunc_02081254() & 1) {
            return;
        }
        map_->load();
        phase_ = 1;
        break;
    case 1:
        if (++alpha_ >= 0x1f) {
            phase_ = 2;
            alpha_ = 0x1f;
        }
        map_->playerMapPosition();
        map_->setAlpha(alpha_);
        break;
    case 9:
        break;
    }
}

ARM void FieldImageMap::draw()
{
    map_->draw();
}

ARM void FieldImageMap::startClose()
{
    phase_ = 8;
}

ARM void FieldImageMap::startOpen()
{
    phase_ = 3;
}

ARM void FieldImageMap::open()
{
    startOpen();
}

ARM void FieldImageMap::close()
{
    startClose();
}

ARM int FieldImageMap::isOpen()
{
    return phase_ == 7;
}

ARM int FieldImageMap::isClose()
{
    return phase_ == 2;
}

ARM void FieldImageMap::clearAllMap()
{
    map_->clear();
    unk_008.clear();
    phase_ = 9;
}
