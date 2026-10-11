#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"

namespace btl {
    struct EndTask2 : task::PartTask
    {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };
}