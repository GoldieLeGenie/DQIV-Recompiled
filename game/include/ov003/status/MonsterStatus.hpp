#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/CharacterStatus.hpp"

namespace status{
    struct MonsterStatus : status::CharacterStatus {
        int              ctrlId_;        // 0x4F8
        int              sortIndex_;     // 0x4FC
        dss::BitFlaguint eventFlag_;     // 0x500
        int              mosyasIndex_;   // 0x504
        CharacterStatus* mosyasTarget_;  // 0x508
        MonsterStatus();
        ~MonsterStatus();
        void setup(int monsterGroup, int monsterIndex, int sortIndex);
        void cleanup();
        bool isEnable();
        bool isBattleEnable();
        bool isBeingEnable();
        int getDropItem();
        int getHaveDropItem();
        virtual void print();
        virtual void printAction();
        void naturalRecovery();
        void setStartStatus();
        void execStartOfRound();
        virtual bool isDeathDisable();
        virtual void setMosyasChange(int monsterIndex);
        virtual void setMosyasRelease();
    };
}