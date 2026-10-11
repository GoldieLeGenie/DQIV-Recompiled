#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"
#include "main/cmn/UnkImmigrantTown.hpp"

THUMB void UnkMaterielMenu_02189630::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    int count = 0;
    int level = UnkImmigrantTown::getSingleton()->unkfunc_02038140();
    for (int i = 0; i < 23; i++) {
        if (status::excelParam.surechigaiTenant_[i].getLevel() <= level) {
            count++;
        }
    }
    if (level == 5) {
        count--;
        if (!g_AreaFlag.check(0x1b8)) {
            count--;
        }
    }
    unk_1c = count + data_020f0078.unkfunc_0203a388();
    navigator_.setup(2, 4, unk_1c);
}

THUMB void UnkMaterielMenu_02189630::menuExecute()
{
    MenuTemplate_materiel::suretigaiSelectChiaus(&menuItem_, menuItem_.active_, dss::max(dss::min(unk_1c - navigator_.getPageNo() * 8, 8), 0));
}

THUMB void UnkMaterielMenu_02189630::menuDraw()
{
    int flag = 0;
    data_020f0078.unkfunc_0203a388();
    int index = navigator_.getIndex(menuItem_.active_);
    if (index > data_020f0078.unkfunc_0203a388() - 1) {
        index = navigator_.getIndex(menuItem_.active_) - data_020f0078.unkfunc_0203a388();
        flag = 1;
    } else {
        index = navigator_.getIndex(menuItem_.active_);
    }
    int page = navigator_.getPageNo();
    int pageMax = navigator_.getPageMaxCount();
    unkfunc_0216fe60(unk_1c, page, pageMax, index, 1, flag);
    if (unk_1c != 0) {
        menuItem_.drawActive();
    }
}

THUMB void UnkMaterielMenu_02189630::menuUpdate()
{
    if (data_020ed1bc.isOpen() && (unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
        data_020ed1bc.close();
    }
    navigator_.setup(2, 4, unk_1c);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 3) {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessageNOWAIT(0x92a33);
            data_020ed1bc.addMessageWAITKEY();
            close();
            gMaterielMenu_SURECHIGAI_ROOT.open();
        }
        redraw_ = 1;
    }
}
