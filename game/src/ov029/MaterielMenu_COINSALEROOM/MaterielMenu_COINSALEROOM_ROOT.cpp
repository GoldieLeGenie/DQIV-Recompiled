#include "ov029/MaterielMenu_COINSALEROOM/MaterielMenu_COINSALEROOM.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_COINSALEROOM_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_UP);
    menuItem_.active_ = 5;
    navigator_.setupBase();
    mode_ = 0;
    oldActive_ = menuItem_.active_;
    coin_ = 0;
    first_ = 1;
    if (status::g_Story.chapter_ == 2) {
        coinPrice_ = 10;
    } else if (status::g_Story.chapter_ == 3) {
        coinPrice_ = 200;
    } else {
        coinPrice_ = 20;
    }
    blink_ = 0;
    blinkCount_ = 0;
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_COIN_SELECT(&menuItem_, oldActive_, 6);
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::menuDraw()
{
    if (data_020ed1bc.isMessageWAITPROG()) {
        unkfunc_0216fd34(coin_, 1);
        if (blink_ != 0) {
            if (blinkCount_ > 15) {
                menuItem_.drawActive();
            }
            if (blinkCount_ == 0 || blinkCount_ == 16) {
                redraw_ = 1;
            }
            blinkCount_++;
            if (blinkCount_ > 30) {
                blinkCount_ = 0;
            }
        } else {
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::menuUpdate()
{
    if (messageUpdata() == false) {
        buyCoinUpdata();
    }
}

THUMB bool MaterielMenu_COINSALEROOM_ROOT::messageUpdata()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            closeMessage();
        }
        return false;
    }
    switch (mode_) {
    case 0:
        if (status::g_Party.casinoCoin_ == 999999) {
            showMessage(0xc870a, 0xc870d, 0xc870e);
            mode_ = 2;
        } else {
            TextAPI::setMACRO0(0x2a, 0xf0000000, status::g_Party.casinoCoin_);
            TextAPI::setMACRO0(0x44, 0xf0000000, coinPrice_);
            showMessage(0xc870a, 0xc8711, 0xc8712);
            data_020ed1bc.setMessageLastCursor(true);
            data_020ed1bc.addMessageWAITKEY();
            blink_ = 1;
            redraw_ = 1;
            mode_ = -1;
        }
        break;
    case 1:
        TextAPI::setMACRO0(0x2a, 0xf0000000, status::g_Party.casinoCoin_);
        TextAPI::setMACRO0(0x44, 0xf0000000, coinPrice_);
        showMessage(0xc8711, 0xc8712, -1);
        data_020ed1bc.setMessageLastCursor(true);
        data_020ed1bc.addMessageWAITKEY();
        blink_ = 1;
        redraw_ = 1;
        mode_ = -1;
        break;
    case 2:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        return true;
    }
    return false;
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::buyCoinUpdata()
{
    if (data_020ed1bc.isOpen() && data_020ed1bc.isMessageWAITPROG()) {
        if (first_ == 1) {
            redraw_ = 1;
            first_ = 0;
        }
        int addCoin = 0;
        switch (menuItem_.active_) {
        case 0:
            addCoin = 100000;
            break;
        case 1:
            addCoin = 10000;
            break;
        case 2:
            addCoin = 1000;
            break;
        case 3:
            addCoin = 100;
            break;
        case 4:
            addCoin = 10;
            break;
        case 5:
            addCoin = 1;
            break;
        }
        navigator_.setup(6, 1, 6);
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            if (result == 2) {
                if (coin_ == 0) {
                    cancelBuyCoin();
                    return;
                }
                closeMessage();
                close();
                gMaterielMenu_COINSALEROOM_BUY.coin_ = coin_;
                gMaterielMenu_COINSALEROOM_BUY.coinPrice_ = coinPrice_;
                gMaterielMenu_COINSALEROOM_BUY.open();
            }
            if (result == 3) {
                cancelBuyCoin();
            }
            if (result == 4) {
                coin_ += addCoin;
                if (coin_ > 99999) {
                    coin_ = 99999;
                }
            }
            if (result == 5) {
                coin_ -= addCoin;
                if (coin_ < 0) {
                    coin_ = 0;
                }
            }
            if (result == 7) {
                menuItem_.active_ = 1;
            }
            if (menuItem_.active_ == 0) {
                menuItem_.active_ = 5;
            }
            oldActive_ = menuItem_.active_;
            redraw_ = 1;
        }
    }
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::closeMessage()
{
    data_020ed1bc.close();
    if (status::g_Party.casinoCoin_ == 999999) {
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
    }
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::cancelBuyCoin()
{
    showMessage(0xc8721, -1, -1);
    mode_ = 2;
}

THUMB void MaterielMenu_COINSALEROOM_ROOT::showMessage(int messageID1, int messageID2, int messageID3)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID1);
    if (messageID2 != -1) {
        data_020ed1bc.addMessage(messageID2);
    }
    if (messageID3 != -1) {
        data_020ed1bc.addMessage(messageID3);
    }
}
