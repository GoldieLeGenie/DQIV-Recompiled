#include "ov016/TownMenu_ITEM/TownMenuItemMessage.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectCommand.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "ov016/UnkTownMenu_02176fa0/UnkTownMenu_02176fa0.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"

THUMB void TownMenuItemMessage::menuSetup()
{
    MenuSoundManager::getSingleton()->initialize();
    cursedItem_ = 0;
}

THUMB void TownMenuItemMessage::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK || stat == MENUBASE_STAT_CANCEL) {
            messageUpdate(stat);
        }
        return;
    }
    if (!MenuSoundManager::getSingleton()->isPlaySound()) {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(7, 0x40000000, cursedItem_);
        TextAPI::setMACRO0(10, 0x40000000, cursedItem_);
        if (messageMode_ == MESS_EQIP_NORMAL) {
            data_020ed1bc.addMessage(0xc3a53);
        } else {
            data_020ed1bc.addMessage(0xc3d8d);
        }
    }
}

THUMB void TownMenuItemMessage::setMessageMenu(TOWN_MENU_ITEM_MESSAGE_TYPE type)
{
    switch (type) {
    case MESS_NONE:
        break;
    case MESS_EQIP_NORMAL:
        messageMode_ = MESS_EQIP_NORMAL;
        setItemEqipNormal();
        break;
    case MESS_EQIP_NOROI:
        messageMode_ = MESS_EQIP_NOROI;
        setItemEqipNoroi();
        break;
    case MESS_THROW_OK:
        messageMode_ = MESS_THROW_OK;
        setItemThrowAffi();
        break;
    case MESS_THROW_NG:
        messageMode_ = MESS_THROW_NG;
        setItemThrowNG();
        break;
    case MESS_THROW_DIFFICULT:
        messageMode_ = MESS_THROW_DIFFICULT;
        setItemThrowAffi();
        break;
    }
}

THUMB void TownMenuItemMessage::messageUpdate(int stat)
{
    data_020ed1bc.close();
    switch (messageMode_) {
    case MESS_NONE:
        break;
    case MESS_EQIP_NORMAL:
    case MESS_EQIP_NOROI:
        endMessageToItemSelect();
        break;
    case MESS_THROW_OK:
        if (stat == MENUBASE_STAT_OK) {
            setItemThrow();
        } else if (stat == MENUBASE_STAT_CANCEL) {
            throwEndToReturnMenu();
        }
        break;
    case MESS_THROW_NG:
        endMessageToItemSelect();
        break;
    case MESS_THROW_DIFFICULT:
        if (stat == MENUBASE_STAT_OK) {
            setItemThrowDifficult();
        } else if (stat == MENUBASE_STAT_CANCEL) {
            endMessageToCommandSelect();
        }
        break;
    case MESS_THROW_END:
        throwEndToReturnMenu();
        break;
    }
}

THUMB void TownMenuItemMessage::setItemEqipNoroi()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    cursedItem_ = status::PlayerItemInfo::getEquipItemIdByType(activeChara, status::UseItem::getItemType(status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll())));
    MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
    gTownMenuItemSelectCommand.sound_ = 1;
}

THUMB void TownMenuItemMessage::setItemEqipNormal()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    int itemId = status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    if (status::UseItem::isCurse(itemId) && status::g_Party.getPlayerIndex(activeChara) != 0x19) {
        cursedItem_ = itemId;
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
        gTownMenuItemSelectCommand.sound_ = 1;
        return;
    }
    status::PlayerStatus* playerStatus = status::g_Party.getPlayerStatus(activeChara);
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara));
    TextAPI::setMACRO0(10, 0x40000000, itemId);
    if (playerStatus->haveStatusInfo_.isDeath() == 1) {
        int count = 0;
        while (status::g_Party.getPlayerStatus(count)->haveStatusInfo_.isDeath() == 1) {
            count++;
            if (count > status::g_Party.getCount()) {
                count = 0;
                break;
            }
        }
        TextAPI::setMACRO0(12, 0x50000000, status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.playerIndex_);
        data_020ed1bc.addMessage(0xc3d45);
        return;
    }
    data_020ed1bc.addMessage(0xc3d43);
}

THUMB void TownMenuItemMessage::setItemThrowAffi()
{
    int itemId;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        itemId = status::FukuroItemInfo::getItemId(activeItem, TownMenuPlayerControl::getSingleton()->activeItemPage_);
    } else {
        int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
        itemId = status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(10, 0x40000000, itemId);
    data_020ed1bc.addMessage(0xc3d4c);
    data_020ed1bc.setYesNo();
}

THUMB void TownMenuItemMessage::setItemThrowNG()
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(0xc3d54);
    messageMode_ = MESS_THROW_END;
}

THUMB void TownMenuItemMessage::setItemThrowDifficult()
{
    int itemId;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        itemId = status::FukuroItemInfo::getItemId(activeItem, TownMenuPlayerControl::getSingleton()->activeItemPage_);
    } else {
        int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
        itemId = status::PlayerItemInfo::getItemIndex(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(10, 0x40000000, itemId);
    data_020ed1bc.addMessage(0xc3d51);
    data_020ed1bc.setYesNo();
    messageMode_ = MESS_THROW_OK;
}

THUMB void TownMenuItemMessage::setItemThrow()
{
    messageMode_ = MESS_THROW_END;
    int itemId;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        int activePage = TownMenuPlayerControl::getSingleton()->activeItemPage_;
        itemId = status::FukuroItemInfo::getItemId(activeItem, activePage);
        status::FukuroItemInfo::throwFukuroItem(status::FukuroItemInfo::getIndexToAll(activeItem, activePage));
        TownMenuPlayerControl::getSingleton()->setFukuroActiveItemByChangeMax();
    } else {
        int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
        int activeItem = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
        itemId = status::PlayerItemInfo::getItemIndex(activeChara, activeItem);
        if (status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveEquipment_.isEquipment(itemId) &&
            status::UseItem::isCurse(itemId) && status::g_Party.getPlayerIndex(activeChara) != 0x19) {
            cursedItem_ = itemId;
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            gTownMenuItemSelectCommand.sound_ = 1;
            return;
        }
        status::PlayerItemInfo::throwCharaItem(activeChara, activeItem);
        TownMenuPlayerControl::getSingleton()->setPlayerActiveItemByChangeMax();
    }
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(10, 0x40000000, itemId);
    data_020ed1bc.addMessage(0xc3d4f);
    throwEndToReturnMenu();
}

THUMB void TownMenuItemMessage::endMessageToItemSelect()
{
    close();
    gTownMenuItemSelectCommand.close();
    gUnkTownMenu_02176fa0.open();
}

THUMB void TownMenuItemMessage::endMessageToCommandSelect()
{
    close();
    data_020ed1bc.close();
    gTownMenuItemSelectCommand.resetLock_ = 1;
}

THUMB void TownMenuItemMessage::endMessageToCharaSelect()
{
    close();
    gTownMenuItemSelectCommand.close();
    gTownMenuItemSelectChara.open();
}

THUMB void TownMenuItemMessage::throwEndToReturnMenu()
{
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        if (status::FukuroItemInfo::getItemMaxCount() == 0) {
            TownMenuPlayerControl::getSingleton()->activeItem_ = 0;
            endMessageToCharaSelect();
        } else {
            endMessageToItemSelect();
        }
    } else if (status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->activeChara_) == 0) {
        TownMenuPlayerControl::getSingleton()->activeItem_ = 0;
        endMessageToCharaSelect();
    } else {
        endMessageToItemSelect();
    }
}
