#include "ov029/MaterielMenu_COINSALEROOM/MaterielMenu_COINSALEROOM.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_COINSALEROOM_BUY::menuSetup()
{
    status::g_Party.setPlayerMode();
}

THUMB void MaterielMenu_COINSALEROOM_BUY::menuExecute()
{
}

THUMB void MaterielMenu_COINSALEROOM_BUY::menuDraw()
{
    unkfunc_0216fd34(coin_, 0);
}

THUMB void MaterielMenu_COINSALEROOM_BUY::menuUpdate()
{
    if (messageUpdata() == false) {
        buyCoinMessage();
    }
}

THUMB bool MaterielMenu_COINSALEROOM_BUY::messageUpdata()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            yesMessage();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            noMessage();
        }
        return true;
    }
    return false;
}

THUMB void MaterielMenu_COINSALEROOM_BUY::buyCoinMessage()
{
    int totalGold = coin_ * coinPrice_;
    if (status::g_Party.gold_ < totalGold) {
        showMessage(0xc8715);
        mode_ = 0;
        return;
    }
    if (coin_ + status::g_Party.casinoCoin_ > 999999) {
        TextAPI::setMACRO0(0x38, 0xf0000000, 999999 - status::g_Party.casinoCoin_);
        showMessage(0xc871b);
        mode_ = 1;
        return;
    }
    TextAPI::setMACRO0(0x49, 0xf0000000, coin_);
    TextAPI::setMACRO0(0x46, 0xf0000000, totalGold);
    showMessage(0xc8718);
    data_020ed1bc.setYesNo();
    mode_ = 2;
}

THUMB void MaterielMenu_COINSALEROOM_BUY::yesMessage()
{
    data_020ed1bc.close();
    switch (mode_) {
    case 0:
        close();
        gMaterielMenu_COINSALEROOM_ROOT.open();
        gMaterielMenu_COINSALEROOM_ROOT.mode_ = 1;
        break;
    case 1: {
        int maxToken = 999999 - status::g_Party.casinoCoin_;
        int tokenGold = maxToken * coinPrice_;
        TextAPI::setMACRO0(0x38, 0xf0000000, maxToken);
        TextAPI::setMACRO0(0x43, 0xf0000000, tokenGold);
        showMessage(0xc871c);
        data_020ed1bc.setYesNo();
        mode_ = 2;
        break;
    }
    case 2:
        getCasinoCoin();
        break;
    case 3:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_COINSALEROOM_BUY::noMessage()
{
    data_020ed1bc.close();
    close();
    gMaterielMenu_COINSALEROOM_ROOT.open();
    gMaterielMenu_COINSALEROOM_ROOT.mode_ = 1;
}

THUMB void MaterielMenu_COINSALEROOM_BUY::getCasinoCoin()
{
    if (coin_ + status::g_Party.casinoCoin_ > 999999) {
        status::g_Party.setGold(status::g_Party.gold_ - coinPrice_ * (999999 - status::g_Party.casinoCoin_));
        status::g_Party.setCasinoCoin(999999);
    } else {
        status::g_Party.setGold(status::g_Party.gold_ - coin_ * coinPrice_);
        status::g_Party.addCasinoCoin(coin_);
    }
    showMessage(0xc871f);
    mode_ = 3;
}

THUMB void MaterielMenu_COINSALEROOM_BUY::showMessage(int messageID)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID);
}
