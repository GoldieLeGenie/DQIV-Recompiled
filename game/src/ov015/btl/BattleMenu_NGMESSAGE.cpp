#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

THUMB void BattleMenu_NGMESSAGE::menuSetup()
{
    messageID_ = 0;
    returnPos_ = 0;
}

THUMB void BattleMenu_NGMESSAGE::menuExecute()
{
}

THUMB void BattleMenu_NGMESSAGE::menuDraw()
{
}

THUMB void BattleMenu_NGMESSAGE::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
            close();
            if (returnMenu_ == MENU_ACTIONMENU) {
                BattleMonsterNamePlate::getSingleton().init();
                BattleMonsterNamePlate::getSingleton().setMonster();
                gBattleMenu_ACTIONMENU.open();
                gBattleMenu_ACTIONMENU.pageItem_.active_ = returnPos_;
            } else {
                gBattleMenu_ROOT.open();
                gBattleMenu_ROOT.menuItem_.active_ = returnPos_;
            }
        }
        return;
    }
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    if (chara != -1) {
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveStatus_.playerIndex_);
    }
    g_TownMenuMessage.openMessageForBATTLE();
    g_TownMenuMessage.addMessage(messageID_);
}
