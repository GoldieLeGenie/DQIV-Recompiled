#include "ov000/town/riseup/TownRiseup.hpp"
#include "main/sound/SoundManager.hpp"

THUMB TownRiseupMedal::TownRiseupMedal()
{
}

THUMB TownRiseupMedal::~TownRiseupMedal()
{
}

THUMB void TownRiseupMedal::setup(int type)
{
    TownRiseupBase::setup(type);
    param = &defaultParam[3];
    endCounter_ = param->endWait_;
    startCounter_ = param->startWait_;
    phase_ = RISEUP_START_WAIT;
    alpha_ = 1;
    sprite_.setDisplayType(2);
}

THUMB void TownRiseupMedal::setType(int type)
{
    param = &defaultParam[type];
}

THUMB void TownRiseupMedal::setResource(void* resource)
{
    sprite_.setup((cmn::CommonEffectData*)resource, 1);
}

THUMB void TownRiseupMedal::setPosition(dss::Fix32Vector3 pos)
{
    height_ = pos.vy.value;
    TownRiseupBase::setPosition(pos);
    sprite_.setPosition(position_);
}

THUMB void TownRiseupMedal::execute()
{
    if (enable_ == 0) {
        return;
    }
    switch (phase_) {
    case RISEUP_NONE:
        break;
    case RISEUP_START_WAIT:
        if (--startCounter_ <= 0) {
            SoundManager::playRestart(0x30, 0xf);
            phase_ = RISEUP_RISING;
            sprite_.start();
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
        if (alpha_ == 0) {
            phase_ = RISEUP_NONE;
            enable_ = 0;
        }
        break;
    }
}

THUMB void TownRiseupMedal::draw()
{
    if (enable_ != 0 && startCounter_ == 0) {
        dss::Fix32 scale = sprite_.rate_;
        dss::Fix32Vector3 pos = position_;
        dss::Fix32 rate = scale;
        calcNearPos(pos, rate);
        sprite_.setPosition(pos);
        sprite_.setScale(rate);
        sprite_.draw();
        sprite_.setScale(scale);
        if (sprite_.isEnd()) {
            enable_ = 0;
        }
    }
}

THUMB void TownRiseupMedal::cleanup()
{
    TownRiseupBase::cleanup();
    sprite_.cleanup(1);
}

THUMB bool TownRiseupMedal::isFinish()
{
    if (phase_ == RISEUP_FADE_OUT) {
        return true;
    }
    if (enable_ == 0) {
        return true;
    }
    return false;
}
