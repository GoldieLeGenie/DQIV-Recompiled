#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerHiandlow.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/MaterielMenu_WINDOW_MANAGER/MaterielMenu_WINDOW_MANAGER.hpp"
#include "ov009/casino/PokerManager.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenuPokerHiandlow::menuSetup()
{
    menu_command_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menu_command_.active_ = 0;
    cursor_command_.setupBase();
    PokerManager::getSingleton()->startHighAndLow();
    gameMode_ = 0;
    doubleupCount_ = 0;
    haveCoin_ = status::g_Party.casinoCoin_ - PokerManager::getSingleton()->betCoin_;
    int betCoin = PokerManager::getSingleton()->betCoin_;
    getCoin_ = betCoin * PokerManager::getSingleton()->getMultiple();
    win_ = 0;
}

THUMB void MaterielMenuPokerHiandlow::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_POKER_HI_AND_LOW(&menu_command_, menu_command_.active_);
}

THUMB void MaterielMenuPokerHiandlow::menuDraw()
{
    unkfunc_0216fd48(haveCoin_, win_);
    menu_command_.drawActive();
}

THUMB void MaterielMenuPokerHiandlow::menuUpdate()
{
    if (!messageUpdate()) {
        statusUpdate();
    }
}

THUMB bool MaterielMenuPokerHiandlow::messageUpdate()
{
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK || stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (gameMode_ == 0) {
                gameMode_ = 1;
            }
            return false;
        }
        return true;
    }
    if (gameMode_ == 0) {
        showMessage(0, -1);
    }
    return false;
}

THUMB void MaterielMenuPokerHiandlow::statusUpdate()
{
    cursor_command_.setup(1, 2, 2);
    int result = MenuUpdate_Assist::menuSelect(menu_command_, cursor_command_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        switch (gameMode_) {
        case 1:
            PokerManager::getSingleton()->setAnswer(doubleupCount_, menu_command_.active_);
            judgementHiAndLow();
            break;
        case 2:
            if (menu_command_.active_ == 0) {
                gameMode_ = 0;
                win_ = 0;
            } else {
                status::g_Party.setCasinoCoin(haveCoin_ + getCoin_);
                haveCoin_ = haveCoin_ + getCoin_;
                menu_command_.active_ = 0;
                TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
                showMessage(0xc92f1, 0xc92f1 + 8);
                getCoin_ = 0;
                gameMode_ = 3;
            }
            break;
        case 3:
            if (menu_command_.active_ == 0) {
                gMaterielMenu_POKER_BETCOIN.open();
            } else {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
            break;
        }
    }
    redraw_ = 1;
}

THUMB void MaterielMenuPokerHiandlow::judgementHiAndLow()
{
    switch (PokerManager::getSingleton()->getHighAndLowResult(doubleupCount_)) {
    case 0:
        doubleupCount_++;
        getCoin_ *= 2;
        PokerManager::getSingleton()->setGetCoin(getCoin_);
        win_ = 1;
        if (doubleupCount_ >= 11) {
            showMessage(0xc92e7, -1);
            gameMode_ = 4;
        } else if (getCoin_ >= 10000) {
            showMessage(0xc92e3, -1);
            gameMode_ = 4;
        } else {
            TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
            TextAPI::setMACRO0(0x45, 0xf0000000, getCoin_ * 2);
            TextAPI::setMACRO0(0x2e, 0xf0000000, doubleupCount_ + 1);
            showMessage(0xc92d7, 0xc92d7 + 2);
            gameMode_ = 2;
        }
        break;
    case 1:
        getCoin_ = 0;
        PokerManager::getSingleton()->setGetCoin(0);
        doubleupCount_ = 0;
        win_ = 1;
        showMessage(0xc92f5, 0xc92f5 + 4);
        gameMode_ = 3;
        break;
    case 2:
        doubleupCount_++;
        showMessage(0xc92ed, -1);
        gameMode_ = 1;
        break;
    }
    menu_command_.active_ = 0;
}

THUMB void MaterielMenuPokerHiandlow::showMessage(int mes1, int mes2)
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(mes1);
    if (mes2 != -1) {
        data_020ed1bc.addMessage(mes2);
    }
}
