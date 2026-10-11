#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownStageManager.hpp"

THUMB TownFurnitureControlFade::TownFurnitureControlFade()
{
}

THUMB TownFurnitureControlFade::~TownFurnitureControlFade()
{
}

THUMB void TownFurnitureControlFade::execute()
{
    if (enable_) {
        counter_++;
        int alpha;
        if (fade_) {
            alpha = 31 - counter_ * 31 / frame_;
        } else {
            alpha = counter_ * 31 / frame_;
        }
        TownStageManager::getSingleton()->setMapUidAlpha(uid_, alpha, priority_);
        if (counter_ >= frame_) {
            enable_ = 0;
        }
    }
}

THUMB void TownFurnitureControlFade::cleanup()
{
    TownFurnitureControlBase::cleanup();
}

THUMB void TownFurnitureControlFade::setFurnitureFade(int uid, int frame, int fade, int priority)
{
    fade_ = fade;
    priority_ = priority;
    TownFurnitureControlBase::setup(uid, frame);
}
