#include "main/btl/BattleExecLevelup.hpp"
#include "ov032/MaterielMenu_EXTRA_PRESENT_EXP/MaterielMenu_EXTRA_PRESENT_EXP.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/task/ExecTask.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_EXTRA_PRESENT_EXP::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    navigator_.setupBase();
    navigator_.setup(1, 1, 1);
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    extraExp_ = MaterielMenuPlayerControl::getSingleton()->extraExp_;
    subExp_ = extraExp_ <= 10000 ? 60 : 300;
    levelUpMode_ = 0;
    bgm_ = 0;
    MenuSoundManager::getSingleton()->initialize();
}

THUMB void MaterielMenu_EXTRA_PRESENT_EXP::menuExecute()
{
    MenuTemplate_materiel::shopSellQuantity(&menuItem_, menuItem_.active_);
}

THUMB void MaterielMenu_EXTRA_PRESENT_EXP::menuDraw()
{
    if (extraExp_ > 0) {
        unkfunc_0216fdb0(activeChara_, extraExp_);
    }
}

THUMB void MaterielMenu_EXTRA_PRESENT_EXP::menuUpdate()
{
    if (MenuSoundManager::getSingleton()->isPlaySound()) {
        return;
    }
    if (extraExp_ > 0) {
        extraExp_ -= subExp_;
        SoundManager::playSe(0x12d, 0);
        if (extraExp_ < 0) {
            subExp_ += extraExp_;
            if (status::g_Party.getLevelupPlayer() != -1) {
                levelUpMode_ = 1;
                bgm_ = SoundManager::bgmIndex_;
                SoundManager::stopBgm(0);
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_LEVEL_UP);
            }
        }
        status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.addExp(subExp_);
        redraw_ = 1;
        return;
    }
    switch (levelUpMode_) {
    case 0:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    case 1:
        if (status::g_Party.getLevelupPlayer() != -1) {
            if (g_BattleExecLevelup.execute() == false) {
                g_BattleExecLevelup.terminate();
            }
        } else {
            if (g_BattleExecLevelup.execute() == false) {
                g_BattleExecLevelup.terminate();
                levelUpMode_ = 2;
            }
        }
        break;
    case 2:
        if (MenuUpdate_Assist::menuSelect(menuItem_, navigator_) != 0) {
            SoundManager::playBgm(bgm_, 0);
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
        break;
    }
}
