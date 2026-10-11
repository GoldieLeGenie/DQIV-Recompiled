#pragma once
#include "globaldefs.h"
#include "main/window/GlobalMap.hpp"

struct FieldSystem;

// field map classes of ov001 (0x0212c884-0x0212d5f0), code not decompiled yet

// sprites shown by FieldImageMap once the world map is closed
struct UnkFieldMapSprite_0212c884 {
    FieldSystem* system_;                       // 0x00
    UnkMenuSprite unk_04;                       // 0x04
    UnkMenuSprite unk_54;                       // 0x54

    UnkFieldMapSprite_0212c884();
    ~UnkFieldMapSprite_0212c884();
    void unkfunc_0212c8c4(FieldSystem* system);
    void unkfunc_0212c8d4();
    void unkfunc_0212c8f8();
    int unkfunc_0212c9e4(int x, int y, void* buffer);
    void unkfunc_0212cd58(int x, int y);
    void unkfunc_0212cee0();
    void unkfunc_0212cfb8();
    void unkfunc_0212cfd4(unsigned char alpha);
};

// world map of the normal field
struct UnkFieldMap_0212cff8 : window::GlobalMap {
    UnkMenuSprite unk_150;                      // 0x150

    UnkFieldMap_0212cff8();
    ~UnkFieldMap_0212cff8();
    virtual void setup(Render* render);
    virtual void setup();
    virtual void cleanup();
    virtual void draw();
    virtual void load();
    virtual void clear();
    virtual void playerMapPosition();
    virtual void setAlpha(unsigned char alpha);
};

// base of the maps of the other field types
struct UnkFieldMap_0212d214 : window::UnkMapBase_020381d0 {
    UnkMenuSprite unk_100;                      // 0x100 player marker
    int unk_150;                                // 0x150 world passed to unkfunc_0203822c

    UnkFieldMap_0212d214();
    ~UnkFieldMap_0212d214();
    virtual void setup(Render* render);
    virtual void setup() = 0;
    virtual void cleanup();
    virtual void draw();
    virtual void load();
    virtual void clear();
    virtual void setAlpha(unsigned char alpha);
    virtual void playerMapPosition();
    static dss::Vector2<int> convertMapPos(int x, int y);
};

// map of field type 2
struct UnkFieldMap_0212d3ec : UnkFieldMap_0212d214 {
    UnkFieldMap_0212d3ec();
    ~UnkFieldMap_0212d3ec();
    virtual void setup();
};

// map of field type 1
struct UnkFieldMap_0212d558 : UnkFieldMap_0212d214 {
    UnkFieldMap_0212d558();
    ~UnkFieldMap_0212d558();
    virtual void setup();
};
