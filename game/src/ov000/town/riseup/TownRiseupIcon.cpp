#include "ov000/town/riseup/TownRiseup.hpp"
#include "main/sound/SoundManager.hpp"

THUMB TownRiseupIcon::TownRiseupIcon()
{
}

THUMB TownRiseupIcon::~TownRiseupIcon()
{
}

THUMB void TownRiseupIcon::setup(int type)
{
    TownRiseupBase::setup(type);
    if (type <= 0xa1 || type == 0xc8) {
        param = &defaultParam[0];
    } else if (type == 0xc9) {
        param = &defaultParam[1];
    } else {
        param = &defaultParam[2];
    }
    endCounter_ = param->endWait_;
    startCounter_ = param->startWait_;
    phase_ = RISEUP_START_WAIT;
    alpha_ = 1;
}

THUMB void TownRiseupIcon::setType(int type)
{
    param = &defaultParam[type];
}

THUMB void TownRiseupIcon::setResource(void* resource)
{
    item_ = (BillboardItem*)resource;
}

THUMB void TownRiseupIcon::setPosition(dss::Fix32Vector3 pos)
{
    height_ = pos.vy.value;
    TownRiseupBase::setPosition(pos);
}

THUMB void TownRiseupIcon::execute()
{
    if (enable_ == 0) {
        return;
    }
    switch (phase_) {
    case RISEUP_NONE:
        break;
    case RISEUP_START_WAIT:
        if (--startCounter_ <= 0) {
            SoundManager::playSe(param->sound_, 0);
            phase_ = RISEUP_RISING;
        }
        break;
    case RISEUP_RISING:
        if (position_.vy.value >= param->maxHigh_ + height_) {
            phase_ = RISEUP_END_WAIT;
            break;
        }
        alpha_ += param->fadein_;
        if (alpha_ > 31) {
            alpha_ = 31;
        }
        position_.vy.value += param->velocity_;
        if (position_.vy.value > param->maxHigh_ + height_) {
            position_.vy.value = param->maxHigh_ + height_;
        }
        break;
    case RISEUP_END_WAIT:
        if (--endCounter_ <= 0) {
            phase_ = RISEUP_FADE_OUT;
        }
        break;
    case RISEUP_FADE_OUT:
        alpha_ -= param->fadeout_;
        if (alpha_ <= 0) {
            phase_ = RISEUP_NONE;
            enable_ = 0;
        }
        break;
    }
}

THUMB void TownRiseupIcon::draw()
{
    if (enable_ != 0 && startCounter_ == 0) {
        dss::Fix32Vector3 scale = *item_->getScale();
        dss::Fix32Vector3 pos = position_;
        dss::Fix32 rate = scale.vx;
        calcNearPos(pos, rate);
        item_->setPosition(pos);
        item_->setScale(rate);
        item_->setAlpha((unsigned char)alpha_);
        item_->draw();
        item_->setScale(scale);
    }
}

THUMB bool TownRiseupIcon::isFinish()
{
    if (phase_ == RISEUP_FADE_OUT) {
        return true;
    }
    if (enable_ == 0) {
        return true;
    }
    return false;
}
