#pragma once
#include "globaldefs.h"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/global/Global.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/effect/UnkScreenEffectManager.hpp"

struct TownStageManager;
struct TownPlayerManager;
struct FieldPlayerManager;

namespace cmn
{
    enum ACTION_EFFECT {
        ACTION_NONE=0,
        ACTION_TRAVELDOOR=1,
        ACTION_RIREMITO=2,
        ACTION_RANARUTA=3,
        ACTION_BATTLE=4
    };
    struct NonBattleActionManager {
        ACTION_EFFECT status_;
        int startFlag_;
        int waitTurn_;

        static NonBattleActionManager* getSingleton();
        void setAction(ACTION_EFFECT action);
        void execute();
    };

    extern NonBattleActionManager g_NonBattleActionManager;    // data_020ef9c8
}


extern char data_020c1328[8];
