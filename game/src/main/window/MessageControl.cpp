#include "main/window/MessageControl.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "ov000/town/TownSystem.hpp"

ARM void window::MessageControl::setup()
{
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::allAnimLock = 1;
    } else if (data_0210bb94.unkfunc_02058114(0xe)) {
        back_ = SpriteCharacter::getAllCharaAnim();
        SpriteCharacter::setAllCharaAnim(false);
    }
    playerLock(true);
    state_ = MESSAGE;
}

ARM void window::MessageControl::execute()
{
    switch (state_) {
        case MESSAGE:
            if (MenuAPI::isFinishMenu()) {
                MenuAPI::changeMenuModeNormal();
                state_ = CLOSE_WAIT;
            }
            break;
        case CLOSE_WAIT:
            if (MenuAPI::isMenuModeNormal()) {
                setupIcon();
                if (goNext(prev_)) {
                    if (data_0210bb94.unkfunc_02058114(0xc)) {
                        BillboardCharacter::allAnimLock = 0;
                    } else {
                        SpriteCharacter::setAllCharaAnim(back_);
                    }
                    playerLock(false);
                }
            }
            break;
    }
}

ARM int window::MessageControl::getPhase()
{
    return PHASE_MESSAGE;
}
