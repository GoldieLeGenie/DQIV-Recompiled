#include "ov000/town/TownFurnitureControl.hpp"

THUMB TownFurnitureControlStorage::TownFurnitureControlStorage()
{
}

THUMB TownFurnitureControlStorage::~TownFurnitureControlStorage()
{
}

THUMB void TownFurnitureControlStorage::initialize()
{
    moveCounter_ = 0;
    fadeCounter_ = 0;
    moveCounter2_ = 0;
    for (unsigned int i = 0; i < 8; i++) {
        move_[i].enable_ = 0;
    }
    for (unsigned int i = 0; i < 16; i++) {
        fade_[i].enable_ = 0;
    }
    for (unsigned int i = 0; i < 8; i++) {
        move2_[i].enable_ = 0;
    }
}

THUMB void TownFurnitureControlStorage::terminate()
{
}

THUMB TownFurnitureControlBase* TownFurnitureControlStorage::getContainer(int type)
{
    switch (type) {
    case 0:
        moveCounter_++;
        for (unsigned int i = 0; i < 8; i++) {
            if (move_[i].enable_ == 0) {
                return &move_[i];
            }
        }
        break;
    case 1:
        moveCounter2_++;
        for (unsigned int i = 0; i < 8; i++) {
            if (move2_[i].enable_ == 0) {
                return &move2_[i];
            }
        }
        break;
    case 2:
        fadeCounter_++;
        for (unsigned int i = 0; i < 16; i++) {
            if (fade_[i].enable_ == 0) {
                return &fade_[i];
            }
        }
        break;
    }
    return NULL;
}

THUMB void TownFurnitureControlStorage::restoreContainer(int type)
{
    switch (type) {
    case 0:
        moveCounter_--;
        break;
    case 1:
        moveCounter2_--;
        break;
    case 2:
        fadeCounter_--;
        break;
    }
}
