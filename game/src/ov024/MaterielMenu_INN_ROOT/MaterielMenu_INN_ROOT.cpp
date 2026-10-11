#include "ov024/MaterielMenu_INN_ROOT/MaterielMenu_INN_ROOT.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/ShopList.hpp"
#include "main/profile/Profile.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"

THUMB void MaterielMenu_INN_ROOT::menuSetup()
{
    status::g_Party.setBattleMode();
    mode_ = 0;
    fadeMode_ = 0;
    soundCount_ = 0;
    stayCount_ = 0;
    innCharge_ = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == false) {
            revivalPlayer_[stayCount_] = i;
            stayCount_++;
        }
    }
    int price;
    if (MaterielMenu_WINDOW_MANAGER::getSingleton()->menuType_ == MaterielMenu_WINDOW_MANAGER::MENU_INN_TYPE2) {
        price = status::g_Shop.getHotelPrice(1);
    } else {
        price = status::g_Shop.getHotelPrice(0);
    }
    innCharge_ = stayCount_ * price;
    MenuSoundManager::getSingleton()->initialize();
    extraInnType_ = MaterielMenu_WINDOW_MANAGER::getSingleton()->extraInnType_;
}

THUMB void MaterielMenu_INN_ROOT::menuDraw()
{
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void MaterielMenu_INN_ROOT::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            showMessage(0xc67d6);
            mode_ = 4;
        }
        return;
    }
    if (mode_ == 1) {
        fadeEffect();
        return;
    }
    if (mode_ == 4) {
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraImuruEnd_ = 0;
        return;
    }
    if (g_Global.bookingFlag_ == Global::BOOKING_INN) {
        int ctrlID = g_cmnPartyInfo.ctrlID_;
        cmn::g_CommonCounterInfo.setChangeDay();
        cmn::g_CommonCounterInfo.freeCounter_[0]++;
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(ctrlID));
        showMessage(0xc67d2);
        mode_ = 3;
        return;
    }
    if (g_Stage.getTimeZone() == 4 || g_Stage.getTimeZone() == 1) {
        showMessage(0xc67cd);
        data_020ed1bc.setMessageLastCursor(true);
    } else {
        showMessage(0xc67ca);
        data_020ed1bc.setMessageLastCursor(true);
    }
}

THUMB void MaterielMenu_INN_ROOT::selectYes()
{
    switch (mode_) {
    case 0:
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0x33, 0xf0000000, innCharge_);
        data_020ed1bc.addMessage(0xc67ce);
        data_020ed1bc.setYesNo();
        mode_ = 2;
        break;
    case 2:
        checkMoney();
        break;
    case 1:
        fadeEffect();
        break;
    case 4:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraImuruEnd_ = 0;
        break;
    case 3:
        cmn::GameManager::getSingleton();
        cmn::PlayerManager::setLock(0);
        cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 1;
        BillboardCharacter::setAllCharaAnim(1);
        g_Global.bookingFlag_ = Global::BOOKING_NONE;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraImuruEnd_ = 0;
        break;
    }
}

THUMB void MaterielMenu_INN_ROOT::checkMoney()
{
    if (status::g_Party.gold_ >= innCharge_) {
        for (int i = 0; i < stayCount_; i++) {
            status::g_Party.getPlayerStatus(revivalPlayer_[i])->haveStatusInfo_.revival();
        }
        status::g_Party.setGold(status::g_Party.gold_ - innCharge_);
        status::g_Story.setTarot(0);
        showMessage(0xc67d1);
        mode_ = 1;
    } else {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0xc67d5, 0xc67d6);
        mode_ = 4;
    }
}

THUMB void MaterielMenu_INN_ROOT::fadeEffect()
{
    if (extraInnType_ == 1) {
        g_Stage.setWorldTime(0x280);
        g_Stage.setTimeZone((TIME_ZONE)2);
        cmn::g_CommonCounterInfo.setChangeDay();
        cmn::g_CommonCounterInfo.freeCounter_[0]++;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraImuruEnd_ = 1;
        return;
    }
    if (extraInnType_ == 2) {
        switch (fadeMode_) {
        case 0:
            g_Stage.setTimeZone((TIME_ZONE)2);
            g_Stage.setWorldTime(0x280);
            g_Global.fadeOutBlack(0x3c);
            MenuSoundManager::getSingleton()->setPlaySound((MenuSoundManager::MENU_SOUND)0xf);
            fadeMode_ = 1;
            break;
        case 1:
            if (MenuSoundManager::getSingleton()->isPlaySound() == false) {
                g_Global.fadeInBlack(0x3c);
                fadeMode_ = 2;
            }
            break;
        case 2:
            if (!g_GlobalFade.isFadeEnd() != true) {
                cmn::g_CommonCounterInfo.setChangeDay();
                cmn::g_CommonCounterInfo.freeCounter_[0]++;
                showMessage(0xc67d2);
                mode_ = 4;
            }
            break;
        }
    } else {
        switch (fadeMode_) {
        case 0:
            g_Stage.setTimeZone((TIME_ZONE)2);
            g_Stage.setWorldTime(0x280);
            g_Global.fadeOutBlack(0x3c);
            MenuSoundManager::getSingleton()->setPlaySound((MenuSoundManager::MENU_SOUND)4);
            fadeMode_ = 1;
            break;
        case 1:
            if (MenuSoundManager::getSingleton()->isPlaySound() == false) {
                cmn::g_extraMapLink.setTownINN();
                cmn::GameManager::getSingleton();
                cmn::PlayerManager::setLock(1);
                cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
                BillboardCharacter::setAllCharaAnim(0);
                g_Global.bookingFlag_ = Global::BOOKING_INN;
                mode_ = 4;
            }
            break;
        }
    }
}

THUMB void MaterielMenu_INN_ROOT::showMessage(int mes)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(mes);
}

ARM void MaterielMenu_INN_ROOT::menuExecute()
{
}
