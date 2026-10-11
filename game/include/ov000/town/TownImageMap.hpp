#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkSprite2D.hpp"

/* vtable 0x021483b8 */
struct TownImageMap : window::ImageMap {
    dss::Vector2<int> unk_04;                   // 0x04
    UnkMenuSprite mapSprite_;                   // 0x0C
    UnkMenuSprite pointSprite_;                 // 0x5C
    dss::Fix32Vector3 unk_ac;                   // 0xAC
    int isEnable_;                              // 0xB8
    int frame_;                                 // 0xBC
    int phase_;                                 // 0xC0
    int index_;                                 // 0xC4

    TownImageMap();
    ~TownImageMap();
    void setup();
    virtual void cleanup();
    void execute();
    void exitFloor();
    void checkData();
    void draw();
    virtual void open();
    virtual void close();
    virtual int isOpen();
    virtual int isClose();
    void calcTargetPos();
    void unkfunc_02137c00();
    void initialize();
    virtual int isEnable() { return isEnable_; }
};
