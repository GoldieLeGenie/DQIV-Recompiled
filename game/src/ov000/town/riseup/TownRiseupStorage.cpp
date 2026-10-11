#include "ov000/town/riseup/TownRiseup.hpp"

THUMB TownRiseupStorage::TownRiseupStorage()
{
}

THUMB TownRiseupStorage::~TownRiseupStorage()
{
}

/* not original: keeps the single-iteration medal_ loop that the ROM still has */
#pragma push
#pragma opt_removeemptyloops off
THUMB void TownRiseupStorage::initialize()
{
    iconCounter_ = 0;
    spriteCounter_ = 0;
    scriptCounter_ = 0;
    for (unsigned int i = 0; i < 4; i++) {
        icon_[i].enable_ = 0;
    }
    for (unsigned int i = 0; i < 16; i++) {
        sprite_[i].enable_ = 0;
    }
    for (unsigned int i = 0; i < 2; i++) {
        script_[i].enable_ = 0;
    }
    for (unsigned int i = 0; i < 1; i++) {
        medal_[i].enable_ = 0;
    }
}
#pragma pop

THUMB void TownRiseupStorage::terminate()
{
}

THUMB TownRiseupBase* TownRiseupStorage::getContainer(int type)
{
    switch (type) {
    case 0:
        iconCounter_++;
        for (unsigned int i = 0; i < 4; i++) {
            if (icon_[i].enable_ == 0) {
                return &icon_[i];
            }
        }
        break;
    case 1:
        spriteCounter_++;
        for (unsigned int i = 0; i < 16; i++) {
            if (sprite_[i].enable_ == 0) {
                return &sprite_[i];
            }
        }
        break;
    case 2:
        scriptCounter_++;
        for (unsigned int i = 0; i < 2; i++) {
            if (script_[i].enable_ == 0) {
                return &script_[i];
            }
        }
        break;
    case 3:
        medalCounter_++;
        for (unsigned int i = 0; i < 1; i++) {
            if (medal_[i].enable_ == 0) {
                return &medal_[i];
            }
        }
        break;
    }
    return NULL;
}

THUMB void TownRiseupStorage::restoreContainer(int type)
{
    switch (type) {
    case 0:
        iconCounter_--;
        break;
    case 1:
        spriteCounter_--;
        break;
    case 2:
        scriptCounter_--;
        break;
    case 3:
        medalCounter_--;
        break;
    }
}
