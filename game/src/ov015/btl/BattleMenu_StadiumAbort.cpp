#include "ov015/btl/BattleMenu.hpp"
#include "main/status/PartyStatus.hpp"

ARM void BattleMenu_StadiumAbort::menuSetup()
{
    status::g_Party.setBattleMode();
}

ARM void BattleMenu_StadiumAbort::menuExecute()
{
}

ARM void BattleMenu_StadiumAbort::menuDraw()
{
    status::g_Party.setBattleMode();
    unkfunc_0216c040();
}

ARM void BattleMenu_StadiumAbort::menuUpdate()
{
}
