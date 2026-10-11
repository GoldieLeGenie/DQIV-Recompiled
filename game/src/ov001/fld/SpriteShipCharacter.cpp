#pragma ipa file
#include "ov001/fld/SpriteShipCharacter.hpp"

ARM SpriteShipCharacter::SpriteShipCharacter()
{
}

ARM SpriteShipCharacter::~SpriteShipCharacter()
{
}

ARM void SpriteShipCharacter::setup(const char* name)
{
    SpriteCharacter::setup(name);
    unk_a0 = 0x28;
    unk_9c = 0x28;
    unkfunc_0204b33c();
}

ARM void SpriteShipCharacter::execute()
{
    if (flag_.check(FLAG_ANIM_NEUTRAL)) {
        if (!(allFlag_.flag_ & FLAG_ANIM)) {
            return;
        }
    } else if (!flag_.check(FLAG_ANIM)) {
        return;
    }
    switch (anmIndex_ / 6) {
    case 0:
        unkfunc_02084578(0, 0, 0x28, 0x28);
        break;
    case 1:
        unkfunc_02084578(0x28, 0, 0x50, 0x28);
        break;
    case 2:
        unkfunc_02084578(0x50, 0, 0x78, 0x28);
        break;
    case 3:
        unkfunc_02084578(0, 0x28, 0x28, 0x50);
        break;
    case 4:
        unkfunc_02084578(0x28, 0x28, 0x50, 0x50);
        break;
    case 5:
        unkfunc_02084578(0x50, 0x28, 0x78, 0x50);
        break;
    case 6:
        unkfunc_02084578(0, 0x50, 0x28, 0x78);
        break;
    }
    anmIndex_++;
    anmIndex_ = dss::loop(anmIndex_, 0, 0x2a);
}
