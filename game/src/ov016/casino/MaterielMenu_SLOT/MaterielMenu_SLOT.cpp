#include "ov016/casino/MaterielMenu_SLOT/MaterielMenu_SLOT.hpp"
#include "main/dss/Pad.hpp"
#include "ov009/casino/Casino_Slot.hpp"
#include "ov009/casino/CasinoSlot.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/global/Global.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_SLOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    slotType_ = g_Global.getGameStatus();
    Casino_Slot::getSingleton()->setSlotType(slotType_);
    CasinoSlot::getSingleton()->setSlotType(slotType_);
    MenuSoundManager::getSingleton()->initialize();
    status_ = SLOT_START;
    messageCount_ = -1;
    haveCoin_ = 0;
    betCoin_ = 0;
}

THUMB void MaterielMenu_SLOT::setSlotType(int type)
{
    slotType_ = type;
}

THUMB void MaterielMenu_SLOT::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_GET_UPDOWN(&menuItem_);
}

THUMB void MaterielMenu_SLOT::menuDraw()
{
    unkfunc_0216fd58(haveCoin_, 0);
}

THUMB void MaterielMenu_SLOT::menuUpdate()
{
    if (!MenuSoundManager::getSingleton()->isPlaySound()) {
        if (!messageUpdate()) {
            statusUpdate();
        }
    }
}

THUMB bool MaterielMenu_SLOT::messageUpdate()
{
    if (!data_020ed1bc.isOpen()) {
        return false;
    }
    int stat = data_020ed1bc.stat_;
    if (status_ == SLOT_RETRY && messageCount_ == 0) {
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            status_ = SLOT_START;
            messageCount_ = -1;
            data_020ed1bc.close();
        } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            status_ = SLOT_END;
            messageCount_ = -1;
            data_020ed1bc.close();
        }
    }
    closeMessage();
    return true;
}

THUMB void MaterielMenu_SLOT::statusUpdate()
{
    switch (status_) {
    case SLOT_START:
        if (messageCount_ == -1) {
            if (status::g_Party.casinoCoin_ == 0) {
                showMessage(0xc96af);
                status_ = SLOT_END;
                messageCount_ = -1;
                return;
            }
            showMessage(0xc96b2);
            menuItem_.active_ = 0;
            messageCount_++;
            haveCoin_ = status::g_Party.casinoCoin_;
            Casino_Slot::getSingleton()->resetSlot();
            for (int i = 0; i < betCoin_; i++) {
                Casino_Slot::getSingleton()->addCoin(haveCoin_);
            }
        } else {
            inputUpdate();
        }
        break;
    case SLOT_GAME:
        gameUpdate();
        break;
    case SLOT_RESULT:
        if (resultCoin_ > 0) {
            TextAPI::setMACRO0(0x48, 0xf0000000, resultCoin_);
            showMessage(0xc96b8);
            status_ = SLOT_RESULT_EFFECT;
            messageCount_ = -1;
            if (resultCoin_ >= GREAT_FANFARE_NUM) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_L);
            } else if (resultCoin_ >= FANFARE_NUM) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_M);
            } else {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_S);
            }
        } else {
            showMessage(0xc96bc);
            Casino_Slot::getSingleton()->unkfunc_02123944();
            status::g_Party.setCasinoCoin(haveCoin_);
            status_ = SLOT_RETRY;
            messageCount_ = -1;
        }
        break;
    case SLOT_RESULT_EFFECT:
        resultEffectUpdate();
        status::g_Party.setCasinoCoin(haveCoin_);
        break;
    case SLOT_RETRY:
        if (messageCount_ == -1) {
            showMessage(0xc96bf);
            data_020ed1bc.setYesNo();
            messageCount_++;
        }
        break;
    case SLOT_END:
        data_020ed1bc.close();
        close();
        stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_SLOT::inputUpdate()
{
    menuItem_.execInput();
    switch (menuItem_.result_) {
    case 3:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        status_ = SLOT_END;
        messageCount_ = -1;
        break;
    case 2:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        if (betCoin_ > 0) {
            Casino_Slot::getSingleton()->startSlot();
            status_ = SLOT_GAME;
            messageCount_ = -1;
        }
        break;
    case 5:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        if (betCoin_ == 5) {
            Casino_Slot::getSingleton()->startSlot();
            status_ = SLOT_GAME;
            messageCount_ = -1;
        } else {
            Casino_Slot::getSingleton()->addCoin(haveCoin_);
            betCoin_ = Casino_Slot::getSingleton()->m_bet_coin;
            redraw_ = 1;
        }
        break;
    case 6:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        Casino_Slot::getSingleton()->subCoin(haveCoin_);
        betCoin_ = Casino_Slot::getSingleton()->m_bet_coin;
        redraw_ = 1;
        break;
    }
}

THUMB void MaterielMenu_SLOT::resultEffectUpdate()
{
    int oldCoin = haveCoin_;
    if (Casino_Slot::getSingleton()->showEffect()) {
        Casino_Slot::getSingleton()->cashAllCoin(haveCoin_);
        status::g_Party.setCasinoCoin(haveCoin_);
        resultCoin_ = 0;
        Casino_Slot::getSingleton()->unkfunc_02123944();
        status_ = SLOT_RETRY;
        messageCount_ = -1;
    }
    if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 0x400)) {
        Casino_Slot::getSingleton()->cashAllCoin(haveCoin_);
        status::g_Party.setCasinoCoin(haveCoin_);
        resultCoin_ = 0;
    }
    Casino_Slot::getSingleton()->cashCoin(haveCoin_);
    if (oldCoin < haveCoin_) {
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_SLOT::gameUpdate()
{
    if (Casino_Slot::getSingleton()->runningSlot()) {
        resultCoin_ = 0;
        resultCoin_ = Casino_Slot::getSingleton()->getResultAllCoin();
        status_ = SLOT_RESULT;
        messageCount_ = -1;
    }
}

THUMB void MaterielMenu_SLOT::showMessage(int messageID)
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(messageID);
}

THUMB void MaterielMenu_SLOT::closeMessage()
{
    if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
        data_020ed1bc.close();
    }
}
