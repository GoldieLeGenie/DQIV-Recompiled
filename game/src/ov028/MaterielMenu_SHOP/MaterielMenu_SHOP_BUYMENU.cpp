#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/UseItem.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"

#ifdef DQIV_NATIVE
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"
#include "main/dss/Pad.hpp"
#include <nitro/pad.h>

void MaterielMenu_SHOP_BUYMENU::buildRows()
{
    MaterielMenu_SHOP_MANAGER* manager = MaterielMenu_SHOP_MANAGER::getSingleton();
    const int count = manager->getSellItemCount();
    if (manager->getExtraShop() == 1) {
        filter_ = SHOP_FILTER_ALL;
    }
    rowCount_ = 0;
    for (int i = 0; i < count && rowCount_ < 6; i++) {
        if (filter_ == SHOP_FILTER_ALL || unkfunc_native_shopFilterMatch(filter_, manager->getItem(i))) {
            rows_[rowCount_++] = i;
        }
    }
}

void MaterielMenu_SHOP_BUYMENU::buildTabs()
{
    // All, then the categories the shop sells; one tab only when it sells one.
    MaterielMenu_SHOP_MANAGER* manager = MaterielMenu_SHOP_MANAGER::getSingleton();
    tabCount_ = 0;
    tabs_[tabCount_++] = SHOP_FILTER_ALL;
    if (manager->getExtraShop() == 1) {
        return;
    }
    for (int filter = SHOP_FILTER_WEAPON; filter < SHOP_FILTER_COUNT; filter++) {
        for (int i = 0; i < manager->getSellItemCount(); i++) {
            if (unkfunc_native_shopFilterMatch(filter, manager->getItem(i))) {
                tabs_[tabCount_++] = filter;
                break;
            }
        }
    }
    if (tabCount_ == 2) {
        tabs_[0] = tabs_[1];
        tabCount_ = 1;
    }
}

int MaterielMenu_SHOP_BUYMENU::rowOf(int item) const
{
    for (int k = 0; k < rowCount_; k++) {
        if (rows_[k] == item) {
            return k;
        }
    }
    return 0;
}
#endif

THUMB void MaterielMenu_SHOP_BUYMENU::menuSetup()
{
    status::g_Party.setPlayerMode();
    unkfunc_02080e64(-4, 0);
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    navigator_.setupBase();
    activeItem_ = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    message_ = 0;
    for (int i = 0; i < MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount(); i++) {
        int itemID;
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
            if (i == 0) {
                itemID = 0x6f;
            } else {
                itemID = 7;
            }
        } else {
            itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(i);
        }
        for (int j = 0; j < status::g_Party.haveItemSack_.getCount(); j++) {
            if (itemID == status::g_Party.haveItemSack_.getItem(j)) {
                fukuroItemCount_[i] = status::g_Party.haveItemSack_.getItemCount(j);
                break;
            }
            fukuroItemCount_[i] = 0;
        }
    }
#ifdef DQIV_NATIVE
    buildTabs();
    if (fresh_) {
        // Coming from the shop's first menu: the first tab.
        fresh_ = 0;
        tab_ = 0;
    }
    if (tab_ >= tabCount_) {
        tab_ = 0;
    }
    filter_ = tabs_[tab_];
    buildRows();
    if (rowCount_ == 0 || rows_[rowOf(activeItem_)] != activeItem_) {
        activeItem_ = rows_[0];
    }
#endif
}

THUMB void MaterielMenu_SHOP_BUYMENU::menuExecute()
{
#ifdef DQIV_NATIVE
    MenuTemplate_materiel::MATERIEL_CELECT_BUY_TABS(&menuItem_, rowOf(activeItem_), rowCount_);
    MenuTemplate_materiel::MATERIEL_CANCEL(&menuItem2_);
    return;
#endif
    MenuTemplate_materiel::MATERIEL_CELECT_BUY(&menuItem_, activeItem_, MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount());
    MenuTemplate_materiel::MATERIEL_CANCEL(&menuItem2_);
}

THUMB void MaterielMenu_SHOP_BUYMENU::menuDraw()
{
    if (data_020ed1bc.isOpen()) {
        drawShopScreen();
        return;
    }
#ifdef DQIV_NATIVE
    {
        // As drawShopBuyScreen, with the category tabs above the list and icons in it.
        unkfunc_native_shopTabs(tabs_, tabCount_, tab_);
        unkfunc_native_shopBuyList(rows_, rowCount_);
        drawShopItemKind();
        drawMoneyWindow(0x88, 0xa0, 0);
        MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
        const int item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(ctrl->activeItem_);
        drawShopSackCount(fukuroItemCount_);
        if (status::UseItem::getItemType(item) <= 4) {
            drawShopEquipCompare();
        } else {
            drawShopHeldCounts();
        }
        menuItem_.drawActive();
        return;
    }
#endif
    drawShopBuyScreen(fukuroItemCount_, 0);
    menuItem_.drawActive();
}

THUMB void MaterielMenu_SHOP_BUYMENU::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
            if (message_) {
                menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
                message_ = 0;
            }
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
#ifdef DQIV_NATIVE
    if (tabCount_ > 1 && (dss::g_Pad.edge() & (PAD_BUTTON_L | PAD_BUTTON_R))) {
        // L / R: the previous / next tab.
        tab_ = (tab_ + ((dss::g_Pad.edge() & PAD_BUTTON_R) ? 1 : tabCount_ - 1)) % tabCount_;
        filter_ = tabs_[tab_];
        buildRows();
        activeItem_ = rows_[0];
        menuItem_.active_ = 0;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem_;
        redraw_ = 1;
        return;
    }
    navigator_.setup(1, 6, rowCount_);
    {
        const int active = rows_[menuItem_.active_];
        const int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            activeItem_ = rows_[menuItem_.active_];
            if (result == 2) {
                checkBuy();
            } else if (result == 6) {
                changeQuantity(false);
            } else if (result == 7) {
                changeQuantity(true);
            } else {
                MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(active, 1);
            }
            activeItem_ = rows_[menuItem_.active_];
            MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem_;
            redraw_ = 1;
        }
        return;
    }
#endif
    navigator_.setup(1, 6, MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount());
    int active = menuItem_.active_;
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            checkBuy();
        } else if (result == 6) {
            changeQuantity(false);
        } else if (result == 7) {
            changeQuantity(true);
        } else {
            MaterielMenu_SHOP_MANAGER::getSingleton()->setItemQuantity(active, 1);
        }
        activeItem_ = menuItem_.active_;
        int activeItem = activeItem_;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_BUYMENU::checkBuy()
{
    MaterielMenu_SHOP_MANAGER* manager = MaterielMenu_SHOP_MANAGER::getSingleton();
    data_020ed1bc.openMessageForTALK();
    unsigned int gold = status::g_Party.gold_;
    if (gold < manager->getItemPriceSum(activeItem_)) {
        int mes[2];
        MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->noMoney(mes);
        data_020ed1bc.addMessage(mes[0]);
        data_020ed1bc.addMessageNOWAIT(mes[1]);
        data_020ed1bc.addMessageWAITKEY();
        close();
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        return;
    }
    bool plural = false;
    bool battleUse = false;
    int mes[3] = {0, 0, 0};
    if (manager->getItemQuantity(activeItem_) > 1) {
        plural = true;
    }
    if (status::UseItem::isUsuallyUse(manager->getItem(activeItem_))) {
        battleUse = true;
    }
    MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->buyItem(plural, battleUse, mes);
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
        if (menuItem_.active_ == 0) {
            TextAPI::setMACRO0(0xa, 0x40000000, 0x6f);
        } else {
            TextAPI::setMACRO0(0xa, 0x40000000, 7);
        }
    } else {
        TextAPI::setMACRO0(0xa, 0x40000000, manager->getItem(activeItem_));
    }
    TextAPI::setMACRO0(0x54, 0xf0000000, manager->getItemQuantity(activeItem_));
    if (mes[2] == 0) {
        data_020ed1bc.addMessage(mes[0], mes[1]);
    } else {
        data_020ed1bc.addMessage(mes[0], mes[1], mes[2]);
    }
    data_020ed1bc.setMessageLastCursor(true);
    close();
    int activeItem = activeItem_;
    MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
    gMaterielMenu_SHOP_WHOSE.open();
}

THUMB void MaterielMenu_SHOP_BUYMENU::changeQuantity(bool add)
{
    if (add) {
        MaterielMenu_SHOP_MANAGER::getSingleton()->addItem(activeItem_);
    } else {
        MaterielMenu_SHOP_MANAGER::getSingleton()->subItem(activeItem_);
    }
}
