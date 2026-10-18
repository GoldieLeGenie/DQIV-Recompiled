#pragma once
#include <globaldefs.h>
#include "main/dss/CsvData.hpp"

// Group/map list of "data/btlmap/_list.txt" used by the battle menu debug part
struct BattleMapList : CsvData {
    int group_;                                 // 0x20  first row of the current group
    int map_;                                   // 0x24  row in the group
    int unk_28;                                 // 0x28
    int loaded_;                                // 0x2C

    BattleMapList();
    ~BattleMapList();
    void load(const char* filename);    // load
    int isLoaded();                         // loaded_
    char* getGroupName();                       // group name
    void nextGroup();                        // next group
    void prevGroup();                        // previous group
    char* getMapName();                       // map name
    void nextMap();                        // next map
    void prevMap();                        // previous map
};

extern BattleMapList g_battleMapList;
