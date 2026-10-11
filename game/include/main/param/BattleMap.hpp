#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/dss/DssUtils.hpp"


namespace param{
    struct BattleMap {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        char map[13];
        static int getBattleMap(param::BattleMap *data, char *name);
        static DataObject data_;
    };
}



extern char btl_[8];
extern char btldougu[12];
extern char btlyado[8];

