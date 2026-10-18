#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/cmn/ImmigrantTown.hpp"

THUMB void MaterielMenu_SURECHIGAI_MAKE_TAISHI::menuSetup()
{
    status::g_Party.setPlayerMode();
    g_EnvoyManager.mode_ = 1;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    navigator_.setup(6, 2, 50);
    mode_ = 0;
    firstFlag_ = 1;
    changeTaishi_ = 0;
    g_EnvoyManager.backupMyEnvoy();
    int level = UnkImmigrantTown::getSingleton()->getTownLevel();
    if (level == 0) {
        level = 1;
    }
    for (int i = 0; i < 48; i++) {
        if (status::excelParam.surechigai_[i].level <= level) {
            g_EnvoyManager.setTypeSelectable(i, 1);
        }
    }
    if (level == 5) {
        g_EnvoyManager.setTypeSelectable(48, 1);
        g_EnvoyManager.setTypeSelectable(49, 1);
    }
}

THUMB void MaterielMenu_SURECHIGAI_MAKE_TAISHI::menuExecute()
{
    int count = 12;
    if (navigator_.getPageNo() == 4) {
        count = 2;
    }
    MenuTemplate_materiel::surechigaiSelectObject(&menuItem_, menuItem_.active_, count);
}

THUMB void MaterielMenu_SURECHIGAI_MAKE_TAISHI::menuDraw()
{
    if (g_TownMenuMessage.isMessageWAITPROG()) {
        int active = menuItem_.active_;
        drawTagModeAmbassadorScreen(mode_, active, navigator_.getPageNo(), navigator_.getPageMaxCount());
        menuItem_.drawActive();
    }
    if (mode_ == 1) {
        drawTagModeAmbassadorScreen(mode_, g_EnvoyManager.getType(), navigator_.getPageNo(), navigator_.getPageMaxCount());
    }
}

THUMB void MaterielMenu_SURECHIGAI_MAKE_TAISHI::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.isMessageWAITPROG()) {
            if (firstFlag_ == 1) {
                redraw_ = 1;
                firstFlag_ = 0;
                return;
            }
            navigator_.setup(6, 2, 50);
            int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
            if (result != 0) {
                if (result == 2) {
                    int index = navigator_.getIndex(menuItem_.active_);
                    if (g_EnvoyManager.isTypeSelectable(index) == 1) {
                        g_TownMenuMessage.clearMessageWAITPROG();
                        g_TownMenuMessage.close();
                        g_TownMenuMessage.openMessageForTALK();
                        g_TownMenuMessage.addMessage(0x92a73);
                        g_EnvoyManager.setType(index);
                        mode_ = 1;
                    }
                }
                if (result == 3 && changeTaishi_ == 1) {
                    g_TownMenuMessage.close();
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a2f);
                    mode_ = 2;
                    return;
                }
                redraw_ = 1;
            }
            return;
        }
        if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
            if (mode_ == 2) {
                close();
                gMaterielMenu_SURECHIGAI_ROOT.open();
                g_EnvoyManager.restoreMyEnvoy();
                g_EnvoyManager.clearBackup();
                return;
            }
            if (firstFlag_ == 0) {
                close();
                gMaterielMenu_NameEdit.open();
                gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI;
                gMaterielMenu_NameEdit.clearName();
                gMaterielMenu_NameEdit.setNameEditMode();
                if (changeTaishi_ == 1) {
                    gMaterielMenu_NameEdit.unk_98 = 1;
                }
            }
        }
        return;
    }
    if (firstFlag_ == 1) {
        g_TownMenuMessage.openMessageForTALK();
        if (changeTaishi_ == 1) {
            g_TownMenuMessage.addMessageNOWAIT(0x92abd);
        } else {
            g_TownMenuMessage.addMessage(0x92a6e);
            g_TownMenuMessage.addMessageNOWAIT(0x92a6f);
        }
        g_TownMenuMessage.addMessageWAITKEY();
    }
}
