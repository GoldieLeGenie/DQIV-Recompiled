#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/data/ExcelBinaryData.hpp"

namespace status{
    struct Status {
        static int flagShopIndex_;          // data_020bc80c
        static void initialize();
        static void initialize_character();
        static void setEventParty(unsigned int index);
        static void setEventFlag(unsigned int index);
        static void setFlagShopIndex(int index);
        static void setFlagShopExec();
    };
}

