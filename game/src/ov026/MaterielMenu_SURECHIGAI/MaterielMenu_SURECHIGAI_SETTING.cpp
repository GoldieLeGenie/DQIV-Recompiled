#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"

THUMB void MaterielMenu_SURECHIGAI_SETTING::menuSetup()
{
    status::g_Party.setPlayerMode();
    g_EnvoyManager.mode_ = 1;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    step_ = 1;
    envoyType_ = g_EnvoyManager.getType();
}

THUMB void MaterielMenu_SURECHIGAI_SETTING::menuExecute()
{
    switch (step_) {
    case 1:
        MenuTemplate_materiel::surechigaiSelectSex(&menuItem_, menuItem_.active_);
        break;
    case 2:
        MenuTemplate_materiel::surechigaiSelectAetas(&menuItem_, menuItem_.active_);
        break;
    case 3:
        MenuTemplate_materiel::surechigaiSelectSkill(&menuItem_, menuItem_.active_);
        break;
    }
}

THUMB void MaterielMenu_SURECHIGAI_SETTING::menuDraw()
{
    if (step_ != 0) {
        MaterielMenuPlayerControl::getSingleton();
        drawNameEditScreen(step_, envoyType_);
        if (step_ < 4) {
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_SURECHIGAI_SETTING::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (step_ == 0) {
                close();
                gMaterielMenu_NameEdit.open();
                gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI;
                gMaterielMenu_NameEdit.clearName();
                gMaterielMenu_NameEdit.setNameEditMode();
                return;
            }
            if (step_ == 4) {
                step_ = 8;
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a79);
                return;
            }
            if (step_ == 7) {
                step_ = 6;
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a81);
                return;
            }
            if (step_ == 6) {
                g_TownMenuMessage.openMessageForTALK();
                if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                    g_TownMenuMessage.addMessage(0x92a94);
                } else {
                    ui_MsgSndSet(0x32);
                    g_TownMenuMessage.addMessage(0x92a81);
                    if (changeTaishi_ == 1) {
                        MaterielMenu_WINDOW_MANAGER::getSingleton()->changeTaishi_ = 1;
                    }
                    g_EnvoyManager.clearBackup();
                }
                step_ = 9;
                return;
            }
            if (step_ > 4) {
                proceedStep();
                return;
            }
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            return;
        }
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            if (step_ == 4) {
                step_ = 5;
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a7b);
                return;
            }
            if (step_ == 5) {
                step_ = 8;
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a83);
                return;
            }
            if (step_ == 6) {
                step_ = 8;
                g_TownMenuMessage.openMessageForTALK();
                if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                    g_TownMenuMessage.addMessage(0x92a8e);
                } else {
                    ui_MsgSndSet(0x32);
                    g_TownMenuMessage.addMessage(0x92a83, 0x92a79);
                }
                proceedStep();
                return;
            }
            if (step_ == 9) {
                step_ = 6;
                return;
            }
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
        return;
    }
    switch (step_) {
    case 1:
        navigator_.setup(3, 1, 3);
        break;
    case 2:
        navigator_.setup(4, 2, 8);
        break;
    case 3:
        navigator_.setup(2, 4, 0x40);
        break;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            proceedStep();
        }
        if (result == 3) {
            step_--;
            if (step_ == 1) {
                menuItem_.active_ = g_EnvoyManager.getSex();
            }
            if (step_ == 2) {
                menuItem_.active_ = g_EnvoyManager.getAge();
            }
            if (step_ == 0) {
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a73);
            }
        }
        if (step_ == 3) {
            int page = navigator_.getPageNo();
            MaterielMenuPlayerControl::getSingleton()->activeChiausSkillPage_ = page;
        }
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SURECHIGAI_SETTING::proceedStep()
{
    switch (step_) {
    case 1:
        step_ = 2;
        g_EnvoyManager.setSex(menuItem_.active_);
        menuItem_.active_ = 0;
        break;
    case 2:
        step_ = 3;
        g_EnvoyManager.setAge(menuItem_.active_);
        menuItem_.active_ = 0;
        break;
    case 3: {
        g_EnvoyManager.setHeroName(status::g_Story.heroName);
        g_EnvoyManager.setUnique(status::g_Game.getUniqueID());
        step_ = 4;
        g_EnvoyManager.setSkill(navigator_.getIndex(menuItem_.active_));
        int page = navigator_.getPageNo();
        MaterielMenuPlayerControl::getSingleton()->activeChiausSkillPage_ = page;
        menuItem_.active_ = 0;
        ui_MsgSndSet(0x32);
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x92a78);
        g_TownMenuMessage.setYesNo();
        g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
        break;
    }
    case 5:
        close();
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
        if (changeTaishi_ == 1) {
            gMaterielMenu_SURECHIGAI_SELECT_OBJECT.changeTaishi_ = 1;
        }
        break;
    case 6:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    case 8:
        close();
        gMaterielMenu_NameEdit.open();
        gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI_MESSAGE;
        gMaterielMenu_NameEdit.clearName();
        gMaterielMenu_NameEdit.setMessageMode();
        if (changeTaishi_ == 1) {
            gMaterielMenu_NameEdit.changeTaishi_ = 1;
        }
        break;
    case 9:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}
