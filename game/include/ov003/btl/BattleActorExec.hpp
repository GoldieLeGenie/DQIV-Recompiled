#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/task/PartTask.hpp"
#include "ov003/btl/BeforeActionTask.hpp"
#include "ov003/btl/BeforeMessageTask.hpp"
#include "ov003/btl/ExecMessageTask.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"
#include "ov003/btl/ResultMessageTask.hpp"
#include "ov003/btl/AfterMessageTask.hpp"
#include "ov003/btl/AfterActionTask.hpp"
#include "ov003/btl/EndActionTask.hpp"
#include "ov003/btl/EndTask2.hpp"
#include "ov003/btl/ExecActionTask.hpp"

namespace btl {
    struct BattleActorExec
    {
        status::UseActionParam *useActionParam_;
        BeforeActionTask beforeActionTask_;
        ExecActionTask execActionTask_;
        BeforeMessageTask beforeMessageTask_;
        ExecMessageTask execMessageTask_;
        SpecialMessageTask specialMessageTask_;
        ResultMessageTask resultMessageTask_;
        AfterMessageTask afterMessageTask_;
        AfterActionTask afterActionTask_;
        EndActionTask endActionTask_;
        EndTask2 endTask_;
        BattleActorExec();
        ~BattleActorExec();
        void setup(status::UseActionParam* param);
        void execute();
        int isEnd();
        bool isActionEnd();
    };

}

