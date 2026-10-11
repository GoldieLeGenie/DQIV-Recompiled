#include "ov028/MaterielMenu_CHANGEGIFT/MaterielMenu_CHANGEGIFT.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_CHANGEGIFT_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    MaterielMenuPlayerControl::getSingleton()->allClear();
    mode_ = 0;
    int leadpc = 0;
    while (status::g_Party.getPlayerStatus(leadpc)->haveStatusInfo_.isDeath()) {
        leadpc++;
        if (leadpc > status::g_Party.getCount()) {
            leadpc = 0;
            break;
        }
    }
    MaterielMenuPlayerControl::getSingleton()->leadpc_ = leadpc;
}

THUMB void MaterielMenu_CHANGEGIFT_ROOT::menuDraw()
{
    unkfunc_0216fce8();
}

THUMB void MaterielMenu_CHANGEGIFT_ROOT::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            selectYes();
        } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            selectNo();
        }
    } else if (mode_ == 0) {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0xc8af2);
        data_020ed1bc.setMessageLastCursor(true);
        mode_ = 1;
    }
}

THUMB void MaterielMenu_CHANGEGIFT_ROOT::selectYes()
{
    data_020ed1bc.close();
    switch (mode_) {
    case 1:
        checkCoin();
        break;
    case 2:
    case 3:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0xc8b23);
        mode_ = 4;
        break;
    case 4:
        close();
        gMaterielMenu_CHANGEGIFT_SELECTGIFT.open();
        break;
    case 5:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_CHANGEGIFT_ROOT::selectNo()
{
    data_020ed1bc.close();
    if (mode_ == 2 || mode_ == 3) {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0xc8b21);
        mode_ = 5;
    }
}

THUMB void MaterielMenu_CHANGEGIFT_ROOT::checkCoin()
{
    int leadpc = MaterielMenuPlayerControl::getSingleton()->leadpc_;
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0xb, 0x50000000, status::g_Party.getPlayerIndex(leadpc));
    if (status::g_Party.casinoCoin_ == 0) {
        data_020ed1bc.addMessage(0xc8af5, 0xc8af6, 0xc8af7);
        mode_ = 5;
        return;
    }
    TextAPI::setMACRO0(0x2a, 0xf0000000, status::g_Party.casinoCoin_);
    data_020ed1bc.addMessage(0xc8afa);
    data_020ed1bc.setYesNo();
    mode_ = 2;
}

ARM void MaterielMenu_CHANGEGIFT_ROOT::menuExecute()
{
}
