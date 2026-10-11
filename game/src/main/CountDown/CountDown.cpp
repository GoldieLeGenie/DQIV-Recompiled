#include "main/CountDown/CountDown.hpp"
#include "main/dss/Random.hpp"

int TileSubtractValue[8] = {  // data_020bc1bc
    0x60, 0xA0, 0xC0, 0x100, 0x1E0, 0x240, 0x2A0, 0x300,
};
int LandSubtractValue[8] = {  // data_020bc19c
    0x161, 0x66, 0x180, 0x280, 0x1CD, 0x100, 0x100, 0x133,
};

int CorrectValue[31] = {  // data_020bc1dc
    -1193, -982, -844, -737, -647, -568, -497, -432,
     -371, -313, -258, -204, -152, -101,  -51,   -1,
       50,  100,  151,  203,  257,  312,  370,  431,
      496,  567,  646,  736,  843,  981, 1192,
};


THUMB CountDown::CountDown(){
    this->tileIdRate_ = 0;
}

THUMB CountDown::~CountDown(){
    return;
}


THUMB void CountDown::setup()
{
    counter_ = 0x1E00;
    counter_ += CorrectValue[dssrand::rand(0x1F)];
    subtractValue_ = 0;
}

THUMB void CountDown::exec()
{
    counter_ = counter_ - subtractValue_;
}


THUMB void CountDown::setTileIdRate(int title)
{
    tileIdRate_ = title;
    setLandSubtranctValue();
}

THUMB void CountDown::setLandType(LandType land)
{
    this->fix_ = 0;                 
    switch (land)
    {
    case Floor:
        this->fix_ = 1;
        this->landType_ = 0;
        break;
    case Sea:      this->landType_ = 1; break;
    case Desert:   this->landType_ = 2; break;
    case Mountain: this->landType_ = 3; break;
    case Pond:     this->landType_ = 4; break;
    case Bush:     this->landType_ = 5; break;
    case Field:    this->landType_ = 6; break;
    case Forest:   this->landType_ = 7; break;
    }
    setLandSubtranctValue();
}



THUMB void CountDown::setLandSubtranctValue()
{
    int v = LandSubtractValue[this->landType_] * TileSubtractValue[this->tileIdRate_];
    this->subtractValue_ = v / 256;
    if (fix_ != 0)
        this->subtractValue_ = this->subtractValue_ * 3 / 2;
}