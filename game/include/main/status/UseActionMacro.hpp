#pragma once
#include "main/text/TextAPI.hpp"
#include <globaldefs.h>
#include "main/status/CharacterStatus.hpp"

namespace status {
    struct UseActionMacro {
        static void setBeforeMacro(CharacterStatus* actor, int actionIndex);
        static void setExecMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex);
        static void setResultMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex);
        static void setAddMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex);
        static void setStatusChangeMacro(CharacterStatus* actor);
    };
}
