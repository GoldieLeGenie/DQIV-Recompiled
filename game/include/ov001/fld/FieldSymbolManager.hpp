#pragma once
#include "globaldefs.h"
#include "main/param/Param.hpp"

// symbols (towns, caves...) shown on the field map
struct FieldSymbolManager {
    struct SmallMapSymbol {
        unsigned char type;
        unsigned char x;
        unsigned char y;
    };

    static const int KANBAN_MESSAGE = 0xc488a;
    static const int FAR_DISTANCE = 32;
    static const int NEAR_DISTANCE_X = 7;
    static const int NEAR_DISTANCE_Y = 5;

    param::FieldSymbol* symbol_;                // 0x000
    int index_;                                 // 0x004
    SmallMapSymbol small_[114];                 // 0x008
    int walkX_;                                 // 0x160
    int walkY_;                                 // 0x164
    int resetFlag_;                             // 0x168

    FieldSymbolManager();
    ~FieldSymbolManager();
    static FieldSymbolManager* getSingleton();
    void initialize();
    void terminate();
    bool checkSymbol(int uid);
    bool searchSymbol(int& walkX, int& walkY);
};
