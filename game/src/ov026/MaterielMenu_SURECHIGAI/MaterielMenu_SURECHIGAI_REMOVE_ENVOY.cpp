#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"

THUMB void MaterielMenu_SURECHIGAI_REMOVE_ENVOY::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    mode_ = 0;
    envoyCount_ = g_EnvoyManager.getEnvoyCount();
    navigator_.setup(2, 4, envoyCount_);
}

THUMB void MaterielMenu_SURECHIGAI_REMOVE_ENVOY::menuExecute()
{
    int count = envoyCount_ - navigator_.getPageNo() * 8;
    MenuTemplate_materiel::suretigaiSelectChiaus(&menuItem_, menuItem_.active_, count);
}

THUMB void MaterielMenu_SURECHIGAI_REMOVE_ENVOY::menuDraw()
{
    if (g_TownMenuMessage.isOpen() != 1) {
        if (mode_ <= 1) {
            int page = navigator_.getPageNo();
            int pageMax = navigator_.getPageMaxCount();
            drawTagModeTenantListScreen(mode_, envoyCount_, page, pageMax, navigator_.getIndex(menuItem_.active_));
        }
        if (mode_ == 0) {
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_SURECHIGAI_REMOVE_ENVOY::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (mode_ == 1) {
                g_EnvoyManager.selectEnvoy(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = g_EnvoyManager.getName();
                g_TownMenuMessage.openMessageForTALK();
                TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
                TextAPI::setUserString(0, (char*)name);
                g_TownMenuMessage.addMessage(0x92a29, 0x92a2a);
                g_TownMenuMessage.setYesNo(1);
                g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
                mode_ = 2;
                g_EnvoyManager.removeEnvoy(navigator_.getIndex(menuItem_.active_));
                envoyCount_ = g_EnvoyManager.getEnvoyCount();
                navigator_.setup(2, 4, envoyCount_);
                return;
            }
            if (mode_ == 2) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a1e);
                if (g_EnvoyManager.getEnvoyCount() == 0) {
                    g_TownMenuMessage.addMessage(0x92a21, 0x92a22);
                    mode_ = 3;
                    return;
                }
                mode_ = 0;
                menuItem_.active_ = 0;
                navigator_.setPageNo(0);
                envoyCount_ = g_EnvoyManager.getEnvoyCount();
                return;
            }
            if (mode_ == 3) {
                close();
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessageNOWAIT(0x92a33);
                g_TownMenuMessage.addMessageWAITKEY();
                gMaterielMenu_SURECHIGAI_ROOT.open();
            }
            return;
        }
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            if ((unsigned int)(mode_ - 1) <= 1) {
                if (mode_ == 1) {
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a2c);
                    mode_ = 3;
                    return;
                }
                close();
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessageNOWAIT(0x92a33);
                g_TownMenuMessage.addMessageWAITKEY();
                gMaterielMenu_SURECHIGAI_ROOT.open();
            }
        }
        return;
    }
    if (mode_ == 0) {
        navigator_.setup(2, 4, envoyCount_);
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            if (result == 2) {
                g_EnvoyManager.selectEnvoy(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = g_EnvoyManager.getName();
                mode_ = 1;
                g_TownMenuMessage.openMessageForTALK();
                TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
                TextAPI::setUserString(0, (char*)name);
                g_TownMenuMessage.addMessage(0x92a28);
                g_TownMenuMessage.setYesNo(1);
                g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
            }
            if (result == 3) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a2f);
                mode_ = 3;
            }
            redraw_ = 1;
        }
    }
}
