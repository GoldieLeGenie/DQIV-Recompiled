#include "ov016/TownMenu_ITEM/TownMenu_ITEM_EQUIPCHECK.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetItem.hpp"
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

THUMB void TownMenu_ITEM_EQUIPCHECK::menuSetup()
{
    status::g_Party.setPlayerMode();
    cursedMessage_ = 0;
    target_ = TownMenuPlayerControl::getSingleton()->targetChara_;
    if (TownMenuPlayerControl::getSingleton()->targetItem_ == -1) {
        targetItem_ = (char)(status::PlayerItemInfo::getItemMaxCount(target_) - 1);
    } else {
        targetItem_ = TownMenuPlayerControl::getSingleton()->getTargetItemIndexToAll();
    }
    itemID_ = status::PlayerItemInfo::getItemIndex(target_, targetItem_);
    MenuSoundManager::getSingleton()->initialize();
    mode_ = 0;
}

THUMB void TownMenu_ITEM_EQUIPCHECK::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            equipItem();
            return;
        }
        if (stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            close();
            openItemRoot();
        }
        return;
    }
    if (mode_ == 2) {
        openItemRoot();
        return;
    }
    if (!MenuSoundManager::getSingleton()->isPlaySound()) {
        ItemType type = status::UseItem::getItemType(itemID_);
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(10, 0x40000000, itemID_);
        TextAPI::setMACRO0(7, 0x40000000, status::PlayerItemInfo::getEquipItemIdByType(target_, type));
        data_020ed1bc.addMessage(cursedMessage_);
        mode_ = 2;
    }
}

THUMB void TownMenu_ITEM_EQUIPCHECK::equipItem()
{
    ItemType equipType = status::UseItem::getItemType(itemID_);
    status::HaveStatusInfo& toStatusInfo = status::g_Party.getPlayerStatus(target_)->haveStatusInfo_;
    switch (mode_) {
    case 0:
        if (toStatusInfo.isEquipEnable(itemID_)) {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessage(0xc3dc9);
            data_020ed1bc.setYesNo();
            mode_ = 1;
        } else {
            openItemRoot();
        }
        break;
    case 1:
        if (equipType <= ITEM_ACCESSORY) {
            int eqID = status::PlayerItemInfo::getEquipItemIdByType(target_, equipType);
            if (eqID != 0 && status::UseItem::isCurse(eqID) && status::g_Party.getPlayerIndex(target_) != 0x19) {
                cursedMessage_ = 0xc3d8d;
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
                gTownMenuItemSelectTargetItem.sound_ = 1;
                return;
            }
            if (status::UseItem::isCurse(itemID_) && status::g_Party.getPlayerIndex(target_) != 0x19) {
                cursedMessage_ = 0xc3a53;
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
                gTownMenuItemSelectTargetItem.sound_ = 1;
            } else if (toStatusInfo.isDeath()) {
                int count = 0;
                while (status::g_Party.getPlayerStatus(count)->haveStatusInfo_.isDeath() == 1) {
                    count++;
                    if (count > status::g_Party.getCount()) {
                        count = 0;
                        break;
                    }
                }
                data_020ed1bc.openMessageForMENU();
                TextAPI::setMACRO0(12, 0x50000000, status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.playerIndex_);
                TextAPI::setMACRO0(1, 0x50000000, toStatusInfo.haveStatus_.playerIndex_);
                TextAPI::setMACRO0(10, 0x40000000, itemID_);
                data_020ed1bc.addMessage(0xc3d45);
                mode_ = 2;
            } else {
                data_020ed1bc.openMessageForMENU();
                TextAPI::setMACRO0(1, 0x50000000, toStatusInfo.haveStatus_.playerIndex_);
                TextAPI::setMACRO0(10, 0x40000000, itemID_);
                data_020ed1bc.addMessage(0xc3d43);
                mode_ = 2;
            }
            toStatusInfo.setEquipment(targetItem_);
        }
        break;
    case 2:
        openItemRoot();
        break;
    }
}

THUMB void TownMenu_ITEM_EQUIPCHECK::openItemRoot()
{
    close();
    gTownMenuItemSelectTargetChara.close();
    gTownMenuItemSelectTargetItem.close();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        if (status::FukuroItemInfo::getItemMaxCount() == 0) {
            gTownMenuItemSelectChara.open();
        } else {
            gUnkTownMenu_02176fa0.open();
        }
    } else if (status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->activeChara_) == 0) {
        gTownMenuItemSelectChara.open();
    } else {
        gUnkTownMenu_02176fa0.open();
    }
}
