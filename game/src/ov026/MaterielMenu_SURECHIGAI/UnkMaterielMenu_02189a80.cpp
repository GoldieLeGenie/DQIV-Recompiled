#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"

THUMB void UnkMaterielMenu_02189a80::menuSetup()
{
    status::g_Party.setPlayerMode();
    data_020f0078.mode_ = 1;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    unk_1c = 1;
    unk_20 = data_020f0078.unkfunc_0203a5ec();
}

THUMB void UnkMaterielMenu_02189a80::menuExecute()
{
    switch (unk_1c) {
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

THUMB void UnkMaterielMenu_02189a80::menuDraw()
{
    if (unk_1c != 0) {
        MaterielMenuPlayerControl::getSingleton();
        unkfunc_0216feb8(unk_1c, unk_20);
        if (unk_1c < 4) {
            menuItem_.drawActive();
        }
    }
}

THUMB void UnkMaterielMenu_02189a80::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (unk_1c == 0) {
                close();
                gMaterielMenu_NameEdit.open();
                gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI;
                gMaterielMenu_NameEdit.clearName();
                gMaterielMenu_NameEdit.setNameEditMode();
                return;
            }
            if (unk_1c == 4) {
                unk_1c = 8;
                ui_MsgSndSet(0x32);
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a79);
                return;
            }
            if (unk_1c == 7) {
                unk_1c = 6;
                ui_MsgSndSet(0x32);
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a81);
                return;
            }
            if (unk_1c == 6) {
                data_020ed1bc.openMessageForTALK();
                if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                    data_020ed1bc.addMessage(0x92a94);
                } else {
                    ui_MsgSndSet(0x32);
                    data_020ed1bc.addMessage(0x92a81);
                    if (unk_24 == 1) {
                        MaterielMenu_WINDOW_MANAGER::getSingleton()->changeTaishi_ = 1;
                    }
                    data_020f0078.unkfunc_0203aaac();
                }
                unk_1c = 9;
                return;
            }
            if (unk_1c > 4) {
                unkfunc_02189d70();
                return;
            }
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            return;
        }
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (unk_1c == 4) {
                unk_1c = 5;
                ui_MsgSndSet(0x32);
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a7b);
                return;
            }
            if (unk_1c == 5) {
                unk_1c = 8;
                ui_MsgSndSet(0x32);
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a83);
                return;
            }
            if (unk_1c == 6) {
                unk_1c = 8;
                data_020ed1bc.openMessageForTALK();
                if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                    data_020ed1bc.addMessage(0x92a8e);
                } else {
                    ui_MsgSndSet(0x32);
                    data_020ed1bc.addMessage(0x92a83, 0x92a79);
                }
                unkfunc_02189d70();
                return;
            }
            if (unk_1c == 9) {
                unk_1c = 6;
                return;
            }
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
        return;
    }
    switch (unk_1c) {
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
            unkfunc_02189d70();
        }
        if (result == 3) {
            unk_1c--;
            if (unk_1c == 1) {
                menuItem_.active_ = data_020f0078.unkfunc_0203a714();
            }
            if (unk_1c == 2) {
                menuItem_.active_ = data_020f0078.unkfunc_0203a750();
            }
            if (unk_1c == 0) {
                ui_MsgSndSet(0x32);
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a73);
            }
        }
        if (unk_1c == 3) {
            int page = navigator_.getPageNo();
            MaterielMenuPlayerControl::getSingleton()->activeChiausSkillPage_ = page;
        }
        redraw_ = 1;
    }
}

THUMB void UnkMaterielMenu_02189a80::unkfunc_02189d70()
{
    switch (unk_1c) {
    case 1:
        unk_1c = 2;
        data_020f0078.unkfunc_0203a6f4(menuItem_.active_);
        menuItem_.active_ = 0;
        break;
    case 2:
        unk_1c = 3;
        data_020f0078.unkfunc_0203a730(menuItem_.active_);
        menuItem_.active_ = 0;
        break;
    case 3: {
        data_020f0078.unkfunc_0203a674(status::g_Story.heroName);
        data_020f0078.unkfunc_0203a58c(status::g_Game.getUniqueID());
        unk_1c = 4;
        data_020f0078.unkfunc_0203a76c(navigator_.getIndex(menuItem_.active_));
        int page = navigator_.getPageNo();
        MaterielMenuPlayerControl::getSingleton()->activeChiausSkillPage_ = page;
        menuItem_.active_ = 0;
        ui_MsgSndSet(0x32);
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x92a78);
        data_020ed1bc.setYesNo();
        data_020ed1bc.setYesNoPosition(0xc0, 0x40);
        break;
    }
    case 5:
        close();
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
        if (unk_24 == 1) {
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
        gMaterielMenu_NameEdit.unkfunc_0203b004();
        if (unk_24 == 1) {
            gMaterielMenu_NameEdit.unk_98 = 1;
        }
        break;
    case 9:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}
