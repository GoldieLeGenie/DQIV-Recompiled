#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "main/window/GlobalMap.hpp"

struct TownSystem;

struct UnkImageMap_02141f6c : window::ImageMap {
    window::GlobalMap window_;                  // 0x004
    int isEnable_;                              // 0x154
    int frame_;                                 // 0x158
    int phase_;                                 // 0x15C
    int unk_160;                                // 0x160

    UnkImageMap_02141f6c();
    ~UnkImageMap_02141f6c();
    void setup(TownSystem* system);
    virtual void cleanup();
    void execute();
    void draw();
    virtual void open();
    virtual void close();
    virtual int isOpen();
    virtual int isClose();
    virtual int isEnable();
};
