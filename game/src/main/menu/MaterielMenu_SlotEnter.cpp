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
    if (data_020ed1bc.isOpen() != 0) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            TownPlayerManager::getSingleton()->setLock(1);
            g_cmnPartyInfo.prevLocation_ = 1;
            g_Global.setMinigame(1);
            g_Global.setGameStatus(machineSelect_);
            g_Global.startCasino();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
    } else {
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessage(0xc96aa);
        data_020ed1bc.setYesNo();
    }
}
