#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/AutoActionParam.hpp"
#include "main/status/UseAction.hpp"

namespace btl {
    struct RecoveryAutoActionParam : btl::AutoActionParam
    {
        RecoveryAutoActionParam();
        ~RecoveryAutoActionParam();
        virtual void setupTarget();
        virtual void calcEffectValue();
        virtual void calcEffectValueException();
        virtual void calcCorrectEffectVelue();
        virtual void calcCorrectEffectVelueException();
        int isMemberHpDown(int percent);
        int isMemberDeath();
        int getMemberSpazz();
    };
     
}