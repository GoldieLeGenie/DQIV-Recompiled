#pragma once
#include <globaldefs.h>
#include "main/dss/Render.hpp"
#include "ov009/casino/CasinoMiniGameBase.hpp"

struct CasinoSystem {
    CasinoMiniGameBase* minigame;               // 0x00
    Render render_;                             // 0x04 (DS-only)

    static void unkfunc_021227cc();
    CasinoSystem();
    ~CasinoSystem();
    static CasinoSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
};
