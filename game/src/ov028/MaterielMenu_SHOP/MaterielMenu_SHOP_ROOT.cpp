#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"

THUMB void MaterielMenu_SHOP_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = MaterielMenu_SHOP_MANAGER::getSingleton()->getShopAction();
    navigator_.setupBase();
    MaterielMenuPlayerControl::getSingleton()->allClear();
    MaterielMenu_SHOP_MANAGER::getSingleton()->resetItemQuantity();
    mode_ = 0;
}

THUMB void MaterielMenu_SHOP_ROOT::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_SHOP_ROOT(&menuItem_, menuItem_.active_);
}

THUMB void MaterielMenu_SHOP_ROOT::menuDraw()
{
    if (g_TownMenuMessage.isMessageWAITPROG()) {
        drawShopRootScreen();
        menuItem_.drawActive();
    } else if (g_TownMenuMessage.isOpen() && mode_ != 1) {
        drawShopScreen();
    }
}

THUMB void MaterielMenu_SHOP_ROOT::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.isMessageWAITPROG()) {
            if (mode_ == 1) {
                redraw_ = 1;
                mode_ = 2;
            }
            navigator_.setup(1, 3, 3);
            int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
            if (result != 0) {
                if (result == 2) {
                    g_TownMenuMessage.clearMessageWAITPROG();
                    switch (menuItem_.active_) {
                    case 0:
                        MaterielMenu_SHOP_MANAGER::getSingleton()->setShopAction(0);
                        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->buy());
                        g_TownMenuMessage.setMessageLastCursor(true);
                        mode_ = 2;
                        break;
                    case 1:
                        MaterielMenu_SHOP_MANAGER::getSingleton()->setShopAction(1);
                        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sell());
                        g_TownMenuMessage.setMessageLastCursor(true);
                        mode_ = 2;
                        break;
                    case 2:
                        MaterielMenu_SHOP_MANAGER::getSingleton()->setShopAction(2);
                        showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->yameru());
                        mode_ = 3;
                        break;
                    }
                } else if (result == 3) {
                    g_TownMenuMessage.clearMessageWAITPROG();
                    MaterielMenu_SHOP_MANAGER::getSingleton()->setShopAction(menuItem_.active_);
                    showMessage(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->yameru());
                    mode_ = 3;
                }
                redraw_ = 1;
                return;
            }
        }
        if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
            execConduct();
            return;
        }
    }
    if (mode_ == 0) {
        resetCaptureOffset();
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->idle());
        g_TownMenuMessage.addMessageWAITKEY();
        mode_ = 1;
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SHOP_ROOT::execConduct()
{
    switch (mode_) {
    case 2:
        close();
        if (menuItem_.active_ == 0) {
#ifdef DQIV_NATIVE
            gMaterielMenu_SHOP_BUYMENU.fresh_ = 1;
#endif
            gMaterielMenu_SHOP_BUYMENU.open();
        } else {
            gMaterielMenu_SHOP_WHO_SELL.open();
        }
        break;
    case 3:
        resetCaptureOffset();
        MaterielMenu_SHOP_MANAGER::getSingleton()->allClear();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_SHOP_ROOT::showMessage(int mes)
{
    g_TownMenuMessage.openMessageForTALK();
    g_TownMenuMessage.addMessage(mes);
}
