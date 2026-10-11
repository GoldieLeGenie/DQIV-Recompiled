#include <globaldefs.h>
#include "main/status/GameStatus.hpp"

status::GameStatus status::g_Game;

THUMB status::GameStatus::GameStatus(){
    this->playTime_ = 0;
    return;
}


THUMB status::GameStatus::~GameStatus(){
    return;
}
THUMB void status::GameStatus::setPlayTime(unsigned int Playtime)
{
    this->playTime_ = Playtime;
    if (0xcdfd7f0 < Playtime) {
        this->playTime_ = 0xcdfd7f0;
    }
    return;
}

THUMB void status::GameStatus::addPlayTime(unsigned int time)
{
    unsigned int playTime;

    playTime = this->playTime_ + time;
    this->playTime_ = playTime;
    if (0xcdfd7f0 < playTime) {
        this->playTime_ = 0xcdfd7f0;
    }
    return;
}

THUMB unsigned int status::GameStatus::getPlayTime()
{
    return this->playTime_;
}



THUMB void status::GameStatus::resetUniqueID() {
    unsigned char buffer[8];

    func_02079e24(buffer);

    unsigned char byte3 = buffer[3];
    unsigned char byte4 = buffer[4];
    unsigned char byte5 = buffer[5];

    int randomValue = dssrand::rand(0x100);

    unsigned int id = (byte3 << 24)
        + (byte4 << 16)
        + (byte5 << 8)
        + randomValue;

    this->uniqueID_ = id;
    return;
}


THUMB unsigned int status::GameStatus::getUniqueID() {
    if (uniqueID_ == 0) {
        resetUniqueID();
    }
    return uniqueID_;
}


THUMB void status::GameStatus::setUniqueID(unsigned int id)
{
    this->uniqueID_ = id;
    return;
}

