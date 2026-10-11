#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"

namespace btl {
    struct BattleActorEffect
    {
        static int wait_; //data_ov003_02158a50
        static void setExecEffect(status::UseActionParam* useActionParam);
        static int checkCommonExecEffect(status::UseActionParam* useActionParam);
        static int setPlayerEffect(status::UseActionParam* useActionParam);
        static int checkPlayerExecEffect(status::UseActionParam* useActionParam);
        static int setEnemyEffect(status::UseActionParam* useActionParam);
        static int checkEnemyExecEffect(status::UseActionParam* useActionParam);
        static int setResultEnemyEffect(status::UseActionParam* useActionParam);
        static int checkEnemyResultEffect(status::UseActionParam* useActionParam);
        static int setMegazaruEffect(status::UseActionParam* useActionParam);
    };

}
