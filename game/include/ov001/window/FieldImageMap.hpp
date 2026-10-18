#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "ov001/window/UnkFieldMap.hpp"

struct FieldSystem;

struct FieldImageMap : window::ImageMap {
    FieldSystem* system_;                       // 0x004
    FieldLargeMap unk_008;         // 0x008
    FieldGlobalMap unk_0ac;               // 0x0AC
    FieldSubWorldMap_GW unk_24c;               // 0x24C
    FieldSubWorldMap_YW unk_3a0;               // 0x3A0
    window::WorldMapBase* map_;          // 0x4F4
    int alpha_;                                 // 0x4F8
    int phase_;                                 // 0x4FC

    FieldImageMap();
    ~FieldImageMap();
    void setup(FieldSystem* system);
    virtual void cleanup();
    void execute();
    void draw();
    void startClose();
    void startOpen();
    virtual void open();
    virtual void close();
    virtual int isOpen();
    virtual int isClose();
    void clearAllMap();
};
