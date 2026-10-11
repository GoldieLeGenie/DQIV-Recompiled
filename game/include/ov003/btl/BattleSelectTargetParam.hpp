#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/CharacterStatus.hpp"

namespace btl {
    struct BattleSelectTargetParam {
        enum CallTargetSelect {
            StartRound=0,
            StartTurn=1
        };
        status::CharacterStatus *sourceCharacterStatus[18];
        status::CharacterStatus *targetCharacterStatus[18];
        status::CharacterStatus *actorCharacterStatus_;
        int sourceCount_;
        int targetCount_;
        int actionIndex_;
        int targetGroup_;
        int targetIndex_;
        CallTargetSelect callTarget_;
        BattleSelectTargetParam();                                   
        ~BattleSelectTargetParam();                                                                                         
        void clear();
        void setSourceCharacterStatus(int index,status::CharacterStatus *characterStatus);
        status::CharacterStatus* getSourceCharacterStatus(int index);               
        int getSourceCountForGroup(int group);
        status::CharacterStatus* getSourceCharacterStatusForGroup(int group, int index);
        int getSourceCountForGroupDead(int group);
        status::CharacterStatus* getSourceCharacterStatusForGroupDead(int group, int index);
        void setTargetCharacterStatus(int index, status::CharacterStatus* target); 
        status::CharacterStatus* getTargetCharacterStatus(int index);
    };
}