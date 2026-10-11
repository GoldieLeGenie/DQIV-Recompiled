#include "ov030/MaterielMenu_BANK/MaterielMenu_BANK.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_BANK_DRAW::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_UP);
    navigator_.setupBase();
    drawMoney_ = 0;
    first_ = 1;
    int playerCount = 0;
    while (status::g_Party.getPlayerStatus(playerCount)->haveStatusInfo_.isDeath()) {
        playerCount++;
        if (playerCount > status::g_Party.getCount()) {
            playerCount = 0;
            break;
        }
    }
    alivePlayer_ = status::g_Party.getPlayerStatus(playerCount)->haveStatusInfo_.haveStatus_.playerIndex_;
    end_ = 0;
}

THUMB void MaterielMenu_BANK_DRAW::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_MONEY_SELECT(&menuItem_, oldActive_, 3);
}

THUMB void MaterielMenu_BANK_DRAW::menuDraw()
{
    if (first_ == 0) {
        if (data_020ed1bc.isOpen()) {
            unkfunc_0216fd00(1, 0, drawMoney_);
        } else {
            unkfunc_0216fd00(0, 0, drawMoney_);
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_BANK_DRAW::menuUpdate()
{
    if (messageUpdate() == false) {
        bankUpdate();
    }
}

THUMB bool MaterielMenu_BANK_DRAW::messageUpdate()
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
        if (status::g_Party.bankMoney_ == 0) {
            TextAPI::setMACRO0(0xb, 0x50000000, alivePlayer_);
            data_020ed1bc.addMessage(0xc6bc8, 0xc6bdd);
            end_ = 1;
            return true;
        }
        TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
        data_020ed1bc.addMessage(0xc6bcb);
    }
    return false;
}

THUMB void MaterielMenu_BANK_DRAW::bankUpdate()
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
            if (drawMoney_ == 0) {
                cancelDrawfrom();
            } else {
                bankDraw();
            }
            return;
        }
        if (result == 3) {
            cancelDrawfrom();
            return;
        }
        if (result == 4) {
            drawMoney_ += addMoney;
            if (drawMoney_ > 999000) {
                drawMoney_ = 999000;
            }
        }
        if (result == 5) {
            drawMoney_ -= addMoney;
            if (drawMoney_ < 0) {
                drawMoney_ = 0;
            }
        }
        oldActive_ = menuItem_.active_;
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_BANK_DRAW::bankDraw()
{
    data_020ed1bc.openMessageForTALK();
    if (drawMoney_ > status::g_Party.bankMoney_) {
        data_020ed1bc.addMessage(0xc6bce);
        drawMoney_ = 0;
        first_ = 1;
        return;
    }
    if (drawMoney_ + status::g_Party.gold_ > 999999) {
        data_020ed1bc.addMessage(0xc6bd1);
        drawMoney_ = 0;
        first_ = 1;
        return;
    }
    status::g_Party.setBankMoney(status::g_Party.bankMoney_ - drawMoney_);
    status::g_Party.addGold(drawMoney_);
    TextAPI::setMACRO0(0x37, 0xf0000000, drawMoney_);
    data_020ed1bc.addMessage(0xc6bd4);
    if (status::g_Party.bankMoney_ == 0) {
        TextAPI::setMACRO0(0xb, 0x50000000, alivePlayer_);
        data_020ed1bc.addMessage(0xc6bdd);
    } else {
        TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
        data_020ed1bc.addMessage(0xc6bdb);
    }
    end_ = 1;
}

THUMB void MaterielMenu_BANK_DRAW::cancelDrawfrom()
{
    data_020ed1bc.openMessageForTALK();
    if (status::g_Party.bankMoney_ == 0) {
        TextAPI::setMACRO0(0xb, 0x50000000, alivePlayer_);
        data_020ed1bc.addMessage(0xc6bd7, 0xc6bdd);
    } else {
        TextAPI::setMACRO0(0x30, 0xf0000000, status::g_Party.bankMoney_);
        data_020ed1bc.addMessage(0xc6bd7, 0xc6bdb);
    }
    end_ = 1;
}
