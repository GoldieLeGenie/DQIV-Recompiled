#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/MaterielMenu_SHOP_EXTRA_SELL/MaterielMenu_SHOP_EXTRA_SELL.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"

THUMB void MaterielMenu_SHOP_WHO_SELL::menuSetup()
{
    status::g_Party.setPlayerMode();
    resetCaptureOffset();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    messageCurse_ = 0;
    return_ = 0;
    maxCharaCount_ = status::g_Party.getCount();
    if (status::g_Party.fukuro_ != 0) {
        maxCharaCount_++;
    }
    navigator_.setupBase();
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 2) {
        extraShop_ = 1;
        extraMode_ = 0;
    } else {
        extraShop_ = 0;
        extraMode_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_WHO_SELL::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_ICON32_5x2(&menuItem_, activeChara_, maxCharaCount_);
    MenuTemplate_materiel::MATERIEL_CANCEL(&menuItem2_);
}

THUMB void MaterielMenu_SHOP_WHO_SELL::menuDraw()
{
    if (messageCurse_ != 1 && return_ != 1 && extraMode_ == 1) {
        if (g_TownMenuMessage.isOpen()) {
            drawShopScreen();
            return;
        }
        drawShopWhoSellScreen();
        menuItem_.drawActive();
    }
}

THUMB void MaterielMenu_SHOP_WHO_SELL::menuUpdate()
{
    if (extraShop_) {
        if (extraMessageUpdata()) {
            return;
        }
    } else if (g_TownMenuMessage.isOpen()) {
        if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
            messageCurse_ = 0;
            return_ = 0;
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(menuItem2_)) {
        if (extraShop_) {
            showMessage(0x6d9c, 0x6d9d);
            extraMode_ = 3;
            return;
        }
        close();
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->cancel());
        g_TownMenuMessage.addMessageWAITKEY();
        gMaterielMenu_SHOP_ROOT.open();
        gMaterielMenu_SHOP_ROOT.mode_ = 1;
        return;
    }
    navigator_.setup(5, 2, maxCharaCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        int activeChara = menuItem_.active_;
        activeChara_ = activeChara;
        MaterielMenuPlayerControl::getSingleton()->activeChara_ = activeChara;
        if (result == 2) {
            cancel();
        }
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_WHO_SELL::cancel()
{
    if (extraShop_) {
        if (activeChara_ == status::g_Party.getCount()) {
            if (status::g_Party.haveItemSack_.getCount() == 0) {
                showMessage(0x6d88, -1);
                extraMode_ = 3;
                return;
            }
        } else if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount() == 0) {
            showMessage(0x6d88, -1);
            extraMode_ = 3;
            return;
        }
        showMessage(0x6d8a, -1);
        extraMode_ = 2;
        return;
    }
    int itemCount;
    if (activeChara_ == status::g_Party.getCount()) {
        itemCount = status::g_Party.haveItemSack_.getCount();
    } else {
        itemCount = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount();
    }
    if (itemCount > 0) {
        close();
        if (activeChara_ == status::g_Party.getCount()) {
            gMaterielMenu_SHOP_SELL_SACK.open();
        } else {
            gMaterielMenu_SHOP_SELL_ITEM.open();
        }
        return;
    }
    close();
    int playerIndex = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
    g_TownMenuMessage.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
    int mes[2];
    if (activeChara_ == status::g_Party.getCount()) {
        MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveNoItem(true, mes);
    } else {
        MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->haveNoItem(false, mes);
    }
    g_TownMenuMessage.addMessage(mes[0]);
    g_TownMenuMessage.addMessageNOWAIT(mes[1]);
    g_TownMenuMessage.addMessageWAITKEY();
    gMaterielMenu_SHOP_ROOT.open();
    gMaterielMenu_SHOP_ROOT.mode_ = 1;
}

THUMB bool MaterielMenu_SHOP_WHO_SELL::extraMessageUpdata()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            switch (extraMode_) {
            case 0:
                extraMode_ = 1;
                break;
            case 2:
                close();
                gUnkMaterielMenu_0216b304.open();
                break;
            case 3:
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
                break;
            }
        } else if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            if (extraMode_ == 0) {
                showMessage(0x6d9c, 0x6d9d);
                extraMode_ = 3;
            }
        }
        return true;
    }
    if (extraMode_ == 0) {
        showMessage(0x6d84, 0x6d85);
        g_TownMenuMessage.setYesNo();
    }
    return false;
}

THUMB void MaterielMenu_SHOP_WHO_SELL::showMessage(int mes1, int mes2)
{
    g_TownMenuMessage.openMessageForTALK();
    g_TownMenuMessage.addMessage(mes1);
    if (mes2 != -1) {
        g_TownMenuMessage.addMessage(mes2);
    }
}
