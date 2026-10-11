#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_SHOP_WHOSE::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    mode_ = 3;
    noSort_ = 0;
    yesno_ = 0;
    endMessage_ = 0;
    maxCharaCount_ = status::g_Party.getCount();
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    activeItem_ = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    if (status::g_Party.fukuro_ != 0) {
        maxCharaCount_++;
    }
    navigator_.setupBase();
}

THUMB void MaterielMenu_SHOP_WHOSE::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_ICON32_5x2(&menuItem_, activeChara_, maxCharaCount_);
    MenuTemplate_materiel::MATERIEL_CANCEL(&menuItem2_);
}

THUMB void MaterielMenu_SHOP_WHOSE::menuDraw()
{
    if (data_020ed1bc.isOpen() && yesno_ == 0) {
        unkfunc_0216fc58();
        return;
    }
    if (yesno_ == 0) {
        unkfunc_0216fb6c(0);
        menuItem_.drawActive();
        menuItem2_.drawActive();
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        unkfunc_02080e78();
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            if (endMessage_ == 1) {
                close();
                gMaterielMenu_SHOP_BUYMENU.open();
                gMaterielMenu_SHOP_BUYMENU.message_ = 1;
                return;
            }
            yesno_ = 0;
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            yesno_ = 0;
            data_020ed1bc.close();
            selectNo();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(menuItem2_)) {
        close();
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->cancel());
        data_020ed1bc.addMessageWAITKEY();
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        return;
    }
    navigator_.setup(5, 2, maxCharaCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        activeChara_ = menuItem_.active_;
        int activeChara = activeChara_;
        MaterielMenuPlayerControl::getSingleton()->activeChara_ = activeChara;
        if (result == 2) {
            haveMaxCheck();
        }
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::selectYes()
{
    if (noSort_) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveOther());
        data_020ed1bc.setYesNo();
        mode_ = 1;
        noSort_ = 0;
        yesno_ = 1;
        return;
    }
    switch (mode_) {
    case 0:
        yesSort();
        break;
    case 1:
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveWhose());
        data_020ed1bc.setMessageLastCursor(true);
        yesno_ = 1;
        mode_ = 3;
        break;
    case 2:
        checkMoney();
        break;
    case 3:
        menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
        yesno_ = 0;
        mode_ = -1;
        break;
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::selectNo()
{
    if (noSort_) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveOther());
        data_020ed1bc.setYesNo();
        mode_ = 1;
        noSort_ = 0;
        yesno_ = 1;
        return;
    }
    switch (mode_) {
    case 0:
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveOther());
        menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
        data_020ed1bc.setYesNo();
        yesno_ = 1;
        mode_ = 1;
        break;
    case 1:
        close();
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->cancel());
        data_020ed1bc.addMessageWAITKEY();
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        mode_ = -1;
        break;
    case 2:
        checkMoney();
        break;
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::yesSort()
{
    int sortCount = 0;
    for (int i = 0; i < 12; i++) {
        if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.isEquipment(i) ||
            status::UseItem::isOrder(status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(i)) == false) {
            sortCount++;
        }
    }
    if (sortCount == 12) {
        TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveSomething());
        data_020ed1bc.setMessageLastCursor(true);
        yesno_ = 1;
        noSort_ = 1;
        return;
    }
    status::g_Party.haveItemSack_.sortOutItem(&status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_);
    showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sortEnd());
    data_020ed1bc.setMessageLastCursor(true);
    if (status::UseItem::getItemType(MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem_)) <= 4) {
        close();
        int activeItem = activeItem_;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
        gMaterielMenu_SHOP_EQUIPCHECK.open();
    } else {
        giveBuyItem();
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::haveMaxCheck()
{
    status::HaveItem& itemInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_;
    int target = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
    if (activeChara_ == status::g_Party.getCount()) {
        MaterielMenu_SHOP_MANAGER::getSingleton()->buyItem(activeItem_, activeChara_);
        int mes[2] = {0, 0};
        mode_ = -1;
        data_020ed1bc.openMessageForTALK();
        if (status::g_Party.gold_ == 0) {
            close();
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->buyToSack(false, mes);
            data_020ed1bc.addMessage(mes[0]);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
            data_020ed1bc.addMessageWAITKEY();
            gMaterielMenu_SHOP_ROOT.open();
            gMaterielMenu_SHOP_ROOT.mode_ = 1;
            return;
        }
        MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->buyToSack(true, mes);
        data_020ed1bc.addMessage(mes[0], mes[1]);
        data_020ed1bc.setMessageLastCursor(true);
        for (int i = 0; i < MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount(); i++) {
            MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(i, 1);
            endMessage_ = 1;
        }
        return;
    }
    if (itemInfo.getCount() == 12) {
        TextAPI::setMACRO0(0x12, 0x50000000, target);
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveItemMax());
        data_020ed1bc.setYesNo();
        menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
        yesno_ = 1;
        mode_ = 0;
        return;
    }
    if (status::UseItem::getItemType(MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(activeItem_)) <= 4) {
        close();
        int activeItem = activeItem_;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
        gMaterielMenu_SHOP_EQUIPCHECK.open();
        mode_ = -1;
    } else {
        giveBuyItem();
    }
}

THUMB void MaterielMenu_SHOP_WHOSE::checkMoney()
{
    bool overItem = false;
    bool haveNoMoney = false;
    int mes[3] = {-1, -1, -1};
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->buyItem(activeItem_, activeChara_) == false) {
        overItem = true;
    }
    if (status::g_Party.gold_ == 0) {
        haveNoMoney = true;
    }
    int mesCount = MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->checkMoney(overItem, haveNoMoney, mes);
    data_020ed1bc.openMessageForTALK();
    if (haveNoMoney) {
        if (mesCount == 1) {
            data_020ed1bc.addMessageNOWAIT(mes[0]);
        } else {
            data_020ed1bc.addMessage(mes[0]);
            data_020ed1bc.addMessageNOWAIT(mes[1]);
        }
        data_020ed1bc.addMessageWAITKEY();
        close();
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        return;
    }
    for (int i = 0; mes[i] != -1; i++) {
        data_020ed1bc.addMessage(mes[i]);
    }
    data_020ed1bc.setMessageLastCursor(true);
    for (int i = 0; i < MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount(); i++) {
        MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(i, 1);
    }
    close();
    gMaterielMenu_SHOP_BUYMENU.open();
    gMaterielMenu_SHOP_BUYMENU.message_ = 1;
}

THUMB void MaterielMenu_SHOP_WHOSE::giveBuyItem()
{
    TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_);
    if (status::g_Party.isInsideBasha(activeChara_)) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(true, false));
    } else if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.isDeath()) {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(false, true));
    } else {
        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->getItem(false, false));
    }
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    mode_ = 2;
}

THUMB void MaterielMenu_SHOP_WHOSE::showMessage(int mes)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(mes);
}
