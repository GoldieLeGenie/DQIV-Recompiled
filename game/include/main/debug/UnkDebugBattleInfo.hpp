#pragma once
#include <globaldefs.h>
#include "main/debug/UnkDebugDisplay.hpp"
#include "main/status/HaveStatusInfo.hpp"

// Debug battle page: status of one player or monster
struct UnkDebugBattleInfo : UnkDebugDisplay {
    // vtable                                   // 0x00
    int type_;                                  // 0x04  1: player, 2: monster
    int index_;                                 // 0x08
    int blink_;                                 // 0x0C  frames the target arrow blinks
    int showHeap_;                              // 0x10

    virtual void initialize();
    virtual void draw();
    void unkfunc_0203d224(int x, int y);        // heap
    void unkfunc_0203d264(int x, int y, status::HaveStatusInfo* info);
};

extern UnkDebugBattleInfo data_020f2008;
