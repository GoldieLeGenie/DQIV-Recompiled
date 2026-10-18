#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "ov001/window/UnkFieldMap.hpp"

struct FieldSystem;

struct FieldImageMap : window::ImageMap {
    FieldSystem* system_;                       // 0x004
    FieldLargeMap largeMap_;         // 0x008
    FieldGlobalMap globalMap_;               // 0x0AC
    FieldSubWorldMap_GW subWorldMapGW_;               // 0x24C
    FieldSubWorldMap_YW subWorldMapYW_;               // 0x3A0
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
