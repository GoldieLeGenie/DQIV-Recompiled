#pragma once
#include "main/menu/MenuAPI.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/task/PartTask.hpp"
#include "main/task/PartTaskManager.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/task/ExecTask.hpp"

namespace btl {
    // declaration order = reverse of the vtable order in .data
    struct EncountTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct StatusTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct FirstReorderTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct StadiumResultTask : task::PartTask {
        int messageCount_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct StadiumDrawTask : task::PartTask {
        int messageCount_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct StadiumEndTask : task::PartTask {
        int messageCount_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct EventTask2 : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct EventTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct BattleEndTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct TimeReverseEndTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct TimeReverseTask : task::PartTask {
        int counter_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct CrusingEndTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct CrusingTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct DemolitionTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct PartyReorderTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct ExitWaitTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct EscapeTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct ExitTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct RoundEndTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct RoundTask : task::PartTask {
        int waitFlag_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct CommandTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct FirstAttackTask : task::PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

}


#include "ov015/btl/BattleMenu.hpp"

extern "C" {
}
