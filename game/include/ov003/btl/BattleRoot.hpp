#pragma once
#include <globaldefs.h>
#include "main/dss/DssCore.hpp"
#include "GameInfo.hpp"
#include "ov003/btl/BattleRootTask.hpp"
#include "ov003/btl/BattleRound.hpp"
#include "main/status/PartyStatus.hpp"

namespace btl {
    struct BattleRoot {
        EncountTask encountTask_;                   // 0x00
        StatusTask statusTask_;                     // 0x04
        FirstAttackTask firstAttackTask_;           // 0x08
        FirstReorderTask firstReorderTask_;         // 0x0C
        CommandTask commandTask_;                   // 0x10
        RoundTask roundTask_;                       // 0x14
        RoundEndTask roundEndTask_;                 // 0x1C
        BattleEndTask battleEndTask_;               // 0x20
        ExitTask exitTask_;                         // 0x24
        ExitWaitTask exitWaitTask_;                 // 0x28
        PartyReorderTask partyReorderTask_;         // 0x2C
        DemolitionTask demolitionTask_;             // 0x30
        CrusingTask crusingTask_;                   // 0x34
        CrusingEndTask crusingEndTask_;             // 0x38
        TimeReverseTask timeReverseTask_;           // 0x3C
        TimeReverseEndTask timeReverseEndTask_;     // 0x44
        EscapeTask escapeTask_;                     // 0x48
        EventTask eventTask_;                       // 0x4C
        EventTask2 eventTask2_;                     // 0x50
        StadiumEndTask stadiumEndTask_;             // 0x54
        StadiumDrawTask stadiumDrawTask_;           // 0x5C
        StadiumResultTask stadiumResultTask_;       // 0x64
        int eventEncount_;                          // 0x6C
        status::PartyStatus* backupPartyStatus_;    // 0x70
        status::PlayerStatus* backupPlayerStatus_;  // 0x74
        status::PlayerFlag* backupPlayerFlag_;      // 0x78

        BattleRoot();
        ~BattleRoot();
        static BattleRoot* getSingleton();
        void initialize();
        void terminate();
        void execute();
        void draw();
        void setupBattle();
        void cleanupBattle();
        void setupMonster();
        void cleanupMonster();
        void setupCrusingMenu();
        void cleanupCrusingMenu();
        void store();
        void restore();
    };
}

extern btl::BattleRound battleRound_;

