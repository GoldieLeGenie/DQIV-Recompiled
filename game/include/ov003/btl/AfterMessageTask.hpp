#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/task/PartTask.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"

namespace btl {
    struct AfterMessageTask : task::PartTask
    {
        status::UseActionParam *useActionParam_;
        int message_;
        void setup(status::UseActionParam *useActionParam);
        void cleanup();
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };   
}
