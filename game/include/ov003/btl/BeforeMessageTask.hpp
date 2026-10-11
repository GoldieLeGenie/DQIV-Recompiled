#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"
#include "main/task/PartTaskManager.hpp"
#include "main/status/UseActionMacro.hpp"

namespace btl {
    struct BeforeMessageTask : task::PartTask {
        status::UseActionParam *useActionParam_;
        int message_;
        void setup(status::UseActionParam *useActionParam);
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };
}
