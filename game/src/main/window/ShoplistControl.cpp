#include "main/window/ShoplistControl.hpp"
#include "main/dss/Pad.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/NonBattleActionManager.hpp"

ARM void window::ShoplistControl::setup()
{
    state_ = TOWNMAP_WAIT_OPEN;
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::setAllCharaAnim(false);
    } else if (data_0210bb94.unkfunc_02058114(0xe)) {
        SpriteCharacter::setAllCharaAnim(false);
    }
    openList();
    playerLock(true);
}

ARM void window::ShoplistControl::execute()
{
    switch (state_) {
        case TOWNMAP_WAIT_OPEN:
            if (imageMap_->isOpen()) {
                state_ = TOWNMAP_VIEWING;
            }
            break;
        case TOWNMAP_VIEWING:
            if (dss::g_Pad.edge() & 0x800) {
                if (unkfunc_0202a2d8()) {
                    state_ = TOWNMAP_WAIT_CLOSE_TO_MAP;
                    return;
                }
                closeList();
                state_ = TOWMMAP_WAIT_CLOSE;
            } else if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 2)) {
                closeList();
                state_ = TOWMMAP_WAIT_CLOSE;
            } else {
                dss::g_Pad.edge();
            }
            break;
        case TOWMMAP_WAIT_CLOSE:
            if (imageMap_->isClose()) {
                gUnkTownMenu_02178a58.unkfunc_02178aa8(0);
                state_ = TOWMMAP_CLOSE;
            }
            break;
        case TOWMMAP_CLOSE:
            if (goNext(PHASE_NORMAL)) {
                if (data_0210bb94.unkfunc_02058114(0xc)) {
                    BillboardCharacter::setAllCharaAnim(true);
                } else if (data_0210bb94.unkfunc_02058114(0xe)) {
                    SpriteCharacter::setAllCharaAnim(true);
                }
                playerLock(false);
            }
            break;
        case TOWNMAP_MESSAGE:
            if (data_020ed1bc.isOpen()) {
                if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                    data_020ed1bc.close();
                    closeListMessage();
                    state_ = TOWNMAP_VIEWING;
                }
            }
            break;
        case TOWNMAP_WAIT_CLOSE_TO_MAP:
            state_ = TOWNMAP_VIEWING;
            break;
    }
}

ARM void window::ShoplistControl::openList()
{
    gUnkTownMenu_02178a58.unkfunc_02178aa8(2);
    for (int i = 0; i < 6; i++) {
        if (gTownShopMenu.unkfunc_02177250(i)) {
            if (prev_ == PHASE_MAP) {
                imageMap_->openBlack();
            } else {
                imageMap_->open();
            }
            gTownShopMenu.open();
            return;
        }
    }
}

ARM void window::ShoplistControl::closeList()
{
    imageMap_->close();
    gTownShopMenu.close();
    setupIcon();
}

ARM int window::ShoplistControl::unkfunc_0202a2d8()
{
    return gTownShopMenu.unkfunc_02177300();
}

ARM void window::ShoplistControl::closeListMessage()
{
}

ARM void window::ShoplistControl::registShopList(ImageMap* shoplist)
{
    imageMap_ = shoplist;
}

ARM int window::ShoplistControl::getPhase()
{
    return PHASE_SHOPLIST;
}
