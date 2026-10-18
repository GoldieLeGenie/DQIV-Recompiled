#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/Global.hpp"
#include "ov000/town/TownPlayerManager.hpp"

int MaterielMenu_SlotEnter::machineSelect_;

THUMB void MaterielMenu_SlotEnter::menuSetup()
{
    enableFlag_ = 1;
    machine_ = machineSelect_;
}

THUMB void MaterielMenu_SlotEnter::setSlotType(int type)
{
    machineSelect_ = type;
    machine_ = type;
}

THUMB void MaterielMenu_SlotEnter::menuUpdate()
{
    enableUpdate();
}

THUMB void MaterielMenu_SlotEnter::enableUpdate()
{
    if (g_TownMenuMessage.isOpen() != 0) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            TownPlayerManager::getSingleton()->setLock(1);
            g_cmnPartyInfo.prevLocation_ = 1;
            g_Global.setMinigame(1);
            g_Global.setGameStatus(machineSelect_);
            g_Global.startCasino();
        } else if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
    } else {
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessage(0xc96aa);
        g_TownMenuMessage.setYesNo();
    }
}
