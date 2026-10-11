#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerSelectcard.hpp"
#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerBetcoin.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/MaterielMenu_WINDOW_MANAGER/MaterielMenu_WINDOW_MANAGER.hpp"
#include "ov009/casino/PokerManager.hpp"
#include "ov009/casino/CasinoPokerDraw.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/dss/Pad.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"

THUMB void MaterielMenuPokerSelectcard::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 1;
    activeCard_ = menuItem_.active_;
    doubleUpCount_ = 0;
    int betCoin = PokerManager::getSingleton()->betCoin_;
    getCoin_ = betCoin * PokerManager::getSingleton()->getMultiple();
    haveCoin_ = status::g_Party.casinoCoin_ - PokerManager::getSingleton()->betCoin_;
    gameMode_ = 0;
    win_ = 0;
    blink_ = 1;
    for (int i = 0; i < 11; i++) {
        winCoin_[i] = 0;
    }
    for (int i = 0; i < 5; i++) {
        cardCounter_[i] = 1;
    }
    animation_ = 1;
    ang_ = 0x8000;
    gyre_ = 0;
    index_ = 1;
    distance_ = CasinoPokerDraw::getSingleton()->getDistance();
    PokerManager::getSingleton()->startSelectCard(doubleUpCount_);
    startDoubleup();
}

THUMB void MaterielMenuPokerSelectcard::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_POKER_SELECT_CARD(&menuItem_, activeCard_);
}

THUMB void MaterielMenuPokerSelectcard::menuDraw()
{
    if (isPlaySound_ != 0) {
        return;
    }
    unkfunc_02177d24(haveCoin_, getCoin_, 1, 0);
    unkfunc_02177bac(0x90, 0, 0x70, 0x30, -1);
    if (animation_ == 0) {
        unkfunc_02177da0(cardCounter_, 1);
    }
    switch (animation_) {
    case 1:
        pokerOpenCard(true);
        break;
    case 2:
        pokerOpenCard(false);
        break;
    case 3:
        pokerReverseCard();
        break;
    case 0:
        menuItem_.drawActive();
        break;
    }
    int betCoin = PokerManager::getSingleton()->betCoin_;
    int combination = PokerManager::getSingleton()->winningCombination_ - 2;
    if (blink_ == 0) {
        combination = -1;
    }
    unkfunc_02177e34(betCoin, combination, blink_);
    unkfunc_02177bac(0, 0, 0x100, 0xc0, -1);
}

THUMB void MaterielMenuPokerSelectcard::menuUpdate()
{
    if (!MenuSoundManager::getSingleton()->isPlaySound()) {
        isPlaySound_ = 0;
        if (!messageUpdate()) {
            statusUpdate();
        }
    }
}

THUMB bool MaterielMenuPokerSelectcard::messageUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (gameMode_ == 7) {
            SoundManager::playSe(0x15e, 0);
            if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 0x400)) {
                status::g_Party.addCasinoCoin(getCoin_);
                getCoin_ = 0;
                haveCoin_ = status::g_Party.casinoCoin_;
                gameMode_ = 8;
                redraw_ = 1;
            } else if (getCoin_ > 0) {
                if (getCoin_ == 1) {
                    status::g_Party.addCasinoCoin(1);
                    getCoin_ = 0;
                } else {
                    status::g_Party.addCasinoCoin(2);
                    getCoin_ -= 2;
                }
                haveCoin_ = status::g_Party.casinoCoin_;
                redraw_ = 1;
            } else {
                gameMode_ = 8;
            }
            return true;
        }
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK) {
            if (gameMode_ == 6) {
                gameMode_ = 7;
                return true;
            }
            data_020ed1bc.close();
            switch (gameMode_) {
            case 0:
                gameMode_ = 1;
                break;
            case 2:
                animation_ = 3;
                gameMode_ = 0;
                break;
            case 3:
                animation_ = 3;
                break;
            case 8:
                showMessage(0xc92f9, -1);
                data_020ed1bc.setYesNo();
                data_020ed1bc.setYesNoPosition(0xc0, 0x40);
                gameMode_ = 3;
                redraw_ = 1;
                break;
            }
            return false;
        }
        if (stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (gameMode_ == 3) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            } else if (gameMode_ == 2) {
                TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
                showMessage(0xc92f1, -1);
                gameMode_ = 6;
                win_ = 1;
            }
        }
        return true;
    }
    if (gameMode_ == 5) {
        doubleupUpdate();
    }
    return false;
}

THUMB void MaterielMenuPokerSelectcard::statusUpdate()
{
    switch (gameMode_) {
    case 8:
        close();
        gMaterielMenu_POKER_BETCOIN.open();
        break;
    case 1: {
        cursor_.setup(4, 1, 4);
        int active = menuItem_.active_;
        int result = MenuUpdate_Assist::menuSelect(menuItem_, cursor_);
        if (result == 0) {
            return;
        }
        if (result == 2) {
            gameMode_ = 4;
            animation_ = 2;
            index_ = 1;
            PokerManager::getSingleton()->setSelectCard(doubleUpCount_, (short)(activeCard_ - 1));
            for (int i = 1; i < 5; i++) {
                int type = PokerManager::getSingleton()->getSelectCardType(doubleUpCount_, i);
                int no = 0;
                if (type != TYPE_JOKER) {
                    no = PokerManager::getSingleton()->getSelectCardNo(doubleUpCount_, i);
                }
                CasinoPokerDraw::getSingleton()->setCardTexture(i, type, no);
            }
            redraw_ = 1;
            return;
        }
        if (menuItem_.active_ == 0) {
            if (active == 1) {
                activeCard_ = 4;
            } else {
                activeCard_ = 1;
            }
        } else {
            activeCard_ = menuItem_.active_;
        }
        redraw_ = 1;
        break;
    }
    }
}

THUMB void MaterielMenuPokerSelectcard::doubleupUpdate()
{
    switch (PokerManager::getSingleton()->getSelectCardResult(doubleUpCount_)) {
    case 0:
        doubleUpCount_++;
        getCoin_ *= 2;
        PokerManager::getSingleton()->setGetCoin(getCoin_);
        winCoin_[doubleUpCount_ - 1] = getCoin_;
        win_ = 1;
        if (doubleUpCount_ >= 11) {
            showMessage(0xc92e7, -1);
            gameMode_ = 6;
            return;
        }
        if (getCoin_ >= 10000) {
            showMessage(0xc92e3, -1);
            gameMode_ = 6;
            return;
        }
        TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
        TextAPI::setMACRO0(0x45, 0xf0000000, getCoin_ * 2);
        TextAPI::setMACRO0(0x2e, 0xf0000000, doubleUpCount_ + 1);
        showMessage(0xc92d7, 0xc92d7 + 2);
        data_020ed1bc.setYesNo();
        data_020ed1bc.setYesNoPosition(0xc0, 0x40);
        gameMode_ = 2;
        menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
        break;
    case 1: {
        int betCoin = PokerManager::getSingleton()->betCoin_;
        status::g_Party.setCasinoCoin(status::g_Party.casinoCoin_ - betCoin);
        getCoin_ = 0;
        doubleUpCount_ = 0;
        PokerManager::getSingleton()->setBetCoin(betCoin, status::g_Party.casinoCoin_);
        showMessage(0xc92f5, -1);
        gameMode_ = 8;
        win_ = 1;
        blink_ = 0;
        menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
        break;
    }
    case 2:
        doubleUpCount_++;
        winCoin_[doubleUpCount_ - 1] = getCoin_;
        if (doubleUpCount_ >= 11) {
            showMessage(0xc92e7, -1);
            gameMode_ = 6;
            win_ = 1;
            return;
        }
        showMessage(0xc92ed, -1);
        gameMode_ = 2;
        SoundManager::playSe(0x469, 0);
        break;
    }
}

THUMB bool MaterielMenuPokerSelectcard::startDoubleup()
{
    int type = PokerManager::getSingleton()->getSelectCardType(doubleUpCount_, 0);
    if (type != TYPE_JOKER) {
        int no = PokerManager::getSingleton()->getSelectCardNo(doubleUpCount_, 0);
        CasinoPokerDraw::getSingleton()->setCardTexture(0, type, no);
        for (int i = 1; i < 5; i++) {
            CasinoPokerDraw::getSingleton()->setCardReverse(i);
        }
        return true;
    }
    CasinoPokerDraw::getSingleton()->setCardJoker(0);
    for (int i = 1; i < 5; i++) {
        int cardType = PokerManager::getSingleton()->getSelectCardType(doubleUpCount_, i);
        int no = PokerManager::getSingleton()->getSelectCardNo(doubleUpCount_, i);
        CasinoPokerDraw::getSingleton()->setCardTexture(i, cardType, no);
    }
    return true;
}

THUMB void MaterielMenuPokerSelectcard::showMessage(int mes1, int mes2)
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(mes1);
    if (mes2 != -1) {
        data_020ed1bc.addMessage(mes2);
    }
}

THUMB void MaterielMenuPokerSelectcard::pokerOpenCard(bool first)
{
    if (first) {
        hopCard(0);
        gyre_++;
        ang_ += 0x800;
        if (gyre_ > 16) {
            gyre_ = 0;
            ang_ = 0x8000;
            animation_ = 0;
            SoundManager::playSe(0x15f, 0);
            showMessage(0xc92dd, -1);
        }
        return;
    }
    if (index_ == PokerManager::getSingleton()->getSelectCard(doubleUpCount_) + 1) {
        index_++;
    }
    if (index_ > 4) {
        hopCard(PokerManager::getSingleton()->getSelectCard(doubleUpCount_) + 1);
    } else {
        hopCard(index_);
    }
    gyre_++;
    ang_ += 0x800;
    if (gyre_ > 16) {
        index_++;
        gyre_ = 0;
        ang_ = 0x8000;
        SoundManager::playSe(0x15f, 0);
        if (index_ > 5) {
            index_ = 0;
            ang_ = 0;
            animation_ = 0;
            if (PokerManager::getSingleton()->getSelectCardResult(doubleUpCount_) == 0) {
                setSoundNo();
                gameMode_ = 5;
                return;
            }
            doubleupUpdate();
        }
    }
}

THUMB void MaterielMenuPokerSelectcard::pokerReverseCard()
{
    hopCard(index_);
    gyre_++;
    ang_ += 0x800;
    if (gyre_ > 16) {
        index_++;
        gyre_ = 0;
        ang_ = 0;
        SoundManager::playSe(0x15f, 0);
        if (index_ > 4) {
            if (gameMode_ == 3) {
                close();
                gMaterielMenu_POKER_BETCOIN.open();
                return;
            }
            index_ = 0;
            gyre_ = 0;
            ang_ = 0x8000;
            animation_ = 1;
            PokerManager::getSingleton()->startSelectCard(doubleUpCount_);
            win_ = 0;
            if (startDoubleup()) {
                menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
                activeCard_ = 1;
            }
        }
    }
}

THUMB void MaterielMenuPokerSelectcard::hopCard(int index)
{
    dss::Fix32Vector3 pos;
    dss::Fix32 space = CasinoPokerDraw::getSingleton()->getSpace();
    dss::Fix32 depth = CasinoPokerDraw::getSingleton()->getDepth();
    if (gyre_ < 8) {
        distance_.value += 0x200;
    }
    if (gyre_ > 8) {
        distance_.value -= 0x200;
    }
    pos.vy = distance_.value;
    pos.vx.value = (index * 2 - 4) * space.value;
    pos.vz.value = depth.value;
    CasinoPokerDraw::getSingleton()->setCardAngle(index, ang_);
    CasinoPokerDraw::getSingleton()->setCardPosition(index, pos);
}

THUMB void MaterielMenuPokerSelectcard::setSoundNo()
{
    isPlaySound_ = 1;
    int getCoin = getCoin_ * 2;
    if (getCoin >= 10000) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_L);
    } else if (getCoin > 500) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_M);
    } else {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_S);
    }
}
