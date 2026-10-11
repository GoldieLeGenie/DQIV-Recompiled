#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/BattleSelectTargetParam.hpp"

namespace btl {
    struct AutoActionParam {
        status::CharacterStatus *player_;
        int actionIndex_;
        int itemIndex_;
        int effectValue_;
        short targetEffectValue_[12];
        int targetCount_;
        short targetActionDefence_[12];
        short lastEffectValue_[12];
        short targetGroupEffectValue_[5];
        int maxGroup_;
        int maxIndex_;
        btl::BattleSelectTargetParam battleSelectTargetParam_;
        AutoActionParam();
        ~AutoActionParam();
        void setup(int actionIndex, int itemIndex, status::CharacterStatus* player);
        void clear();
        virtual void setupTarget();
        virtual void calcEffectValue();
        virtual void calcEffectValueException();
        void calcActionDefence();
        void calcTargetEffectValue();
        virtual void calcCorrectEffectVelue();
        virtual void calcCorrectEffectVelueException();
        void calcGroupValue();
        void calcAllValue();
        void calcTargetCount();
        void calcSelectGroup();
        void calcSelectIndex();
    };

}