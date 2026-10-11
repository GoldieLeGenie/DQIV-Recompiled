#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov003/btl/DefenceAutoActionParam.hpp"
#include "ov003/btl/RecoveryAutoActionParam.hpp"
#include "ov003/btl/AttackAutoActionParam.hpp"
#include "ov003/btl/AutoActionParam.hpp"
namespace btl {
    struct AutoAction
    {
        status::PlayerStatus *player_;
        int attackCount_;
        int recoveryCount_;
        int defenceCount_;
        int assistCount_;
        int attackMaxIndex_;
        int recoveryMaxIndex_;
        int defenceMaxIndex_;
        int assistMaxIndex_;
        AttackAutoActionParam attackActionParam_[32];
        RecoveryAutoActionParam recoveryActionParam_[32];
        DefenceAutoActionParam defenceActionParam_[32];
        AutoActionParam assistActionParam_[32];
        int monsterLevel_;
        int playerLevel_;
        AutoAction();
        ~AutoAction();
        void clear();
        void setup(status::PlayerStatus* player);
        void selectAction();
        void selectMaxAction();
        void setLevelDifference();
        void setSelectAction();
        void add(int actionIndex, int itemIndex);
        int validAction(int actionIndex);
        void setSelectAttackAction();
        void setSelectRecoveryAction();
        void setSelectDefenceAction();
        void setDefenceActionOM();
        void setDefenceActionBG();
        void setRecoveryAction();
        void setRebirthAction();
        void setAntiSpazzAction();
        void setAction(int actionIndex, int itemIndex, int groupIndex, int targetIndex);
    };
    
}