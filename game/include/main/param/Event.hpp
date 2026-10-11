#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/data/ExcelBinaryData.hpp"


namespace param {
    struct Event
    {
        unsigned short index;
        unsigned short gold;
        unsigned short weapon;
        unsigned short armor;
        unsigned short shield;
        unsigned short helmet;
        unsigned short accessory;
        unsigned char party;
        unsigned char level;
        unsigned char NPC1;
        unsigned char NPC2;
        unsigned char NPC3;
        char caption[128];
        char floor[8];
        char byte_1;
        static param::Event* getFileData(unsigned int id);
        static DataObject data_;
    };
   
}




