#include "ov000/town/TownFurnitureControl.hpp"

THUMB TownFurnitureControlBase::TownFurnitureControlBase()
{
    enable_ = 0;
}

THUMB TownFurnitureControlBase::~TownFurnitureControlBase()
{
}

THUMB void TownFurnitureControlBase::setup(int uid, int frame)
{
    enable_ = 1;
    uid_ = uid;
    frame_ = frame;
    counter_ = 0;
    flag_.flag_ |= FLAG_GARBAGE_CORRECT;
}

THUMB void TownFurnitureControlBase::cleanup()
{
    enable_ = 0;
}

THUMB void TownFurnitureControlBase::execute()
{
}

THUMB void TownFurnitureControlBase::setFurnitureMove(int uid, int frame, dss::Fix32Vector3& start, dss::Fix32Vector3& goal)
{
}

THUMB void TownFurnitureControlBase::setFurnitureFade(int uid, int frame, int fade, int priority)
{
}

THUMB void TownFurnitureControlBase::setGarbageCorrect(bool flag)
{
    if (flag) {
        flag_.flag_ |= FLAG_GARBAGE_CORRECT;
    } else {
        flag_.flag_ &= ~FLAG_GARBAGE_CORRECT;
    }
}

THUMB bool TownFurnitureControlBase::isGarbageCorrect()
{
    return flag_.flag_ & FLAG_GARBAGE_CORRECT;
}
