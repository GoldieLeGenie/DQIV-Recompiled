#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/cmn/PartyTalk.hpp"

THUMB TownFurnitureMessage::TownFurnitureMessage()
{
}

THUMB TownFurnitureMessage::~TownFurnitureMessage()
{
}

THUMB void TownFurnitureMessage::setFirstMessage()
{
    if (checkMsg() != 0 || common_->normalMsg != 0) {
        if (common_->checkMsg != 0) {
            addMessage(common_->checkMsg, false);
        }
        if (data_ != 0) {
            TownWindowSystem::getSingleton()->waitCommonMessage();
        }
    }
}

THUMB void TownFurnitureMessage::setSecondMessage()
{
    if (data_ != 0) {
        addMessage(data_, true);
        cmn::PartyTalk::getSingleton()->setPreMessageNo(data_);
    }
}
