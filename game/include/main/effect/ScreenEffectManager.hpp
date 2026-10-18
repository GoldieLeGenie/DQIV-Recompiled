#pragma once
#include <globaldefs.h>
#include "main/effect/ScreenEffect.hpp"
#include "main/effect/ScreenEffectRiremito.hpp"
#include "main/effect/ScreenEffectTravelDoor.hpp"

// runs the screen transition effects (mobile: Global::SetScreenEffect / IsScreenEffectEnd)
struct ScreenEffectManager {
    int type_;                                  // 0x00
    int step_;                                  // 0x04
    int fadeIn_;                                // 0x08
    ScreenEffect* current_;                  // 0x0C
    ScreenEffect base_;                      // 0x10
    UnkScreenEffect_0202ce0c effect2_;          // 0x30
    ScreenEffectRanaruta effect5_;          // 0x64
    ScreenEffectRiremito effect6_;          // 0xD0
    ScreenEffectTravelDoor effect3_;          // 0x130
    ScreenEffectBattle effect4_;          // 0x160

    static int unk_020edc40;

    static ScreenEffectManager* getSingleton();
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
