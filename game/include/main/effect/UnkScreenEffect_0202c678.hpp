#pragma once
#include "main/effect/UnkScreenEffect.hpp"

// one big quad that falls back while turning (UnkScreenEffectManager type 2)
struct UnkScreenEffect_0202ce0c : UnkScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    int unk_30;                                 // 0x30

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
};

// ripples on the captured screen (UnkScreenEffectManager type 5, Ranaruta)
struct UnkScreenEffect_0202c9b8 : UnkScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    char unk_30[0x54 - 0x30];
    short unk_54[10];                           // 0x54
    int unk_68;                                 // 0x68

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_0202cd34(int index, int vertex, short z);
};

// UnkScreenEffectManager type 6 (Riremito)
struct UnkScreenEffect_0202c678 : UnkScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    int unk_30[8];                              // 0x30
    int unk_50;                                 // 0x50
    int unk_54;                                 // 0x54
    int unk_58;                                 // 0x58
    int unk_5c;                                 // 0x5C

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_0202c6e8();
};
