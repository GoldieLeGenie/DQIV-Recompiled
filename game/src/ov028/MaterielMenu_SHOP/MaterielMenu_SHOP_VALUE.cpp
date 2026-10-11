#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/sound/MenuSoundManager.hpp"

THUMB void MaterielMenu_SHOP_VALUE::menuSetup()
{
    status::g_Party.setPlayerMode();
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    activeItem_ = control->activeItem_ + control->activeItemPage_ * 6;
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    MenuSoundManager::getSingleton()->initialize();
    int iname;
    if (activeChara_ == status::g_Party.getCount()) {
        iname = status::g_Party.haveItemSack_.getItem(activeItem_);
    } else {
        iname = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_);
    }
    int price = status::UseItem::getSellPrice(iname) * MaterielMenu_SHOP_MANAGER::getSingleton()->getSellQuantity();
    int xunit = MaterielMenu_SHOP_MANAGER::getSingleton()->getSellQuantity();
    switch (sellType_) {
    case 0:
        TextAPI::setMACRO0(0xa, 0x40000000, iname);
        TextAPI::setMACRO0(0x3e, 0xf0000000, price);
        if (xunit != 1) {
            TextAPI::setMACRO0(0x54, 0xf0000000, xunit);
            showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellPluralSellOK(), -1, -1);
        } else {
            TextAPI::setMACRO0(0x54, 0xf0000000, xunit);
            showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellOK(), -1, -1);
        }
        data_020ed1bc.setYesNo();
        break;
    case 1: {
        int mes[3] = {-1, -1, -1};
        if (checkItemMoney() == false) {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellNG(true, mes);
            if (mes[2] == -1) {
                showMessage(mes[0], -1, -1);
                data_020ed1bc.addMessageNOWAIT(mes[1]);
            } else {
                showMessage(mes[0], mes[1], -1);
                data_020ed1bc.addMessageNOWAIT(mes[2]);
            }
            data_020ed1bc.addMessageWAITKEY();
            close();
            gMaterielMenu_SHOP_ROOT.open();
            gMaterielMenu_SHOP_ROOT.mode_ = 1;
        } else {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellNG(false, mes);
            showMessage(mes[0], mes[1], mes[2]);
            data_020ed1bc.setMessageLastCursor(true);
            close();
            gMaterielMenu_SHOP_WHO_SELL.open();
        }
        break;
    }
    case 2:
        TextAPI::setMACRO0(0xa, 0x40000000, iname);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellDifficult(), -1, -1);
        data_020ed1bc.setYesNo();
        break;
    }
}

THUMB void MaterielMenu_SHOP_VALUE::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            selectNo();
        }
        return;
    }
    if (MenuSoundManager::getSingleton()->isPlaySound() == false) {
        int itemID;
        if (activeChara_ == status::g_Party.getCount()) {
            itemID = status::g_Party.haveItemSack_.getItem(activeItem_);
        } else {
            itemID = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_);
        }
        TextAPI::setMACRO0(7, 0x40000000, itemID);
        int mes[2];
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->sellOK(activeChara_) == false) {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellCurse(true, mes);
            showMessage(mes[0], -1, -1);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
            data_020ed1bc.addMessageWAITKEY();
            close();
            gMaterielMenu_SHOP_ROOT.open();
            gMaterielMenu_SHOP_ROOT.mode_ = 1;
        } else {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellCurse(false, mes);
            showMessage(mes[0], mes[1], -1);
            close();
            gMaterielMenu_SHOP_WHO_SELL.open();
            gMaterielMenu_SHOP_WHO_SELL.messageCurse_ = 1;
        }
    }
}

THUMB void MaterielMenu_SHOP_VALUE::selectYes()
{
    TextAPI::setMACRO0(0x54, 0xf0000000, MaterielMenu_SHOP_MANAGER::getSingleton()->getSellQuantity());
    int iname;
    (activeChara_ == status::g_Party.getCount())
        ? (iname = status::g_Party.haveItemSack_.getItem(activeItem_))
        : (iname = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_));
    int price = status::UseItem::getSellPrice(iname) * MaterielMenu_SHOP_MANAGER::getSingleton()->getSellQuantity();
    switch (sellType_) {
    case 0:
        checkHaveMoney();
        break;
    case 1:
        break;
    case 2:
        TextAPI::setMACRO0(0xa, 0x40000000, iname);
        TextAPI::setMACRO0(0x3e, 0xf0000000, price);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellOK(), -1, -1);
        data_020ed1bc.setYesNo();
        sellType_ = 0;
        break;
    }
}

THUMB void MaterielMenu_SHOP_VALUE::selectNo()
{
    int mes[2];
    switch (sellType_) {
    case 0:
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->sellOK(activeChara_) == false) {
            close();
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->celectNo(true, mes);
            showMessage(mes[0], -1, -1);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
            gMaterielMenu_SHOP_ROOT.open();
            gMaterielMenu_SHOP_ROOT.mode_ = 1;
            data_020ed1bc.addMessageWAITKEY();
        } else {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->celectNo(false, mes);
            showMessage(mes[0], mes[1], -1);
            data_020ed1bc.setMessageLastCursor(true);
            close();
            gMaterielMenu_SHOP_WHO_SELL.open();
            gMaterielMenu_SHOP_WHO_SELL.return_ = 1;
        }
        break;
    case 1:
        break;
    case 2:
        close();
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->cancel());
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        data_020ed1bc.addMessageWAITKEY();
        break;
    }
}

THUMB void MaterielMenu_SHOP_VALUE::checkHaveMoney()
{
    status::HaveItem& itemInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_;
    if (activeChara_ < status::g_Party.getCount()) {
        int itemID = itemInfo.getItem(activeItem_);
        if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveEquipment_.isEquipment(itemID) &&
            status::UseItem::isCurse(itemID) && status::g_Party.getPlayerIndex(activeChara_) != 0x19) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            return;
        }
    }
    int mes[2];
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->sellItem(activeItem_, activeChara_, -1)) {
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->sellOK(activeChara_) == false) {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellEnd(true, mes);
            showMessage(mes[0], -1, -1);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
            close();
            gMaterielMenu_SHOP_ROOT.open();
            gMaterielMenu_SHOP_ROOT.mode_ = 1;
            data_020ed1bc.addMessageWAITKEY();
        } else {
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellEnd(false, mes);
            showMessage(mes[0], mes[1], -1);
            data_020ed1bc.setMessageLastCursor(true);
            close();
            gMaterielMenu_SHOP_WHO_SELL.open();
        }
        return;
    }
    int playerMax = status::g_Party.getCount();
    int target;
    if (activeChara_ == playerMax) {
        int count = 0;
        while (status::g_Party.getPlayerStatus(count)->haveStatusInfo_.isDeath()) {
            count++;
            if (count > playerMax) {
                count = 0;
                break;
            }
        }
        target = status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.playerIndex_;
    } else {
        target = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
    }
    TextAPI::setMACRO0(0x12, 0x50000000, target);
    MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->overMoney(mes);
    showMessage(mes[0], mes[1], -1);
    data_020ed1bc.addMessageNOWAIT(mes[2]);
    data_020ed1bc.addMessageWAITKEY();
    close();
    gMaterielMenu_SHOP_ROOT.open();
    gMaterielMenu_SHOP_ROOT.mode_ = 1;
}

THUMB bool MaterielMenu_SHOP_VALUE::checkItemMoney()
{
    if (status::g_Party.gold_ == 999999) {
        return false;
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() != 0) {
            return true;
        }
    }
    if (status::g_Party.fukuro_ != 0 && status::g_Party.haveItemSack_.getCount() != 0) {
        return true;
    }
    return false;
}

THUMB void MaterielMenu_SHOP_VALUE::showMessage(int mes1, int mes2, int mes3)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(mes1);
    if (mes2 != -1) {
        data_020ed1bc.addMessage(mes2);
    }
    if (mes3 != -1) {
        data_020ed1bc.addMessage(mes3);
    }
}

ARM void MaterielMenu_SHOP_VALUE::menuDraw()
{
}

ARM void MaterielMenu_SHOP_VALUE::menuExecute()
{
}
