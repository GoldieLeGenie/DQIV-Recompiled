#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/param/Param.hpp"
#include "main/task/PartTask.hpp"
#include "main/task/PartTaskManager.hpp"
#include "ov003/btl/BattleEffectUnit.hpp"
#include "ov003/btl/BattleEffectManager.hpp"

namespace btl {
    struct SpecialMessageTask : task::PartTask
    {
        status::UseActionParam *useActionParam_;
        int message_;
        int counter_;
        void setup(status::UseActionParam *useActionParam);
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

}


extern int currentTarget_; //currentTarget_
extern int targetCount_; //targetCount_ 


