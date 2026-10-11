#pragma once
#include "main/btl/BattleAutoFeed.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/text/TextAPI.hpp"
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/AutoActionParam.hpp"
#include "main/status/UseAction.hpp"

namespace btl {
    struct BattleMessage
    {
        static void setMessage(int message1, int message2, int message3, int message4);
        static void setMessageInTown(int message1, int message2, int message3, int message4);
        static int setBeforeMessage(status::UseActionParam* useActionParam);
        static int setExecMessage(status::UseActionParam* useActionParam);
        static int setSpecialMessage(status::UseActionParam* useActionParam, int currentTarget);
        static int setResultMessage(status::UseActionParam* useActionParam, int currentTarget);
        static int setAfterMessage(status::UseActionParam* useActionParam, int index);
        static void setShakeMessage(status::UseActionParam* useActionParam, int currentTarget);
        static void openEncountMessage();
        static void addEncountMessage(int message);

    };
    
}
