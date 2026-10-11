#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"

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
    if (data_020ed1bc.isMessageWAITPROG()) {
        unkfunc_0216fe58();
        menuItem_.drawActive();
    }
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.isMessageWAITPROG()) {
            if (firstFlag_ == 1) {
                redraw_ = 1;
                firstFlag_ = 0;
                return;
            }
            navigator_.setup(1, 5, 5);
            int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
            if (result != 0) {
                if (result == 2) {
                    data_020ed1bc.clearMessageWAITPROG();
                    selectCommand();
                }
                if (result == 3) {
                    data_020ed1bc.clearMessageWAITPROG();
                    data_020ed1bc.close();
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(0x92a37, 0x92a38);
                }
                redraw_ = 1;
            }
            return;
        }
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (mode_ == 1) {
                close();
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.changeTaishi_ = 1;
                return;
            }
            if (mode_ == 0) {
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessageNOWAIT(0x92a33);
                data_020ed1bc.addMessageWAITKEY();
                firstFlag_ = 1;
                return;
            }
            if (mode_ == 3) {
                if (data_020f0078.unkfunc_0203a388() == 0) {
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(0x92a21, 0x92a22);
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
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (mode_ == 1) {
                menuItem_.active_ = 0;
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessageNOWAIT(0x92a2f);
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
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(0x92ab4);
        data_020ed1bc.addMessageWAITKEY();
    }
}

THUMB void MaterielMenu_SURECHIGAI_ROOT::selectCommand()
{
    data_020ed1bc.close();
    switch (menuItem_.active_) {
    case 0:
        mode_ = 0;
        if (data_020f0078.unkfunc_0203a364() == -1) {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(0x92a16, 0x92a17);
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
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(0x92abc);
        data_020ed1bc.setYesNo(0);
        data_020ed1bc.setYesNoPosition(0xc0, 0x40);
        break;
    case 2:
        mode_ = 2;
        close();
        gUnkMaterielMenu_02189630.open();
        break;
    case 3:
        mode_ = 3;
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x92a1c, 0x92a1d, 0x92a1e);
        break;
    case 4:
        mode_ = 4;
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x92a37, 0x92a38);
        break;
    }
}
