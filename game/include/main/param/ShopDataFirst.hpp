#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"


namespace param{
    struct ShopDataFirst {
        unsigned short item;
        unsigned short price;
        static int getIndex(char *name);
        static DataObject data_;
    };
}