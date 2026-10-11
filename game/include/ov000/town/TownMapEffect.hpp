#pragma once
#include "globaldefs.h"
#include "main/dss/UnkSprite2D.hpp"

struct TownMapEffect {
    enum EFFECT_TYPE {
        EFFECT_TYPE_DREAM1 = 0,
        EFFECT_TYPE_DREAM2 = 1,
        EFFECT_TYPE_DEATHPISARO = 2,
        EFFECT_TYPE_EVILPRIEST = 3
    };

    UnkMenuSprite sprite0_;                     // 0x00
    UnkMenuSprite sprite1_;                     // 0x50
    int exist_;                                 // 0xA0
    EFFECT_TYPE type_;                          // 0xA4
    int m_enable;                               // 0xA8
    short x_;                                   // 0xAC
    short y_;                                   // 0xAE
    short width_;                               // 0xB0
    short height_;                              // 0xB2
    fx32 scaleX_;                               // 0xB4
    fx32 scaleY_;                               // 0xB8
    int loaded_;                                // 0xBC
    short red_;                                 // 0xC0
    short green_;                               // 0xC2
    short blue_;                                // 0xC4
    short m_counter;                            // 0xC6
    short m_base_counter;                       // 0xC8

    void setup(EFFECT_TYPE type);
    void execute();
    void draw();
    void setPisaroEvent(int count);
    void cleanup();
    void unkfunc_02142790();
    void unkfunc_02142850(fx32 scaleX);
    void unkfunc_02142854(fx32 scaleY);
};
