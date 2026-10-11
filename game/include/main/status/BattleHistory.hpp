#pragma once
#include <globaldefs.h>
#include "main/profile/Profile.hpp" 

namespace status{
    struct BattleHistory {
        enum HistoryType {
            RightNow = 0x0,
            WinDeathPissaro = 0x1,
            WinEvilPrist = 0x2,
        };
        HistoryType historyType_;
        unsigned int adventureTime_[3];
        unsigned int battleCount_[3];
        unsigned int monsterCount_[3];
        unsigned int totalGold_[3];
        unsigned int victoryGold_[3];
        unsigned int wipeoutCount_[3];
        unsigned int escapeCount_[3];
        unsigned short maxDamage_[3];
        unsigned short title_[3];
        unsigned char heroLevel_[3];
        unsigned int chapterBattleCount_;
        unsigned int chapterWipeoutCount_;
        unsigned int chapterEscapeCount_;
        unsigned char restMonster_[3];
        BattleHistory();
        ~BattleHistory();
        void initialize();
        void setAdventureTime(unsigned int time);
        void regenesisAdventureTime();
        unsigned int getAdventureTime();
        int getBattleCount();
        void regenesisMonsterCount();
        int getMonsterCount();
        void regenesisTotalGold(unsigned int gold);
        int getTotalGold();
        void regenesisVictoryCount();
        int getVictoryCount();
        void setWipeoutCount(unsigned int count);
        int getWipeoutCount();
        void setEscapeCount(unsigned int count);
        int getEscapeCount();
        void regenesisMaxDamage(unsigned short damage);
        unsigned short getMaxDamage();
        void regenesisHeroLevel(unsigned char level);
        unsigned char getHeroLevel();
        unsigned char getRestMonsterCount();
        void setTitle(unsigned short title);
        unsigned short getTitle();
        void setChapterBattleCount(unsigned int count);
        void regenesisChapterBattleCount();
        unsigned int getChapterBattleCount();
        void setChapterWipeoutCount(unsigned int count);
        void regenesisChapterWipeoutCount();
        unsigned int getChapterWipeoutCount();
        void setChapterEscapeCount(unsigned int count);
        void regenesisChapterEscapeCount();
        unsigned int getChapterEscapeCount();
        void setSaveData(profile::PROFILE_PARTY* pPA, profile::PROFILE_HISTORY* pPB);
        void setLoadData(profile::PROFILE_PARTY* pPA, profile::PROFILE_HISTORY* pPH);
    };

    extern BattleHistory g_BattleHistory;  //data_020ecd3c
}

