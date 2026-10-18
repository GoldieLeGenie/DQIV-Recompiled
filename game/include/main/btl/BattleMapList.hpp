#pragma once
#include <globaldefs.h>
#include "main/dss/CsvData.hpp"

// Group/map list of "data/btlmap/_list.txt" used by the battle menu debug part
struct UnkBattleMapList : CsvData {
    int group_;                                 // 0x20  first row of the current group
    int map_;                                   // 0x24  row in the group
    int unk_28;                                 // 0x28
    int loaded_;                                // 0x2C

    UnkBattleMapList();
    ~UnkBattleMapList();
    void unkfunc_02048e34(const char* filename);    // load
    int unkfunc_02048e4c();                         // loaded_
    char* unkfunc_02048e50();                       // group name
    void unkfunc_02048e60();                        // next group
    void unkfunc_02048ee0();                        // previous group
    char* unkfunc_02048f7c();                       // map name
    void unkfunc_02048f90();                        // next map
    void unkfunc_02048ff8();                        // previous map
};

extern UnkBattleMapList data_020c4fb4;
