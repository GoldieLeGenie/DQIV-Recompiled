#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

// one tile of the captured screen, drawn as a textured quad
struct UnkScreenEffectTile {
    int x;                                      // 0x00
    int y;                                      // 0x04
    int z;                                      // 0x08
    dss::Vector3short vertex[4];                // 0x0C
    int texCoord[4][2];                         // 0x24
};

// screen transition effect: the captured screen is cut into columns_ x rows_ tiles
struct UnkScreenEffect {
    // vtable                                   // 0x00
    unsigned char alpha_;                       // 0x04
    dss::Fix32 scale_;                          // 0x08
    int columns_;                               // 0x0C
    int rows_;                                  // 0x10
    int counter_;                               // 0x14
    int wait_;                                  // 0x18
    dss::BitFlag<unsigned char> flag_;          // 0x1C

    static UnkScreenEffectTile* tiles_;

    UnkScreenEffect();
    ~UnkScreenEffect();
    virtual void start();
    virtual void draw();
    virtual bool isEnd() { return counter_ > 60; }
    int unkfunc_0202b244();
    void unkfunc_0202b474();
    void unkfunc_0202b498();
    void unkfunc_0202b510(int polygonID);
};
