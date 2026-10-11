#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/HaveStatusInfo.hpp"

namespace btl {
    struct BattleMenuPlayerControl 
    {                                       
        int activeChara_;
        int targetChara_;
        int activeItem_;
        int activeMagic_;
        int tacticsSex_;
        int secondHistory_[4];
        int firstHistory_[4];
        short memberHP_[4];
        short memberMP_[4];
        short memberLv_[4];
        short memberHPColor_[4];
        int memberCondition_[4];
        int conditionChange_[4];
        short targetMonsterGroup_[4];
        short magicPosition_[26];
        static BattleMenuPlayerControl* getSingleton();
        int getHPColor(int index) { return (index < 0) ? 0 : memberHPColor_[index]; }
        int getCondition(int index) { return memberCondition_[index]; }
        bool isConditionChange(int index) { return conditionChange_[index]; }
        int getPlayerItemId();
        void clear();
        void allClear();
        void setNoSelectHistory(int index);
        int makePlayerHistory();
        int resetPlayerHistory(int playerNum);
        void setAttackHistory();
        void setDefenceHistory();
        void setUseItemHistory();
        void setUseActionHistory();
        bool flashStatus(int memberNum);
        bool flashHP(int memberNum);
        bool flashMP(int memberNum);
        bool flashHPColor(int index);
        int isFlashHPColor(int index, status::HaveStatusInfo::DiffStatus timing);
        int flashCondition(int memberNum);
        int isFlashCondition(status::HaveStatusInfo* info, status::HaveStatusInfo::MenuStatusChange menuStatus);
        int getTargetGroup();
        void setTargetGroup(int monsterNum);
        void setMagicPosition(int position);
        int getMagicPosition();
    };
}

struct CondCheckTable { int v[6]; };     // data_ov015_021764e4 
struct CondMessageTable { int v[7]; };   // data_ov015_02176514 

