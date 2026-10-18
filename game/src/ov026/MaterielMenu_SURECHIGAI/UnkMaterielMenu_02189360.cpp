#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"

THUMB void UnkMaterielMenu_02189360::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    unk_1c = 0;
    unk_20 = g_EnvoyManager.getEnvoyCount();
    navigator_.setup(2, 4, unk_20);
}

THUMB void UnkMaterielMenu_02189360::menuExecute()
{
    int count = unk_20 - navigator_.getPageNo() * 8;
    MenuTemplate_materiel::suretigaiSelectChiaus(&menuItem_, menuItem_.active_, count);
}

THUMB void UnkMaterielMenu_02189360::menuDraw()
{
    if (g_TownMenuMessage.isOpen() != 1) {
        if (unk_1c <= 1) {
            int page = navigator_.getPageNo();
            int pageMax = navigator_.getPageMaxCount();
            drawTagModeTenantListScreen(unk_1c, unk_20, page, pageMax, navigator_.getIndex(menuItem_.active_));
        }
        if (unk_1c == 0) {
            menuItem_.drawActive();
        }
    }
}

THUMB void UnkMaterielMenu_02189360::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (unk_1c == 1) {
                g_EnvoyManager.selectEnvoy(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = g_EnvoyManager.getName();
                g_TownMenuMessage.openMessageForTALK();
                TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
                TextAPI::setUserString(0, (char*)name);
                g_TownMenuMessage.addMessage(0x92a29, 0x92a2a);
                g_TownMenuMessage.setYesNo(1);
                g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
                unk_1c = 2;
                g_EnvoyManager.removeEnvoy(navigator_.getIndex(menuItem_.active_));
                unk_20 = g_EnvoyManager.getEnvoyCount();
                navigator_.setup(2, 4, unk_20);
                return;
            }
            if (unk_1c == 2) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a1e);
                if (g_EnvoyManager.getEnvoyCount() == 0) {
                    g_TownMenuMessage.addMessage(0x92a21, 0x92a22);
                    unk_1c = 3;
                    return;
                }
                unk_1c = 0;
                menuItem_.active_ = 0;
                navigator_.setPageNo(0);
                unk_20 = g_EnvoyManager.getEnvoyCount();
                return;
            }
            if (unk_1c == 3) {
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
            if ((unsigned int)(unk_1c - 1) <= 1) {
                if (unk_1c == 1) {
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a2c);
                    unk_1c = 3;
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
    if (unk_1c == 0) {
        navigator_.setup(2, 4, unk_20);
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            if (result == 2) {
                g_EnvoyManager.selectEnvoy(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = g_EnvoyManager.getName();
                unk_1c = 1;
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
                unk_1c = 3;
            }
            redraw_ = 1;
        }
    }
}
