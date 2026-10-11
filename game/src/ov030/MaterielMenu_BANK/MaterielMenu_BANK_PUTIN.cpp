#include "ov030/MaterielMenu_BANK/MaterielMenu_BANK.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_BANK_PUTIN::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_UP);
    menuItem_.active_ = 2;
    oldActive_ = 2;
    navigator_.setupBase();
    putinMoney_ = 0;
    first_ = 1;
    end_ = 0;
}

THUMB void MaterielMenu_BANK_PUTIN::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_MONEY_SELECT(&menuItem_, oldActive_, 3);
}

THUMB void MaterielMenu_BANK_PUTIN::menuDraw()
{
    if (first_ == 0) {
        if (data_020ed1bc.isOpen()) {
            unkfunc_0216fd00(1, 1, putinMoney_);
        } else {
            unkfunc_0216fd00(0, 1, putinMoney_);
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_BANK_PUTIN::menuUpdate()
{
    if (messageUpdate() == false) {
        bankUpdate();
    }
}

THUMB bool MaterielMenu_BANK_PUTIN::messageUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
            if (end_) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        }
        return true;
    }
    if (first_) {
        first_ = 0;
        data_020ed1bc.openMessageForTALK();
        if (status::g_Party.bankMoney_ >= 99999000) {
            TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
            data_020ed1bc.addMessage(0xc6bb8, 0xc6bdb);
            end_ = 1;
            return true;
        }
        data_020ed1bc.addMessage(0xc6bbb);
    }
    return false;
}

THUMB void MaterielMenu_BANK_PUTIN::bankUpdate()
{
    int addMoney = 0;
    switch (menuItem_.active_) {
    case 0:
        addMoney = 100000;
        break;
    case 1:
        addMoney = 10000;
        break;
    case 2:
        addMoney = 1000;
        break;
    }
    navigator_.setup(3, 1, 3);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            if (putinMoney_ == 0) {
                cancelPutin();
            } else {
                bankPutin();
            }
            return;
        }
        if (result == 3) {
            cancelPutin();
            return;
        }
        if (result == 4) {
            putinMoney_ += addMoney;
            if (putinMoney_ > 999000) {
                putinMoney_ = 999000;
            }
        }
        if (result == 5) {
            putinMoney_ -= addMoney;
            if (putinMoney_ < 0) {
                putinMoney_ = 0;
            }
        }
        oldActive_ = menuItem_.active_;
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_BANK_PUTIN::bankPutin()
{
    data_020ed1bc.openMessageForTALK();
    if (putinMoney_ > status::g_Party.gold_) {
        data_020ed1bc.addMessage(0xc6bbe);
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        putinMoney_ = 0;
        first_ = 1;
        return;
    }
    if (putinMoney_ + status::g_Party.bankMoney_ > 99999000) {
        TextAPI::setMACRO0(0x34, 0xf0000000, 99999000 - status::g_Party.bankMoney_);
        data_020ed1bc.addMessage(0xc6bc1);
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        putinMoney_ = 0;
        first_ = 1;
        return;
    }
    status::g_Party.addBankMoney(putinMoney_);
    status::g_Party.setGold(status::g_Party.gold_ - putinMoney_);
    TextAPI::setMACRO0(0x31, 0xf0000000, putinMoney_);
    TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
    data_020ed1bc.addMessage(0xc6bc4, 0xc6bdb);
    menuItem_.result_ = 0;
    menuItem_.lastresult_ = 0;
    end_ = 1;
}

THUMB void MaterielMenu_BANK_PUTIN::cancelPutin()
{
    data_020ed1bc.close();
    data_020ed1bc.openMessageForTALK();
    if (status::g_Party.bankMoney_ == 0) {
        int i = 0;
        bool alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        while (!alive) {
            i++;
            alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        }
        TextAPI::setMACRO0(0xb, 0x50000000, status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_);
        data_020ed1bc.addMessage(0xc6bd7, 0xc6bdd);
    } else {
        TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
        data_020ed1bc.addMessage(0xc6bd7, 0xc6bdb);
    }
    end_ = 1;
}
