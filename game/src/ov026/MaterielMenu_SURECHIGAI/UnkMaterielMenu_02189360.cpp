#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"

THUMB void UnkMaterielMenu_02189360::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    unk_1c = 0;
    unk_20 = data_020f0078.unkfunc_0203a388();
    navigator_.setup(2, 4, unk_20);
}

THUMB void UnkMaterielMenu_02189360::menuExecute()
{
    int count = unk_20 - navigator_.getPageNo() * 8;
    MenuTemplate_materiel::suretigaiSelectChiaus(&menuItem_, menuItem_.active_, count);
}

THUMB void UnkMaterielMenu_02189360::menuDraw()
{
    if (data_020ed1bc.isOpen() != 1) {
        if (unk_1c <= 1) {
            int page = navigator_.getPageNo();
            int pageMax = navigator_.getPageMaxCount();
            unkfunc_0216fe7c(unk_1c, unk_20, page, pageMax, navigator_.getIndex(menuItem_.active_));
        }
        if (unk_1c == 0) {
            menuItem_.drawActive();
        }
    }
}

THUMB void UnkMaterielMenu_02189360::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (unk_1c == 1) {
                data_020f0078.unkfunc_0203a34c(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = data_020f0078.unkfunc_0203a65c();
                data_020ed1bc.openMessageForTALK();
                TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
                TextAPI::setUserString(0, (char*)name);
                data_020ed1bc.addMessage(0x92a29, 0x92a2a);
                data_020ed1bc.setYesNo(1);
                data_020ed1bc.setYesNoPosition(0xc0, 0x40);
                unk_1c = 2;
                data_020f0078.unkfunc_0203a3a8(navigator_.getIndex(menuItem_.active_));
                unk_20 = data_020f0078.unkfunc_0203a388();
                navigator_.setup(2, 4, unk_20);
                return;
            }
            if (unk_1c == 2) {
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a1e);
                if (data_020f0078.unkfunc_0203a388() == 0) {
                    data_020ed1bc.addMessage(0x92a21, 0x92a22);
                    unk_1c = 3;
                    return;
                }
                unk_1c = 0;
                menuItem_.active_ = 0;
                navigator_.setPageNo(0);
                unk_20 = data_020f0078.unkfunc_0203a388();
                return;
            }
            if (unk_1c == 3) {
                close();
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessageNOWAIT(0x92a33);
                data_020ed1bc.addMessageWAITKEY();
                gMaterielMenu_SURECHIGAI_ROOT.open();
            }
            return;
        }
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if ((unsigned int)(unk_1c - 1) <= 1) {
                if (unk_1c == 1) {
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(0x92a2c);
                    unk_1c = 3;
                    return;
                }
                close();
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessageNOWAIT(0x92a33);
                data_020ed1bc.addMessageWAITKEY();
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
                data_020f0078.unkfunc_0203a34c(navigator_.getIndex(menuItem_.active_));
                unsigned char* name = data_020f0078.unkfunc_0203a65c();
                unk_1c = 1;
                data_020ed1bc.openMessageForTALK();
                TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
                TextAPI::setUserString(0, (char*)name);
                data_020ed1bc.addMessage(0x92a28);
                data_020ed1bc.setYesNo(1);
                data_020ed1bc.setYesNoPosition(0xc0, 0x40);
            }
            if (result == 3) {
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a2f);
                unk_1c = 3;
            }
            redraw_ = 1;
        }
    }
}
