#pragma once
#include <globaldefs.h>
#include "main/effect/UnkScreenEffect.hpp"
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
    void unkfunc_0202ace4();
    void unkfunc_0202ad28();
    void unkfunc_0202ad98();
    void unkfunc_0202adb4();
    void unkfunc_0202aea4();
    void unkfunc_0202aec4(int type);
    bool unkfunc_0202af54();
    void unkfunc_0202af70();
    void unkfunc_0202afcc();
};
