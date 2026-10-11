#pragma once
#include "main/dss/DssUtils.hpp"
#include <globaldefs.h>
#include "main/data/DataObject.hpp"


namespace param {
    struct FloorParam {
        unsigned char bgmDaytime;
        unsigned char bgmNight;
        char floor[8];
        char battlefloor[11];
        char eventfloor[12];
        char byte_1;
        char byte_2;
        char byte_3;
        char byte_4;
        char byte_5;
        char byte_6;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
        static int getFloorIndex(param::FloorParam* data, char* name);
        static DataObject data_;
    };
}

