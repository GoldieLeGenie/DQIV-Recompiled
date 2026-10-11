#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"

struct Param {
    int ctrlId_;                                // 0x00
    int index_;                                 // 0x04
    int dir_;                                   // 0x08
    int x_;                                     // 0x0C
    int y_;                                     // 0x10
    int z_;                                     // 0x14
};

struct PlacementParameter {
    DataObject dataObject_;                     // 0x00
    Param param_;                               // 0x10

    PlacementParameter();
    ~PlacementParameter();
    void setup(void* addr);
    void cleanup();
    int execute();
    static void setStartFunction(int (*fc)(void*));

    static int (*startFunction_)(void*);
};
