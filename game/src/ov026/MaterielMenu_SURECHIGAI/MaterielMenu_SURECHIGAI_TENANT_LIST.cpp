#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/cmn/ImmigrantTown.hpp"

THUMB void MaterielMenu_SURECHIGAI_TENANT_LIST::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    int count = 0;
    int level = ImmigrantTown::getSingleton()->getTownLevel();
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
    tenantCount_ = count + g_EnvoyManager.getEnvoyCount();
    navigator_.setup(2, 4, tenantCount_);
}

THUMB void MaterielMenu_SURECHIGAI_TENANT_LIST::menuExecute()
{
    MenuTemplate_materiel::suretigaiSelectChiaus(&menuItem_, menuItem_.active_, dss::max(dss::min(tenantCount_ - navigator_.getPageNo() * 8, 8), 0));
}

THUMB void MaterielMenu_SURECHIGAI_TENANT_LIST::menuDraw()
{
    int flag = 0;
    g_EnvoyManager.getEnvoyCount();
    int index = navigator_.getIndex(menuItem_.active_);
    if (index > g_EnvoyManager.getEnvoyCount() - 1) {
        index = navigator_.getIndex(menuItem_.active_) - g_EnvoyManager.getEnvoyCount();
        flag = 1;
    } else {
        index = navigator_.getIndex(menuItem_.active_);
    }
    int page = navigator_.getPageNo();
    int pageMax = navigator_.getPageMaxCount();
    drawTagModeTenantScreen(tenantCount_, page, pageMax, index, 1, flag);
    if (tenantCount_ != 0) {
        menuItem_.drawActive();
    }
}

THUMB void MaterielMenu_SURECHIGAI_TENANT_LIST::menuUpdate()
{
    if (g_TownMenuMessage.isOpen() && (unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
        g_TownMenuMessage.close();
    }
    navigator_.setup(2, 4, tenantCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 3) {
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessageNOWAIT(0x92a33);
            g_TownMenuMessage.addMessageWAITKEY();
            close();
            gMaterielMenu_SURECHIGAI_ROOT.open();
        }
        redraw_ = 1;
    }
}
