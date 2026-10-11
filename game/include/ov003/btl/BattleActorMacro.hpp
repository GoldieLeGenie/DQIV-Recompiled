#pragma once
#include <globaldefs.h>
#include "main/status/CharacterStatus.hpp"
#include "main/status/UseActionParam.hpp"

namespace btl {
    struct BattleActorMacro {
        static void setExecMacro(status::UseActionParam& useActionParam);
        static void setMacroActor(status::CharacterStatus* actor, int actionIndex);
        static void setMacroTarget(status::CharacterStatus* target, int actionIndex, int targetIndex);
        static void setResultMacro(status::UseActionParam& useActionParam, int targetIndex);
        static void setAddMacro(status::UseActionParam& useActionParam, int targetIndex);
    };
}
