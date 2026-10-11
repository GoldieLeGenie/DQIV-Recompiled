#include "ov000/town/riseup/TownRiseup.hpp"

THUMB TownRiseupScriptMove::TownRiseupScriptMove()
{
}

THUMB TownRiseupScriptMove::~TownRiseupScriptMove()
{
}

THUMB void TownRiseupScriptMove::setup(int type)
{
    TownRiseupBase::setup(type);
}

THUMB void TownRiseupScriptMove::setResource(void* resource)
{
    item_ = (BillboardItem*)resource;
}

THUMB void TownRiseupScriptMove::execute()
{
    if (enable_ != 0) {
        dss::Fix32Vector3 vec = (end_ - start_);
        position_ = start_ + (vec * counter_ / frame_);
        if (++counter_ >= frame_) {
            cleanup();
        }
    }
}

THUMB void TownRiseupScriptMove::draw()
{
    if (enable_ != 0 && startCounter_ == 0) {
        dss::Fix32Vector3 scale = *item_->getScale();
        dss::Fix32Vector3 pos = position_;
        dss::Fix32 rate = scale.vx;
        calcNearPos(pos, rate);
        item_->setPosition(pos);
        item_->setScale(rate);
        item_->draw();
        item_->setScale(scale);
    }
}

THUMB bool TownRiseupScriptMove::isFinish()
{
    return enable_ == 0;
}

THUMB void TownRiseupScriptMove::setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame)
{
    start_ = start;
    end_ = end;
    frame_ = frame;
    counter_ = 0;
}
