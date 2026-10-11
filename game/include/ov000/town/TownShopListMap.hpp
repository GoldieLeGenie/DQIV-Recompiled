#pragma once
#include "globaldefs.h"
#include "main/window/ImageMap.hpp"
#include "main/dss/UnkSprite2D.hpp"

/* vtable 0x02147cc4 */
struct TownShopListMap : window::ImageMap {
    UnkMenuSprite shopSprite_;                  // 0x04
    UnkMenuSprite blackSprite_;                 // 0x54
    int isEnable_;                              // 0xA4
    int frame_;                                 // 0xA8
    int phase_;                                 // 0xAC

    TownShopListMap();
    ~TownShopListMap();
    void initialize();
    virtual void cleanup();
    void execute();
    void checkData();
    void draw();
    virtual void open();
    virtual void openBlack();
    virtual void close();
    virtual int isOpen();
    virtual int isClose();
    virtual int isEnable();
};
