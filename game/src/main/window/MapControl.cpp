#include "main/window/MapControl.hpp"
#include "main/dss/Pad.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/NonBattleActionManager.hpp"

ARM void window::MapControl::setup()
{
    state_ = TOWNMAP_WAIT_OPEN;
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::setAllCharaAnim(false);
    } else if (data_0210bb94.unkfunc_02058114(0xe)) {
        SpriteCharacter::setAllCharaAnim(false);
    }
    openMap();
    playerLock(true);
}

ARM void window::MapControl::execute()
{
    switch (state_) {
        case TOWNMAP_WAIT_OPEN:
            if (imageMap_->isOpen()) {
                state_ = TOWNMAP_VIEWING;
            }
            break;
        case TOWNMAP_VIEWING:
            if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 2)) {
                closeMap();
                state_ = TOWMMAP_WAIT_CLOSE;
            } else if (dss::g_Pad.edge() & 1) {
                showMapMessage();
                state_ = TOWNMAP_MESSAGE;
            } else if (dss::g_Pad.edge() & 0x800) {
                if (goNext(PHASE_SHOPLIST)) {
                    imageMap_->close();
                    playerLock(false);
                } else {
                    closeMap();
                    state_ = TOWMMAP_WAIT_CLOSE;
                }
            }
            break;
        case TOWMMAP_WAIT_CLOSE:
            if (imageMap_->isClose()) {
                if (goNext(PHASE_NORMAL)) {
                    if (data_0210bb94.unkfunc_02058114(0xc)) {
                        BillboardCharacter::setAllCharaAnim(true);
                    } else {
                        SpriteCharacter::setAllCharaAnim(true);
                    }
                    playerLock(false);
                }
            }
            break;
        case TOWNMAP_MESSAGE:
            if (data_020ed1bc.isOpen()) {
                if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                    data_020ed1bc.close();
                    closeMapMessage();
                    state_ = TOWNMAP_VIEWING;
                }
            }
            break;
    }
}

ARM void window::MapControl::initialize()
{
    imageMap_ = 0;
}

ARM void window::MapControl::registImageMap(ImageMap* imageMap)
{
    imageMap_ = imageMap;
}

ARM void window::MapControl::openMap()
{
    imageMap_->open();
    gUnkTownMenu_02178a58.unkfunc_02178aa8(1);
}

ARM void window::MapControl::closeMap()
{
    imageMap_->close();
    gUnkTownMenu_02178a58.unkfunc_02178aa8(0);
    setupIcon();
}

ARM void window::MapControl::showMapMessage()
{
    status::g_Party.getPlayerStatus(0);
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(0xc3dd2);
}

ARM void window::MapControl::closeMapMessage()
{
    gUnkTownMenu_02178a58.unkfunc_02178aa8(1);
}

ARM int window::MapControl::getPhase()
{
    return PHASE_MAP;
}
