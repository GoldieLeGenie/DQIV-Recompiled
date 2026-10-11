#include "ov000/town/TownFurniture.hpp"

THUMB TownFurnitureNothing::TownFurnitureNothing()
{
}

THUMB TownFurnitureNothing::~TownFurnitureNothing()
{
}

THUMB void TownFurnitureNothing::setSecondMessage()
{
    if (data_ != 0 && (furniture_.flag_ & CHECK_MESSAGE_FLAG)) {
        addMessage(data_, true);
    }
}
