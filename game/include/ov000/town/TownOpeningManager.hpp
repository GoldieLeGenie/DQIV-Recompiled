#pragma once
#include "globaldefs.h"
#include "main/data/DataObject.hpp"
#include "main/dss/TextureObject.hpp"
#include "main/object/DSSAObject.hpp"

// town opening: title logo and staff names drawn over the opening scene
struct TownOpeningManager {
    LZDataObject m_tex_data;                    // 0x000 logo texture file
    LZDataObject m_staff_tex_data;              // 0x010 staff texture file
    TextureObject m_tex;                        // 0x020
    TextureObject m_staff_tex;                  // 0x090
    DataObject m_dssa_data[4];                  // 0x100 logo1..4.dssa
    DataObject m_staff_dssa_data;               // 0x140 staff.dssa
    UnkDSSAObject m_dssa[4];                    // 0x150
    DSSAObjectWithCamera m_dssa_camera;         // 0x3C0
    UnkDSSAObject m_staff_dssa;                 // 0x460
    int m_enable;                               // 0x4FC
    int m_effect;                               // 0x500
    int m_effectCounter;                        // 0x504
    int m_counter;                              // 0x508
    int unk_50c;                                // 0x50C

    TownOpeningManager();
    ~TownOpeningManager();
    static TownOpeningManager* getSingleton();
    void setup();
    void cleanup();
    void draw();
    void execute();
};
