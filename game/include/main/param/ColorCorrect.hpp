#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"


namespace param {
    struct ColorCorrect {
        unsigned char backcolor;
        char floor[8];
        char byte_1;
        char byte_2;
        unsigned char dmmy0;
        static int getCorrectIndex(param::ColorCorrect *data, char *name);
        static DataObject data_;
    };   
}

