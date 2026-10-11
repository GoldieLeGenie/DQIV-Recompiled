#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "ov001/window/UnkFieldMap.hpp"

struct FieldSystem;

struct FieldImageMap : window::ImageMap {
    FieldSystem* system_;                       // 0x004
    UnkFieldMapSprite_0212c884 unk_008;         // 0x008
    UnkFieldMap_0212cff8 unk_0ac;               // 0x0AC
    UnkFieldMap_0212d558 unk_24c;               // 0x24C
    UnkFieldMap_0212d3ec unk_3a0;               // 0x3A0
    window::UnkMapBase_020381d0* map_;          // 0x4F4
    int alpha_;                                 // 0x4F8
    int phase_;                                 // 0x4FC

    FieldImageMap();
    ~FieldImageMap();
    void setup(FieldSystem* system);
    virtual void cleanup();
    void execute();
    void draw();
    void unkfunc_0212aa14();
    void unkfunc_0212aa20();
    virtual void open();
    virtual void close();
    virtual int isOpen();
    virtual int isClose();
    void clearAllMap();
};
