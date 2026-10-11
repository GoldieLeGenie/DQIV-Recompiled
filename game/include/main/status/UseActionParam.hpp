#pragma once 
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/CharacterStatus.hpp"
#include "main/status/HaveItemSack.hpp"
#include "main/status/UseActionMessage.hpp"


namespace status{
    struct UseActionParam {
        CharacterStatus* actorCharacterStatus_;
        CharacterStatus* targetCharacterStatus_[18];
        CharacterStatus* bakTargetCharacterStatus_[18];
        CharacterStatus* originalTargetCharacterStatus_[18];
        int targetResult_[18];
        HaveItemSack* actorHaveItemSack_;
        int actionIndex_;
        int itemIndex_;
        int itemSortIndex_;
        int targetCount_;
        int damage_;
        int currentTargetIndex_;
        UseActionMessage message_[18];
        int result_;
        int exec_;
        UseActionParam() { clear(); };
        ~UseActionParam() {}
        void clear();
        void clearMessage();
    };
} 
