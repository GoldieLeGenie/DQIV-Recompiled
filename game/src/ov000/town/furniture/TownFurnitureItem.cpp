#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"

THUMB TownFurnitureItem::TownFurnitureItem()
{
}

THUMB TownFurnitureItem::~TownFurnitureItem()
{
}

THUMB void TownFurnitureItem::setSecondMessage()
{
    status::g_Party.setPlayerMode();
    if (data_ == 0xffff) {
        data_ = 0;
    }
    TextAPI::setMACRO0(0xa, 0x40000000, data_);
    addMessage(0xc40db, true);
    int result = addPlayerItem();
    int player = 0;
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            player = status::g_Party.getPlayerIndex(i);
            break;
        }
    }
    if (player == 0) {
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                player = status::g_Party.getPlayerIndex(i);
                break;
            }
        }
    }
    int item = data_;
    if (item != 0x84) {
        if ((char)((status::excelParam.getItemData()[item].byte_2 & (1 << 4)) >> 4) || item == 0x84) {
            TownWindowSystem::getSingleton()->waitCommonMessage();
        }
        TextAPI::setMACRO0(0xa, 0x40000000, data_);
        if (result > -1) {
            if (status::PartyStatus::getPlayerStatusForPlayerIndex(result)->haveStatusInfo_.isDeath()) {
                TextAPI::setMACRO0(1, 0x50000000, player);
                TextAPI::setMACRO0(0x12, 0x50000000, result);
                addMessage(0xc3d28, true);
                return;
            }
            TextAPI::setMACRO0(0x12, 0x50000000, result);
            addMessage(0xc40de, true);
            return;
        }
        TextAPI::setMACRO0(0xa, 0x40000000, data_);
        addMessage(0xc40e0, true);
        return;
    }
    if (item == 0x84 && status::g_Party.isFirstMedalCoin() == 1) {
        addMessage(0xc4128, true);
        addMessage(0xc4129, true);
    }
}

THUMB void TownFurnitureItem::startRiseup()
{
    if (data_ == 0x84) {
        index_ = TownRiseupManager::getSingleton()->setupMedal(getFurnPosition());
    } else {
        index_ = TownRiseupManager::getSingleton()->setup(data_, getFurnPosition());
    }
    counter_ = 0;
}

THUMB bool TownFurnitureItem::endRiseup()
{
    TownFurnitureObject::endRiseup();
    return false;
}

THUMB int TownFurnitureItem::addPlayerItem()
{
    status::g_Party.setPlayerMode();
    if (data_ == 0x84) {
        status::g_Party.addPlayerMedalCoin(1);
        return status::g_Party.getPlayerIndex(0);
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() && status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() < 12) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.add(data_);
            cmn::PartyTalk::getSingleton()->resetPartyTalk();
            cmn::PartyTalk::getSingleton()->setPreItem(data_);
            return status::g_Party.getPlayerIndex(i);
        }
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() < 12) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.add(data_);
            cmn::PartyTalk::getSingleton()->resetPartyTalk();
            cmn::PartyTalk::getSingleton()->setPreItem(data_);
            return status::g_Party.getPlayerIndex(i);
        }
    }
    status::g_Party.haveItemSack_.add(data_);
    cmn::PartyTalk::getSingleton()->resetPartyTalk();
    cmn::PartyTalk::getSingleton()->setPreItem(data_);
    return -1;
}

THUMB bool TownFurnitureItem::isRiseupEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(index_);
}

THUMB bool TownFurnitureItem::soundStart()
{
    if (counter_ == 0) {
        SoundManager::playRestart(0x30, 0xf);
    }
    if (counter_ == SOUND_WAIT) {
        TownWindowSystem::getSingleton()->clearCommonMessage();
        if (data_ == 0x84) {
            status::g_Party.isFirstMedalCoin();
        }
        return true;
    }
    counter_++;
    return false;
}
