#pragma ipa file
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"

THUMB void MaterielMenu_SHOP_EQUIPCHECK::menuSetup()
{
    status::g_Party.setPlayerMode();
    mode_ = 0;
    haveItemOver_ = 0;
    ctrlID_ = g_cmnPartyInfo.ctrlID_;
    MenuSoundManager::getSingleton()->initialize();
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::menuExecute()
{
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::menuDraw()
{
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(ctrlID_));
            yesAdmin();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(ctrlID_));
            noAdmin();
        }
        return;
    }
    if (mode_ == 0) {
        messageSetup();
        return;
    }
    if (MenuSoundManager::getSingleton()->isPlaySound() == false) {
        int activeItem = MaterielMenuPlayerControl::getSingleton()->activeItem_;
        int activeChara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
        ItemType equipType = status::UseItem::getItemType(MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem));
        int eqID = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveEquipment_.getEquipment(equipType);
        TextAPI::setMACRO0(7, 0x40000000, eqID);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->equipCurseItem(true));
        data_020ed1bc.setMessageLastCursor(true);
        mode_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::yesAdmin()
{
    int mes[3] = {-1, -1, -1};
    bool haveNoMoney = false;
    int activeItem = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    int activeChara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    ItemType equipType = status::UseItem::getItemType(MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem));
    int eqID = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveEquipment_.getEquipment(equipType);
    switch (mode_) {
    case 0:
        messageSetup();
        break;
    case 1:
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->equipCurseItem(false));
        mode_ = 3;
        data_020ed1bc.setYesNo();
        break;
    case 2: {
        if (eqID != 0 && status::UseItem::isCurse(eqID) && status::g_Party.getPlayerIndex(activeChara) != 0x19) {
            MenuSoundManager::getSingleton()->setPlaySound((MenuSoundManager::MENU_SOUND)1);
            break;
        }
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->buyItem(activeItem, activeChara) == false) {
            haveItemOver_ = 1;
        }
        int itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem);
        int itemIndex;
        int playerIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveStatus_.playerIndex_;
        itemIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.getCount() - 1;
        status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.setEquipment(itemIndex);
        TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
        TextAPI::setMACRO0(0xa, 0x40000000, itemID);
        ui_MsgSndSet(0x30);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->equipItem());
        data_020ed1bc.setMessageLastCursor(true);
        mode_ = 5;
        break;
    }
    case 3:
        giveItem();
        break;
    case 5: {
        if (status::g_Party.gold_ == 0) {
            haveNoMoney = true;
        }
        int mesCount = MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->checkMoney(haveItemOver_, haveNoMoney, mes);
        checkMoneyMessage(mes, mesCount, haveNoMoney);
        if (status::g_Party.gold_ == 0) {
            rerurnRoot();
        } else {
            mode_ = 6;
        }
        break;
    }
    case 6:
        for (int i = 0; i < MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount(); i++) {
            MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(i, 1);
        }
        if (status::g_Party.gold_ == 0) {
            rerurnRoot();
        } else {
            close();
            data_020ed1bc.setMessageLastCursor(true);
            gMaterielMenu_SHOP_BUYMENU.open();
        }
        break;
    }
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::noAdmin()
{
    int activeChara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    int activeItem = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    int mes[3] = {-1, -1, -1};
    switch (mode_) {
    case 0:
        messageSetup();
        break;
    case 1:
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->equipCurseItem(false));
        mode_ = 3;
        data_020ed1bc.setYesNo();
        break;
    case 2:
        giveItem();
        break;
    case 3: {
        int mesCount = MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->checkMoney(haveItemOver_, false, mes);
        checkMoneyMessage(mes, mesCount, false);
        ItemType equipType = status::UseItem::getItemType(MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem));
        int eqID = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveEquipment_.getEquipment(equipType);
        if (status::UseItem::isCurse(eqID) && status::g_Party.getPlayerIndex(activeChara) != 0x19) {
            mode_ = 6;
        } else {
            rerurnRoot();
        }
        break;
    }
    case 4: {
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->buyItem(activeItem, activeChara) == false) {
            haveItemOver_ = 1;
        }
        int itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem);
        int itemIndex;
        int playerIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveStatus_.playerIndex_;
        itemIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.getCount() - 1;
        status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.setEquipment(itemIndex);
        TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
        TextAPI::setMACRO0(0xa, 0x40000000, itemID);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->equipItem());
        close();
        gMaterielMenu_SHOP_WHOSE.open();
        gMaterielMenu_SHOP_WHOSE.mode_ = 4;
        break;
    }
    case 6:
        for (int i = 0; i < MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount(); i++) {
            MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(i, 1);
        }
        close();
        gMaterielMenu_SHOP_BUYMENU.open();
        break;
    }
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::giveItem()
{
    int activeChara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    int activeItem = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    int playerIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveStatus_.playerIndex_;
    data_020ed1bc.close();
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->buyItem(activeItem, activeChara) == false) {
        haveItemOver_ = 1;
    }
    TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
    if (status::g_Party.isInsideBasha(activeChara)) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(true, false));
    } else if (status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.isDeath()) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(false, true));
    } else {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(false, false));
        data_020ed1bc.setMessageLastCursor(true);
    }
    mode_ = 5;
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::messageSetup()
{
    int activeChara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    int itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(control->activeItem_);
    int playerIndex = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveStatus_.playerIndex_;
    bool equip = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.isEquipEnable(itemID);
    mode_ = equip == false ? 3 : 2;
    TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
    showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->checkEquip(equip));
    data_020ed1bc.setYesNo();
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::showMessage(int mes)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(mes);
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::rerurnRoot()
{
    data_020ed1bc.addMessageWAITKEY();
    close();
    gMaterielMenu_SHOP_ROOT.open();
    gMaterielMenu_SHOP_ROOT.mode_ = 1;
}

THUMB void MaterielMenu_SHOP_EQUIPCHECK::checkMoneyMessage(int* mes, int mesCount, bool haveNoMoney)
{
    data_020ed1bc.openMessageForTALK();
    if (haveNoMoney) {
        if (mesCount == 1) {
            data_020ed1bc.addMessageNOWAIT(mes[0]);
        } else {
            data_020ed1bc.addMessage(mes[0]);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
        }
        data_020ed1bc.addMessageWAITKEY();
    } else {
        for (int count = 0; mes[count] != -1; count++) {
            data_020ed1bc.addMessage(mes[count]);
        }
        data_020ed1bc.setMessageLastCursor(true);
    }
}
