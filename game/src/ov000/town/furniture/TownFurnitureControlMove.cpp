#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownStageManager.hpp"

THUMB TownFurnitureControlMove::TownFurnitureControlMove()
{
}

THUMB TownFurnitureControlMove::~TownFurnitureControlMove()
{
}

THUMB void TownFurnitureControlMove::setFurnitureMove(int uid, int frame, dss::Fix32Vector3& start, dss::Fix32Vector3& goal)
{
    start_ = start;
    goal_ = goal;
    TownFurnitureControlBase::setup(uid, frame);
}

THUMB void TownFurnitureControlMove::execute()
{
    if (enable_) {
        counter_++;
        dss::Fix32Vector3 diff = goal_ - start_;
        dss::Fix32Vector3 pos = start_ + (diff * counter_ / frame_);
        TownStageManager::getSingleton()->setMapUidPosFX32(uid_, pos);
        if (counter_ >= frame_) {
            enable_ = 0;
        }
    }
}

THUMB void TownFurnitureControlMove::cleanup()
{
    TownFurnitureControlBase::cleanup();
}
