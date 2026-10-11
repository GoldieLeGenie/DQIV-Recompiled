#pragma once
#include <globaldefs.h>

namespace status{
    struct ItemData {
        unsigned char index_;
        unsigned char equip_;
        unsigned short count_;
        ItemData();
        ~ItemData();
        void clear();

    };
}