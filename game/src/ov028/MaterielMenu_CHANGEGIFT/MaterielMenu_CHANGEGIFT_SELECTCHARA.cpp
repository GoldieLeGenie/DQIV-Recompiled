#include "ov028/MaterielMenu_CHANGEGIFT/MaterielMenu_CHANGEGIFT.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_CHANGEGIFT_SELECTCHARA::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    activeChara_ = 0;
    maxCharaCount_ = status::g_Party.getCount();
    if (status::g_Party.fukuro_ != 0) {
        maxCharaCount_++;
    }
    navigator_.setupBase();
}

THUMB void MaterielMenu_CHANGEGIFT_SELECTCHARA::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_ICON32_5x2(&menuItem_, activeChara_, maxCharaCount_);
    MenuTemplate_materiel::MATERIEL_CANCEL(&menuItem2_);
}

THUMB void MaterielMenu_CHANGEGIFT_SELECTCHARA::menuDraw()
{
    unkfunc_0216fb6c(1);
    menuItem_.drawActive();
    menuItem2_.drawActive();
}

THUMB void MaterielMenu_CHANGEGIFT_SELECTCHARA::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(menuItem2_)) {
        cancelChange();
        return;
    }
    navigator_.setup(5, 2, maxCharaCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        activeChara_ = menuItem_.active_;
        int activeChara = activeChara_;
        MaterielMenuPlayerControl::getSingleton()->activeChara_ = activeChara;
        if (result == 2) {
            close();
            gMaterielMenu_CHANGEGIFT_EQUIPCHECK.open();
        }
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_CHANGEGIFT_SELECTCHARA::cancelChange()
{
    int leadpc = MaterielMenuPlayerControl::getSingleton()->leadpc_;
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0xb, 0x50000000, status::g_Party.getPlayerStatus(leadpc)->haveStatusInfo_.haveStatus_.playerIndex_);
    TextAPI::setMACRO0(0x2a, 0xf0000000, status::g_Party.casinoCoin_);
    data_020ed1bc.addMessage(0xc8afa);
    data_020ed1bc.setYesNo();
    close();
    gMaterielMenu_CHANGEGIFT_ROOT.open();
    gMaterielMenu_CHANGEGIFT_ROOT.mode_ = 3;
}
