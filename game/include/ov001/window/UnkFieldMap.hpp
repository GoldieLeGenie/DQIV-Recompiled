#pragma once
#include "globaldefs.h"
#include "main/window/GlobalMap.hpp"

struct FieldSystem;

// field map classes of ov001 (0x0212c884-0x0212d5f0), code not decompiled yet

// sprites shown by FieldImageMap once the world map is closed
struct FieldLargeMap {
    FieldSystem* system_;                       // 0x00
    MenuSprite unk_04;                       // 0x04
    MenuSprite unk_54;                       // 0x54

    FieldLargeMap();
    ~FieldLargeMap();
    void setup(FieldSystem* system);
    void cleanup();
    void initSprites();
    int loadSpecialBlock(int x, int y, void* buffer);
    void loadMapBlocks(int x, int y);
    void load();
    void clear();
    void setAlpha(unsigned char alpha);
};

// world map of the normal field
struct FieldGlobalMap : window::GlobalMap {
    MenuSprite unk_150;                      // 0x150

    FieldGlobalMap();
    ~FieldGlobalMap();
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
struct FieldSubWorldMap : window::WorldMapBase {
    MenuSprite unk_100;                      // 0x100 player marker
    int unk_150;                                // 0x150 world passed to drawSymbols

    FieldSubWorldMap();
    ~FieldSubWorldMap();
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
struct FieldSubWorldMap_YW : FieldSubWorldMap {
    FieldSubWorldMap_YW();
    ~FieldSubWorldMap_YW();
    virtual void setup();
};

// map of field type 1
struct FieldSubWorldMap_GW : FieldSubWorldMap {
    FieldSubWorldMap_GW();
    ~FieldSubWorldMap_GW();
    virtual void setup();
};
