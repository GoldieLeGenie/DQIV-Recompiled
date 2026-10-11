#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/AutoActionParam.hpp"
#include "main/status/UseAction.hpp"

namespace btl { 
    struct DefenceAutoActionParam : btl::AutoActionParam
    {
        DefenceAutoActionParam();
        ~DefenceAutoActionParam();
        virtual void setupTarget();
        virtual void calcEffectValue();
        virtual void calcEffectValueException();
        virtual void calcCorrectEffectVelue();
    };
    
}