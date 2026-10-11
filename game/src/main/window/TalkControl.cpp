#include "main/window/TalkControl.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "main/cmn/NonBattleActionManager.hpp"

ARM window::TalkControl::TalkControl()
{
}

ARM window::TalkControl::~TalkControl()
{
}

ARM void window::TalkControl::setup()
{
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::setAllCharaAnim(false);
    } else if (data_0210bb94.unkfunc_02058114(0xe)) {
        SpriteCharacter::setAllCharaAnim(false);
    }
    playerLock(true);
    openTalk();
}

ARM void window::TalkControl::execute()
{
    if (gTownMenu_PARTY_TALK.isOpen()) {
        return;
    }
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::setAllCharaAnim(true);
    } else {
        SpriteCharacter::setAllCharaAnim(true);
    }
    goNext(PHASE_NORMAL);
    playerLock(false);
}

ARM void window::TalkControl::openTalk()
{
    gTownMenu_PARTY_TALK.open();
}

ARM int window::TalkControl::getPhase()
{
    return PHASE_PARTY_TALK;
}
