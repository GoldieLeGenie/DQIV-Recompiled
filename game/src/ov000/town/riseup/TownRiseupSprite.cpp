#include "ov000/town/riseup/TownRiseup.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "main/sound/SoundManager.hpp"

THUMB TownRiseupSprite::TownRiseupSprite()
{
}

THUMB TownRiseupSprite::~TownRiseupSprite()
{
}

THUMB void TownRiseupSprite::setup(int type)
{
    TownRiseupBase::setup(type);
    if (startCounter_ == 0) {
        TownFurnitureManager* mgr = TownFurnitureManager::getSingleton();
        if (type == mgr->twinkleEffect) {
            SoundManager::playSe(0x23e, 0);
        }
        sprite_.start();
        phase_ = SPRITE_ANIMATION;
    } else {
        phase_ = SPRITE_START_WAIT;
    }
}

THUMB void TownRiseupSprite::setupNear(int flag)
{
    if (flag == 1) {
        sprite_.setDisplayType(3);
    } else {
        sprite_.setDisplayType(0);
    }
}

THUMB void TownRiseupSprite::setResource(void* resource)
{
    sprite_.setup((cmn::CommonEffectData*)resource, 1);
}

THUMB void TownRiseupSprite::execute()
{
    if (enable_ == 0) {
        return;
    }
    switch (phase_) {
    case SPRITE_MOVE:
        if (counter_ < frame_) {
            position_ = start_ + move_ * counter_;
        } else {
            position_ = end_;
        }
        setPosition(position_);
        counter_++;
        if (counter_ >= frame_) {
            cleanup();
        }
        break;
    case SPRITE_FADE_IN:
        alpha_.value = (counter_ << 12) / frame_;
        sprite_.setAlpha(alpha_);
        counter_++;
        if (counter_ >= frame_) {
            cleanup();
        }
        break;
    case SPRITE_FADE_OUT:
        alpha_.value = ((frame_ - counter_) << 12) / frame_;
        sprite_.setAlpha(alpha_);
        counter_++;
        if (counter_ >= frame_) {
            cleanup();
        }
        break;
    }
}

THUMB void TownRiseupSprite::draw()
{
    DSSAObject::calcType_ = 1;
    if (enable_ != 0) {
        switch (phase_) {
        case SPRITE_START_WAIT:
            if (--startCounter_ <= 0) {
                TownFurnitureManager* mgr = TownFurnitureManager::getSingleton();
                if (index_ == mgr->twinkleEffect) {
                    SoundManager::playSe(0x23e, 0);
                }
                sprite_.start();
                phase_ = SPRITE_ANIMATION;
            }
            break;
        case SPRITE_ANIMATION:
            sprite_.draw();
            if (sprite_.isEnd()) {
                enable_ = 0;
            }
            break;
        case SPRITE_MOVE:
        case SPRITE_FADE_IN:
        case SPRITE_FADE_OUT:
            sprite_.draw();
            if (sprite_.isEnd()) {
                TownFurnitureManager* mgr = TownFurnitureManager::getSingleton();
                if (index_ == mgr->twinkleEffect) {
                    SoundManager::playSe(0x23e, 0);
                }
                sprite_.start();
            }
            break;
        }
    }
    DSSAObject::calcType_ = 0;
}

THUMB void TownRiseupSprite::cleanup()
{
    TownRiseupBase::cleanup();
    sprite_.cleanup(1);
}

THUMB void TownRiseupSprite::setPosition(dss::Fix32Vector3 pos)
{
    position_ = pos;
    sprite_.setPosition(position_);
}

THUMB bool TownRiseupSprite::isFinish()
{
    if (sprite_.isEnd()) {
        return true;
    }
    if (enable_ == 0) {
        return true;
    }
    return false;
}

THUMB void TownRiseupSprite::setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame)
{
    phase_ = SPRITE_MOVE;
    start_ = start;
    end_ = end;
    move_ = (end - start) / frame;
    frame_ = frame;
    counter_ = 0;
    setPosition(start);
}

THUMB void TownRiseupSprite::setScriptFade(dss::Fix32Vector3 pos, int frame, int flag)
{
    if (flag == 0) {
        phase_ = SPRITE_FADE_IN;
        alpha_.value = 0;
    } else {
        phase_ = SPRITE_FADE_OUT;
        alpha_.value = 0x1000;
    }
    start_ = pos;
    end_ = pos;
    frame_ = frame;
    counter_ = 0;
    sprite_.setAlpha(alpha_);
}
