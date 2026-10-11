#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownStageManager.hpp"

THUMB TownFurnitureControlMove2::TownFurnitureControlMove2()
{
}

THUMB TownFurnitureControlMove2::~TownFurnitureControlMove2()
{
}

THUMB void TownFurnitureControlMove2::execute()
{
    if (enable_) {
        dss::Fix32Vector3 step = (goal_ - start_) / frame_;
        dss::Fix32Vector3 pos = start_ + step * counter_;
        if (counter_ >= frame_) {
            enable_ = 0;
            pos = goal_;
        }
        TownStageManager::getSingleton()->setMapUidPosFX32(uid_, pos);
        counter_++;
    }
}

THUMB void TownFurnitureControlMove2::cleanup()
{
    TownFurnitureControlBase::cleanup();
}
