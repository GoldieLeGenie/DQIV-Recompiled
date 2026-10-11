#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"
#include "main/status/BaseActionStatus.hpp"

namespace btl { struct BattleActorManager2; }
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ExcelParam.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"
#include "ov003/btl/BattleSelectTargetParam.hpp"
#include "ov015/btl/BattleSelectTarget.hpp"

namespace btl {
    struct ExecActionTask : task::PartTask{
        status::UseActionParam *useActionParam_;
        virtual void initialize();
        bool checkCommonExec(status::UseActionParam* param);
        virtual void terminate();
        virtual void execute();
        void setupTorunekoAction();
    };
}
