#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"


namespace status{
    struct GameFlag
    {
        dss::BitFlaguint flag_[32];
        GameFlag();
        ~GameFlag();
        void set(unsigned int index);
        bool check(unsigned int index);
        void clear();
        void remove(unsigned int index);
    };
}
extern status::GameFlag g_AreaFlag; 
extern status::GameFlag g_LocalFlag;
extern status::GameFlag g_GlobalFlag;

