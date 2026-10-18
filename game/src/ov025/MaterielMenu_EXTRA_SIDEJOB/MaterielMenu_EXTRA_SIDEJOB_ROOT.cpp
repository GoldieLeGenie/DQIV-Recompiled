#include "ov025/MaterielMenu_EXTRA_SIDEJOB/MaterielMenu_EXTRA_SIDEJOB.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/dss/Random.hpp"
#include "main/status/ShopList.hpp"

THUMB void MaterielMenu_EXTRA_SIDEJOB_ROOT::menuSetup()
{
    for (int i = 0; i < 3; i++) {
        status::g_Shop.sideJobItemFlag_[i] = 1;
    }
    mode_ = (dssrand::rand(NEXT_MENU_ODDS) == 0) ? SIDEJOB_BUY : SIDEJOB_SELL;
    g_TownMenuMessage.openMessageForTALK();
    g_TownMenuMessage.addMessage(0x63c1);
    g_TownMenuMessage.setYesNo();
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_ROOT::menuExecute()
{
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_ROOT::menuDraw()
{
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_ROOT::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (mode_ == SIDEJOB_SELL) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x63c2);
                close();
                gMaterielMenu_EXTRA_SIDEJOB_SELL.open();
            } else if (mode_ == SIDEJOB_BUY) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x63ed);
                close();
                gMaterielMenu_EXTRA_SIDEJOB_BUY.open();
            } else {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
                return;
            }
            g_TownMenuMessage.setYesNo();
        } else if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessage(0x63e8);
            mode_ = SIDEJOB_END;
        }
    }
}
