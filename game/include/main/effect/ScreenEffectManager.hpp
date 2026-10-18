#pragma once
#include <globaldefs.h>
#include "main/effect/ScreenEffect.hpp"
#include "main/effect/UnkScreenEffect_0202c678.hpp"
#include "main/effect/UnkScreenEffect_0202ff7c.hpp"

// runs the screen transition effects (mobile: Global::SetScreenEffect / IsScreenEffectEnd)
struct UnkScreenEffectManager {
    int type_;                                  // 0x00
    int step_;                                  // 0x04
    int fadeIn_;                                // 0x08
    UnkScreenEffect* current_;                  // 0x0C
    UnkScreenEffect base_;                      // 0x10
    UnkScreenEffect_0202ce0c effect2_;          // 0x30
    UnkScreenEffect_0202c9b8 effect5_;          // 0x64
    UnkScreenEffect_0202c678 effect6_;          // 0xD0
    UnkScreenEffect_0202ff7c effect3_;          // 0x130
    UnkScreenEffect_020302a0 effect4_;          // 0x160

    static int unk_020edc40;

    static UnkScreenEffectManager* getSingleton();
    void initialize();
    void execute();
    void draw();
    void terminate();
    void stopEffect();
    void startEffect(int type);
    bool isEffectDrawing();
    void startWhiteFlash();
    void updateWhiteFlash();
};
