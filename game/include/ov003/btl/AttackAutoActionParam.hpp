#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/AutoActionParam.hpp"
#include "main/status/UseAction.hpp"

namespace btl { 
    struct AttackAutoActionParam : btl::AutoActionParam
    {
        AttackAutoActionParam();
        ~AttackAutoActionParam();
        virtual void setupTarget();
        virtual void calcEffectValue();
        virtual void calcEffectValueException();
        virtual void calcCorrectEffectVelue();
        virtual void calcCorrectEffectVelueException();
        void calcCorrectEffectVelueMahokanta();
        void calcCorrectEffectVelueMahosute();
    };
}



