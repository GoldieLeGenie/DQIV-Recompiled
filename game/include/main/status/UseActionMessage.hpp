#pragma once 
#include <globaldefs.h>
#include "GameInfo.hpp"

namespace status
{
    struct UseActionMessage {
        int beforeMessage_[2];
        int execMessage_[4];
        int spclMessage_[2];
        int resultMessage_[2];
        int addMessage_[2];
        UseActionMessage() { clear(); };
        void clear();
    };
} // namespace status
