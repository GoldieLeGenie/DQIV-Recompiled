#include "main/window/ShopMenuControl.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov001/fld/FieldStage.hpp"

ARM void window::ShopMenuControl::setup()
{
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        BillboardCharacter::setAllCharaAnim(false);
        TownStageManager::getSingleton()->stage_.m_fld.SetPause(1);
    } else if (data_0210bb94.unkfunc_02058114(0xe)) {
        fld::FieldStage::getSingleton()->fieldData.pause_ = 1;
        SpriteCharacter::setAllCharaAnim(false);
    }
    playerLock(true);
    state_ = OPEN_SETUP;
}

ARM void window::ShopMenuControl::execute()
{
    switch (state_) {
        case OPEN_SETUP:
            MenuAPI::clearMenuAll();
            state_ = OPEN_SETUP1;
            break;
        case OPEN_SETUP1:
            state_ = OPEN_SETUP2;
            break;
        case OPEN_SETUP2:
            state_ = OPEN_WAIT;
            MenuAPI::changeMenuModeExtra();
            break;
        case OPEN_WAIT:
            if (MenuAPI::isMenuModeExtra()) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(menuType_);
                state_ = OPEN;
            }
            break;
        case OPEN:
            if (MaterielMenu_WINDOW_MANAGER::getSingleton()->endWindow_) {
                MenuAPI::changeMenuModeNormal();
                state_ = CLOSE_WAIT;
            }
            break;
        case CLOSE_WAIT:
            if (MenuAPI::isMenuModeNormal()) {
                setupIcon();
                state_ = RELEASE_WAIT;
            }
            break;
        case RELEASE_WAIT:
            if (data_0210bb94.unkfunc_02058114(0xc)) {
                TownStageManager::getSingleton()->stage_.m_fld.SetPause(0);
                BillboardCharacter::setAllCharaAnim(true);
            } else {
                fld::FieldStage::getSingleton()->fieldData.pause_ = 0;
                SpriteCharacter::setAllCharaAnim(true);
            }
            playerLock(false);
            goNext(PHASE_NORMAL);
            break;
    }
}

ARM int window::ShopMenuControl::getPhase()
{
    return PHASE_SHOPMENU;
}
