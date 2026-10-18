#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"

THUMB void MaterielMenu_SURECHIGAI_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    firstFlag_ = 1;
    mode_ = 5;
    MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ = 0;
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::menuExecute()
{
    MenuTemplate_materiel::surechigaiRoot(&menuItem_, menuItem_.active_);
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::menuDraw()
{
    if (g_TownMenuMessage.isMessageWAITPROG()) {
        drawTagModeRootScreen();
        menuItem_.drawActive();
    }
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.isMessageWAITPROG()) {
            if (firstFlag_ == 1) {
                redraw_ = 1;
                firstFlag_ = 0;
                return;
            }
            navigator_.setup(1, 5, 5);
            int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
            if (result != 0) {
                if (result == 2) {
                    g_TownMenuMessage.clearMessageWAITPROG();
                    selectCommand();
                }
                if (result == 3) {
                    g_TownMenuMessage.clearMessageWAITPROG();
                    g_TownMenuMessage.close();
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a37, 0x92a38);
                }
                redraw_ = 1;
            }
            return;
        }
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (mode_ == 1) {
                close();
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.changeTaishi_ = 1;
                return;
            }
            if (mode_ == 0) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessageNOWAIT(0x92a33);
                g_TownMenuMessage.addMessageWAITKEY();
                firstFlag_ = 1;
                return;
            }
            if (mode_ == 3) {
                if (g_EnvoyManager.getEnvoyCount() == 0) {
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a21, 0x92a22);
                    firstFlag_ = 1;
                } else {
                    close();
                    gUnkMaterielMenu_02189360.open();
                }
                mode_ = 5;
                return;
            }
            if (firstFlag_ == 0) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
            return;
        }
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            if (mode_ == 1) {
                menuItem_.active_ = 0;
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessageNOWAIT(0x92a2f);
                mode_ = 5;
                firstFlag_ = 1;
            }
            if (firstFlag_ == 0) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        }
        return;
    }
    if (firstFlag_ == 1) {
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessageNOWAIT(0x92ab4);
        g_TownMenuMessage.addMessageWAITKEY();
    }
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::selectCommand()
{
    g_TownMenuMessage.close();
    switch (menuItem_.active_) {
    case 0:
        mode_ = 0;
        if (g_EnvoyManager.findFreeEnvoy() == -1) {
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessage(0x92a16, 0x92a17);
            mode_ = 5;
            firstFlag_ = 1;
            break;
        }
        close();
        stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->type_ = 3;
        gMaterielMenu_SAVE.open();
        gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_SURECHIGAI;
        break;
    case 1:
        mode_ = 1;
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessageNOWAIT(0x92abc);
        g_TownMenuMessage.setYesNo(0);
        g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
        break;
    case 2:
        mode_ = 2;
        close();
        gUnkMaterielMenu_02189630.open();
        break;
    case 3:
        mode_ = 3;
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x92a1c, 0x92a1d, 0x92a1e);
        break;
    case 4:
        mode_ = 4;
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x92a37, 0x92a38);
        break;
    }
}
