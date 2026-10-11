#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/status/PartyStatus.hpp"

THUMB void BattleMenuSub_HISTORY::menuSetup()
{
    status::g_Party.setBattleMode();
    commandChara_ = -1;
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        btl::BattleMenuPlayerControl::getSingleton()->flashStatus(i);
    }
    update_ = 0;
}

THUMB void BattleMenuSub_HISTORY::menuExecute()
{
}

THUMB void BattleMenuSub_HISTORY::menuDraw()
{
    if (history_ != 0) {
        status::g_Party.setBattleMode();
        unkfunc_0216c468(commandChara_);
    }
}

THUMB void BattleMenuSub_HISTORY::menuUpdate()
{
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (btl::BattleMenuPlayerControl::getSingleton()->flashStatus(i)) {
            redraw_ = 1;
        }
    }
    if (update_ != 0) {
        status::g_Party.setBattleMode();
        int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            if (i == chara) {
                if (btl::BattleMenuPlayerControl::getSingleton()->makePlayerHistory()) {
                    redraw_ = 1;
                }
            } else if (btl::BattleMenuPlayerControl::getSingleton()->resetPlayerHistory(i)) {
                redraw_ = 1;
            }
        }
        if (isRedraw_ != 0) {
            redraw_ = 1;
            isRedraw_ = 0;
        }
    }
}
