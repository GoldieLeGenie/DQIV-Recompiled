#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"

namespace btl {
    struct EndActionTask : task::PartTask
    {
        status::UseActionParam *useActionParam_;
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };
}