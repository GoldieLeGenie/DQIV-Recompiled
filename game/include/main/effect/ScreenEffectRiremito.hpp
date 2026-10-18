#pragma once
#include "main/effect/ScreenEffect.hpp"

// one big quad that falls back while turning (ScreenEffectManager type 2)
struct UnkScreenEffect_0202ce0c : ScreenEffect {
    int depth_;                                 // 0x20
    int offsetY_;                                 // 0x24
    int angle_;                                 // 0x28
    int quadScale_;                                 // 0x2C
    int unk_30;                                 // 0x30

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
};

// ripples on the captured screen (ScreenEffectManager type 5, Ranaruta)
struct ScreenEffectRanaruta : ScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int waveSpeed_;                                 // 0x28
    int waveZ_;                                 // 0x2C
    char unk_30[0x54 - 0x30];
    short waveTiles_[10];                           // 0x54
    int isNight_;                                 // 0x68

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void setSharedVertexZ(int index, int vertex, short z);
};

// ScreenEffectManager type 6 (Riremito)
struct ScreenEffectRiremito : ScreenEffect {
    int unk_20;                                 // 0x20
    int angle_;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    int unk_30[8];                              // 0x30
    int frame_;                                 // 0x50
    int unk_54;                                 // 0x54
    int unk_58;                                 // 0x58
    int unk_5c;                                 // 0x5C

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_0202c6e8();
};
