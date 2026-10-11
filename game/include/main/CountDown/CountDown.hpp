#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"

struct CountDown {
    int counter_;
    int correctValue_;
    int tileIdRate_;
    int landType_;
    int subtractValue_;
    int fix_;
    CountDown();
    ~CountDown();
    void setup();
    void exec();
    void setTileIdRate(int title);
    void setLandType(LandType land);
    void setLandSubtranctValue();
};

extern int LandSubtractValue[];//data_020bc19c
extern int TileSubtractValue[];//data_020bc1bc
extern int CorrectValue[];//data_020bc1dc


