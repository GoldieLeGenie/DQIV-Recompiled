#pragma once
#include "main/effect/ScreenEffect.hpp"

// ScreenEffectManager type 4 (battle)
struct ScreenEffectBattle : ScreenEffect {
    int unk_20;                                 // 0x20
    int unk_24;                                 // 0x24
    int unk_28;                                 // 0x28
    int unk_2c;                                 // 0x2C
    int angle0_;                                 // 0x30
    int angle1_;                                 // 0x34

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void unkfunc_020302e0(short column, short row, short index, short* sinValue, short* cosValue);
};

// ScreenEffectManager type 3 (travel door)
struct ScreenEffectTravelDoor : ScreenEffect {
    int reverse_;                                 // 0x20
    int frame_;                                 // 0x24
    int angle_;                                 // 0x28
    int angleSpeed_;                                 // 0x2C

    virtual void start();
    virtual void draw();
    virtual bool isEnd();
    void setReverse(int flag);
};
