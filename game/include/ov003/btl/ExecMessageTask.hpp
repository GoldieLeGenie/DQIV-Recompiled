#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "ov003/btl/BattleActorEffect.hpp"

namespace btl{
    struct ExecMessageTask : task::PartTask {
        status::UseActionParam *useActionParam_;
        int message_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };
    
}
