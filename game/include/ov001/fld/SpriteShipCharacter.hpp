#pragma once
#include "globaldefs.h"
#include "main/object/SpriteCharacter.hpp"

/* vtable 0x02160ac4 */
struct SpriteShipCharacter : SpriteCharacter {
    virtual void execute();

    SpriteShipCharacter();
    ~SpriteShipCharacter();
    void setup(const char* name);
};
