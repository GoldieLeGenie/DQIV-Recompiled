#pragma once
#include <globaldefs.h>
#include "ov009/casino/CasinoMiniGameBase.hpp"
#include "main/dss/UnkSprite2D.hpp"

struct CasinoPoker : CasinoMiniGameBase {
    UnkMenuSprite upperSprite_;                 // 0x08 (DS-only)

    static char* stagePoker;

    CasinoPoker();
    ~CasinoPoker();
    static CasinoPoker* getSingleton();
    virtual void initialize();
    virtual void terminate();
    virtual void execute();
    virtual void draw();
    virtual char* getStageName();
};
