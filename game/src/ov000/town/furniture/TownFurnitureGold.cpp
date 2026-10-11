#include "ov000/town/TownFurniture.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"

THUMB TownFurnitureGold::TownFurnitureGold()
{
}

THUMB TownFurnitureGold::~TownFurnitureGold()
{
}

THUMB void TownFurnitureGold::setSecondMessage()
{
    addPartyGold();
    int chara = 0;
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            chara = i;
            break;
        }
    }
    TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveStatus_.playerIndex_);
    TextAPI::setMACRO0(0x32, 0xf0000000, data_);
    addMessage(0xc411e, true);
}

THUMB void TownFurnitureGold::startRiseup()
{
    index_ = TownRiseupManager::getSingleton()->setup(200, getFurnPosition());
}

THUMB bool TownFurnitureGold::endRiseup()
{
    TownFurnitureObject::endRiseup();
    return false;
}

THUMB void TownFurnitureGold::addPartyGold()
{
    status::g_Party.addGold(data_);
}

THUMB bool TownFurnitureGold::isRiseupEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(index_);
}
