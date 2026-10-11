#pragma once
#include "main/effect/UnkScreenEffect.hpp"

// UnkScreenEffectManager type 4 (battle)
struct UnkScreenEffect_020302a0 : UnkScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    int unk_30;                                 // 0x30
    int unk_34;                                 // 0x34

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_020302e0(short column, short row, short index, short* sinValue, short* cosValue);
};

// UnkScreenEffectManager type 3 (travel door)
struct UnkScreenEffect_0202ff7c : UnkScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_02030278(int flag);
};
