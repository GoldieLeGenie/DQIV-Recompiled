#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"


namespace param {
    struct MonsterMap {
        unsigned short tileID;
        unsigned char section;
        char floorID[8];
        unsigned char dmmy0;
        static int getFloorIndex(param::MonsterMap* data, int section, const char* name);
        static DataObject data_;
    };
}

