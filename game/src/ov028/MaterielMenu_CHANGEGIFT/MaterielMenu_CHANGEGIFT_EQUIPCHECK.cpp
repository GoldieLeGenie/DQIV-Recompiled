#include "ov028/MaterielMenu_CHANGEGIFT/MaterielMenu_CHANGEGIFT.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::menuSetup()
{
    status::g_Party.setPlayerMode();
    equipmode_ = 0;
    yesnoFlag_ = 0;
    fukuro_ = 0;
    playSound_ = 0;
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    if (activeChara_ == status::g_Party.getCount()) {
        fukuro_ = 1;
    }
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    itemID_ = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(control->activeItem_);
    MenuSoundManager::getSingleton()->initialize();
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::menuDraw()
{
    unkfunc_0216fce8();
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::menuUpdate()
{
    if (MenuSoundManager::getSingleton()->isPlaySound() == false && playSound_ == 1) {
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(activeChara_);
        ItemType equipType = status::UseItem::getItemType(itemID_);
        int equipItem = player->haveStatusInfo_.haveEquipment_.getEquipment(equipType);
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(7, 0x40000000, equipItem);
        data_020ed1bc.addMessage(0xc8b15);
        equipmode_ = 4;
        yesnoFlag_ = 0;
        playSound_ = 0;
    }
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (yesnoFlag_ == 0) {
                setMessage();
            } else {
                equipYesMessage();
            }
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (yesnoFlag_ == 0) {
                setMessage();
            } else if (equipmode_ == 2) {
                aliveCheck();
            } else {
                equipNoMessage();
            }
        }
    } else if (equipmode_ == 0) {
        equipCheck();
    }
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::equipCheck()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, statusInfo.haveStatus_.playerIndex_);
    if (fukuro_) {
        getGift();
        TextAPI::setMACRO0(0xa, 0x40000000, itemID_);
        data_020ed1bc.addMessage(0xc8b06);
        close();
        gMaterielMenu_CHANGEGIFT_ROOT.open();
        gMaterielMenu_CHANGEGIFT_ROOT.mode_ = 1;
        return;
    }
    if (statusInfo.haveItem_.getCount() == 12) {
        data_020ed1bc.addMessage(0xc8b09);
        equipmode_ = 1;
        return;
    }
    if (status::UseItem::getItemType(itemID_) > 4) {
        aliveCheck();
        return;
    }
    if (statusInfo.isEquipEnable(itemID_) == false) {
        data_020ed1bc.addMessage(0xc8b0d);
        data_020ed1bc.setYesNo();
        equipmode_ = 3;
        yesnoFlag_ = 1;
        return;
    }
    data_020ed1bc.addMessage(0xc8b24);
    data_020ed1bc.setYesNo();
    equipmode_ = 2;
    yesnoFlag_ = 1;
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::setMessage()
{
    data_020ed1bc.openMessageForTALK();
    switch (equipmode_) {
    case 1:
        TextAPI::setMACRO0(0xa, 0x40000000, itemID_);
        data_020ed1bc.addMessage(0xc8b0a);
        break;
    case 4:
        TextAPI::setMACRO0(0xa, 0x40000000, itemID_);
        data_020ed1bc.addMessage(0xc8b16);
        break;
    }
    data_020ed1bc.setYesNo();
    yesnoFlag_ = 1;
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::equipYesMessage()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    int equipItem = 0;
    ItemType equipType = status::UseItem::getItemType(itemID_);
    if (equipType <= 4) {
        int eqID = statusInfo.haveEquipment_.getEquipment(equipType);
        equipItem = statusInfo.haveEquipment_.getEquipment(equipType);
    }
    switch (equipmode_) {
    case 1:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0xc8b03);
        close();
        gMaterielMenu_CHANGEGIFT_SELECTCHARA.open();
        break;
    case 2:
        if (equipItem != 0 && status::UseItem::isCurse(equipItem) && statusInfo.haveStatus_.playerIndex_ != 0x19) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            playSound_ = 1;
            break;
        }
        getGift();
        statusInfo.setEquipment(statusInfo.haveItem_.getCount() - 1);
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0x12, 0x50000000, statusInfo.haveStatus_.playerIndex_);
        TextAPI::setMACRO0(0xa, 0x40000000, itemID_);
        data_020ed1bc.addMessage(0xc8b12);
        close();
        gMaterielMenu_CHANGEGIFT_ROOT.open();
        gMaterielMenu_CHANGEGIFT_ROOT.mode_ = 1;
        break;
    case 3:
    case 4:
        aliveCheck();
        break;
    }
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::equipNoMessage()
{
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(MaterielMenuPlayerControl::getSingleton()->leadpc_);
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0xb, 0x50000000, player->haveStatusInfo_.haveStatus_.playerIndex_);
    TextAPI::setMACRO0(0x2a, 0xf0000000, status::g_Party.casinoCoin_);
    data_020ed1bc.addMessage(0xc8afa);
    data_020ed1bc.setYesNo();
    close();
    gMaterielMenu_CHANGEGIFT_ROOT.open();
    gMaterielMenu_CHANGEGIFT_ROOT.mode_ = 3;
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::aliveCheck()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    getGift();
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, statusInfo.haveStatus_.playerIndex_);
    if (statusInfo.isDeath() == false) {
        data_020ed1bc.addMessage(0xc8b1b);
    } else {
        data_020ed1bc.addMessage(0xc8b1e);
    }
    close();
    gMaterielMenu_CHANGEGIFT_ROOT.open();
    gMaterielMenu_CHANGEGIFT_ROOT.mode_ = 1;
}

THUMB void MaterielMenu_CHANGEGIFT_EQUIPCHECK::getGift()
{
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    int itemPrice = MaterielMenu_SHOP_MANAGER::getSingleton()->getItemPrice(control->activeItem_);
    if (fukuro_) {
        status::g_Party.haveItemSack_.add(itemID_);
    } else {
        status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.add(itemID_);
    }
    status::g_Party.setCasinoCoin(status::g_Party.casinoCoin_ - itemPrice);
}

ARM void MaterielMenu_CHANGEGIFT_EQUIPCHECK::menuExecute()
{
}
