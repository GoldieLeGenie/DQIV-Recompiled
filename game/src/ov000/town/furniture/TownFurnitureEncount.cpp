#include "ov000/town/TownFurniture.hpp"
#include "main/encount/Encount.hpp"
#include "main/status/StageStatus.hpp"

THUMB TownFurnitureEncount::TownFurnitureEncount()
{
}

THUMB TownFurnitureEncount::~TownFurnitureEncount()
{
}

THUMB void TownFurnitureEncount::setupExtend(int data)
{
    monster_ = data;
}

THUMB void TownFurnitureEncount::setSecondMessage()
{
    TextAPI::setMACRO0(0x13, 0x60000000, monster_);
    addMessage(0xc40d5, true);
}

THUMB void TownFurnitureEncount::cleanup()
{
    TownFurnitureObject::cleanup();
    encount::Encount::getSingleton()->forceBrew(data_);
    g_Stage.encountMapUid_ = uid_;
}
