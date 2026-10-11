#include "main/btl/BattleScriptManager.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/StageStatus.hpp"
#include "main/global/Global.hpp"
#include "main/status/BattleResult.hpp"
#include "main/Commands/CommonCommand.hpp"

char btldpou1[12] = "btldpou1";
char btldpou2[12] = "btldpou2";
char btldpou3[12] = "btldpou3";
char btldpou4[12] = "btldpou4";
char btldpou5[12] = "btldpou5";
char btldpou6[12] = "btldpou6";
char btldpou7[12] = "btldpou7";
char btldpou8[12] = "btldpou8";
char btldpou9[12] = "btldpou9";
char btlmc2_d[12] = "btlmc2_d";


THUMB btl::BattleScriptManager::BattleScriptManager()
{
    checkBattle_ = 0;
    setDataCount_ = 0;
}


THUMB void btl::BattleScriptManager::setScriptBattleResult(int flagType, int flagNo, int victory)
{
    checkBattle_ = 1;
    flagInfo_[setDataCount_].flagNo = flagNo;
    flagInfo_[setDataCount_].flagType = flagType;
    flagInfo_[setDataCount_].victory = victory;
    setDataCount_ = setDataCount_ + 1;
}


THUMB void btl::BattleScriptManager::checkScriptBattleResult()
{
    if (checkBattle_ == 1) {
        for (int i = 0; i < setDataCount_; i++) {
            btl::SetFlagParam param;
            param.flagType = flagInfo_[i].flagType;
            param.flagNo = flagInfo_[i].flagNo;
            param.value = 0;

            if (status::g_BattleResult.playerVictory_ == 1) {
                param.value = (flagInfo_[i].victory == 0) ? 1 : 0;
            } else {
                param.value = (flagInfo_[i].victory == 1) ? 1 : 0;
            }

            cmd_set_flag((int*)&param);
        }
    }

    setDataCount_ = 0;
    checkBattle_ = 0;
}

THUMB btl::BattleScriptManager* btl::BattleScriptManager::getSingleton()
{
    static btl::BattleScriptManager singleton;
    return &singleton;
}

THUMB void btl::BattleScriptManager::setEncountMap(int tile)
{
    if (tile != 167 && tile != 168) {
        if (tile != 243) {
            return;
        }
        if (g_AreaFlag.check(0x1b7) == 1) {
            g_Stage.setEvBtlMapName(btldpou9);
            return;
        }
        if (g_AreaFlag.check(0x1b6) == 1) {
            g_Stage.setEvBtlMapName(btldpou8);
            return;
        }
        if (g_AreaFlag.check(0x1b5) == 1) {
            g_Stage.setEvBtlMapName(btldpou7);
            return;
        }
        if (g_AreaFlag.check(0x1b4) == 1) {
            g_Stage.setEvBtlMapName(btldpou6);
            return;
        }
        if (g_AreaFlag.check(0x1b3) == 1) {
            g_Stage.setEvBtlMapName(btldpou5);
            return;
        }
        if (g_AreaFlag.check(0x1b2) == 1) {
            g_Stage.setEvBtlMapName(btldpou4);
            return;
        }
        if (g_AreaFlag.check(0x1b1) == 1) {
            g_Stage.setEvBtlMapName(btldpou3);
            return;
        }
        if (g_AreaFlag.check(0x1a8) == 1) {
            g_Stage.setEvBtlMapName(btldpou2);
            return;
        }
        g_Stage.setEvBtlMapName(btldpou1);
        return;
    }

    if (g_Global.getMapName()[0] == 'm') {
        if (g_Global.getMapName()[1] == 'c') {
            g_Stage.setEvBtlMapName(btlmc2_d);
        }
    }
}