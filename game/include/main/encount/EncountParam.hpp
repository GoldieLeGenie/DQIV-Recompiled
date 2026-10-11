#pragma once
#include <globaldefs.h>
#include "main/param/Param.hpp"
#include "GameInfo.hpp"

namespace encount {
    struct EncountParam{
        
        short tileId_;
        short tileLevel_;
        short tileRate_;
        short formationId_;
        short teamAddDisable_;
        short escapeDisable_;
        short eventEncount_;
        btl::FirstAttack firstAttack_;
        short sound_;
        unsigned short monsterIndex_[14];
        EncountParam();
        ~EncountParam();
        void setup(int index);
        int getFormationId();
        btl::FirstAttack getFirstAttack();
        bool isEventEncount();
        int getMonsterIndex(EncountType type);
        int getSound();
    };
}  // namespace encount
