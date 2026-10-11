#pragma once
#include <globaldefs.h>
#include "main/dss/Render.hpp"

struct CasinoMiniGameBase {
    // vtable                                   // 0x00
    Render* render_;                            // 0x04 (DS-only, set by CasinoSystem)

    virtual void initialize() = 0;
    virtual void terminate() = 0;
    virtual void execute() = 0;
    virtual void draw() = 0;
    virtual char* getStageName() = 0;
};
