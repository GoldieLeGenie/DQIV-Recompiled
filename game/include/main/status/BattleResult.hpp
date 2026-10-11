#pragma once
#include "main/dss/DssUtils.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/BaseStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"


namespace status{
    struct BattleResult {
        struct Result {
            unsigned short monsterCount_;
            unsigned short itemCount_;
            unsigned char level_;
        };
        Result result_[210];
        dss::FlagArray<210> encountFlag_;
        int playerVictory_;
        int playerDemolition_;
        int playerDemolitionMessage_;
        int disablePlayerDemolition_;
        int battleTurnCount_;
        int battleExecute_;
        int playerFirstAttack_;
        int monsterFirstAttack_;
        unsigned short defaultMonsterNo_[210];
        BattleResult();
        ~BattleResult();
        void initialize();
        void setMonsterCount(unsigned int monsterIndex, unsigned short count);
        void addMonsterCount(unsigned int monsterIndex);
        unsigned short getMonsterCount(unsigned int monsterIndex);
        unsigned int getMaxExp(unsigned int monsterIndex);
        unsigned int getMaxGold(unsigned int monsterIndex);
        void setItemCount(unsigned int monsterIndex, unsigned short count);
        void addItemCount(unsigned int  monsterIndex);
        unsigned short getItemCount(unsigned int monsterIndex);
        unsigned int getItemIndex(unsigned int monsterIndex, int getItemID);
        void setLevel(unsigned int monsterIndex, unsigned short level);
        void regenesisLevel(unsigned int monsterIndex, unsigned char level);
        unsigned char getLevel(unsigned int monsterIndex);
        unsigned int getExpTotal(unsigned int monsterIndex);
        unsigned int getGoldTotal(unsigned int monsterIndex);
        void setEncount(unsigned int monsterIndex, int flag);
        bool isEncount(unsigned int monsterIndex);
        int getEncountCount();
        unsigned int getAllMonsterTotalExp();
        void setDisablePlayerDemolition(bool flag);
        int isDisablePlayerDemolition();
    };

    extern BattleResult g_BattleResult;            //data_020ec664
}

