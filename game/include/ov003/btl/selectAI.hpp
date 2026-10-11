#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/btl/AutoAction.hpp"

namespace btl {
    void selectAI(status::UseActionParam* useActionParam);

}

extern btl::AutoAction autoAction; //data_ov003_02149350

