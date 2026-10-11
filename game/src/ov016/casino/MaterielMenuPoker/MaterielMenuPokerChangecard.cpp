#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerChangecard.hpp"
#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerBetcoin.hpp"
#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerHiandlow.hpp"
#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerSelectcard.hpp"
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

THUMB void MaterielMenuPokerChangecard::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    menuItem2_.active_ = 0;
    cursor_.setupBase();
    cursor2_.setupBase();
    gameMode_ = 0;
    animation_ = 1;
    combination_ = -1;
    blink_ = 0;
    isPlaySound_ = 0;
    betCoin_ = PokerManager::getSingleton()->betCoin_;
    getCoin_ = PokerManager::getSingleton()->betCoin_;
    haveCoin_ = status::g_Party.casinoCoin_ - betCoin_;
    ang_ = 0x8000;
    gyre_ = 0;
    index_ = 0;
    distance_ = CasinoPokerDraw::getSingleton()->getDistance();
    PokerManager::getSingleton()->dealCard(-1);
    for (int i = 0; i < 5; i++) {
        change_[i] = 0;
    }
}

THUMB void MaterielMenuPokerChangecard::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_POKER_SELECT_CARD(&menuItem_, menuItem_.active_);
    MenuTemplate_materiel::MATERIEL_POKER_SELECT_DEAL(&menuItem2_, menuItem2_.active_);
}

THUMB void MaterielMenuPokerChangecard::menuDraw()
{
    if (isPlaySound_ == 0) {
        switch (animation_) {
        case 0:
            if ((gameMode_ != 3 || gameMode_ != 4) && isPlaySound_ == 0) {
                unkfunc_02177da0(change_, 0);
                menuItem_.drawActive();
                menuItem2_.drawActive();
            }
            break;
        case 1:
            pokerDealCard();
            break;
        case 2:
            pokerChangeCard();
            break;
        case 4:
            pokerReverseCard(false);
            break;
        case 5:
            pokerReverseCard(true);
            break;
        }
    }
    unkfunc_02177d24(haveCoin_, getCoin_, blink_, blink_);
    unkfunc_02177bac(0x90, 0, 0x70, 0x30, -1);
    unkfunc_02177e34(PokerManager::getSingleton()->betCoin_, combination_, blink_);
    unkfunc_02177bac(0, 0, 0x100, 0xc0, -1);
}

THUMB void MaterielMenuPokerChangecard::menuUpdate()
{
    if (MenuSoundManager::getSingleton()->isPlaySound()) {
        return;
    }
    isPlaySound_ = 0;
    if (blink_ == 1 && animation_ == 0) {
        if (effectCount_ > 90) {
            for (int i = 0; i < 5; i++) {
                if (PokerManager::getSingleton()->combinationCard_[i] == 1) {
                    CasinoPokerDraw::getSingleton()->setEffect(i);
                }
            }
            effectCount_ = 0;
        } else {
            effectCount_++;
        }
    }
    if (animation_ == 1 || animation_ == 2) {
        return;
    }
    if (!messageUpdata()) {
        menuUpdata();
    }
}

THUMB bool MaterielMenuPokerChangecard::messageUpdata()
{
    if (data_020ed1bc.isOpen()) {
        if (gameMode_ == 6) {
            SoundManager::playSe(0x15e, 0);
            if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 0x400)) {
                status::g_Party.addCasinoCoin(getCoin_);
                getCoin_ = 0;
                haveCoin_ = status::g_Party.casinoCoin_;
                gameMode_ = 7;
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
                gameMode_ = 7;
            }
            return true;
        }
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK) {
            if (gameMode_ == 5) {
                gameMode_ = 6;
                return true;
            }
            data_020ed1bc.close();
            if (gameMode_ == 3) {
                animation_ = 5;
            } else if (gameMode_ == 4) {
                animation_ = 4;
                redraw_ = 1;
            } else if (gameMode_ == 7) {
                showMessage(0xc92f9, -1);
                data_020ed1bc.setYesNo();
                data_020ed1bc.setYesNoPosition(0xc0, 0x40);
                betCoin_ = 0;
                getCoin_ = 0;
                gameMode_ = 4;
                redraw_ = 1;
            }
        } else if (stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (gameMode_ == 3) {
                TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
                showMessage(0xc92f1, -1);
                gameMode_ = 5;
            } else if (gameMode_ == 4) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        }
        return true;
    }
    if (gameMode_ == 0) {
        showMessage(0xc92d1, -1);
        gameMode_ = 1;
    }
    if ((gameMode_ == 2 && animation_ == 2) || animation_ == 4) {
        return true;
    }
    if (gameMode_ == 2 && animation_ == 3) {
        combination_ = PokerManager::getSingleton()->judgementCombination();
        if (combination_ == 0) {
            combination_ = -1;
            blink_ = 0;
            showMessage(0xc92f5, 0xc92f5 + 4);
            data_020ed1bc.setYesNo();
            data_020ed1bc.setYesNoPosition(0xc0, 0x40);
            gameMode_ = 4;
            status::g_Party.setCasinoCoin(haveCoin_);
            PokerManager::getSingleton()->setBetCoin(betCoin_, status::g_Party.casinoCoin_);
            getCoin_ = 0;
        } else {
            combination_ -= 2;
            blink_ = 1;
            getCoin_ = PokerManager::getSingleton()->getMultiple() * betCoin_;
            PokerManager::getSingleton()->setGetCoin(getCoin_);
            TextAPI::setMACRO0(0x48, 0xf0000000, getCoin_);
            TextAPI::setMACRO0(0x45, 0xf0000000, getCoin_ * 2);
            TextAPI::setMACRO0(0x2e, 0xf0000000, 1);
            showMessage(0xc92d7, 0xc92d7 + 2);
            data_020ed1bc.setYesNo();
            data_020ed1bc.setYesNoPosition(0xc0, 0x40);
            gameMode_ = 3;
        }
        animation_ = 0;
    }
    return false;
}

THUMB void MaterielMenuPokerChangecard::menuUpdata()
{
    cursor_.setup(5, 1, 5);
    if (gameMode_ == 1) {
        int result = MenuUpdate_Assist::menuSelect(menuItem_, cursor_);
        if (result != 0) {
            if (result == 4 || result == 5) {
                int active = menuItem_.active_;
                PokerManager::getSingleton()->cardPosition_ = active;
                menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
                menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
                menuItem2_.result_ = 0;
                menuItem2_.lastresult_ = 0;
                gameMode_ = 2;
            } else if (result == 2) {
                if (change_[menuItem_.active_] == 0) {
                    change_[menuItem_.active_] = 1;
                } else {
                    change_[menuItem_.active_] = 0;
                }
            }
            redraw_ = 1;
            return;
        }
    }
    cursor2_.setup(1, 1, 1);
    if (gameMode_ == 2) {
        int result = MenuUpdate_Assist::menuSelect(menuItem2_, cursor2_);
        if (result != 0) {
            if (result == 4 || result == 5) {
                menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
                menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
                menuItem_.active_ = PokerManager::getSingleton()->cardPosition_;
                menuItem_.result_ = 0;
                menuItem_.lastresult_ = 0;
                gameMode_ = 1;
            } else if (result == 2) {
                menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_NONE);
                changeCard();
            }
            redraw_ = 1;
        }
    }
}

THUMB void MaterielMenuPokerChangecard::changeCard()
{
    for (int i = 0; i < 5; i++) {
        if (change_[i] == 0) {
            PokerManager::getSingleton()->dealCard(i);
        }
    }
    PokerManager::getSingleton()->clearDebugCard(-1);
    animation_ = 2;
}

THUMB void MaterielMenuPokerChangecard::showMessage(int mes1, int mes2)
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(mes1);
    if (mes2 != -1) {
        data_020ed1bc.addMessage(mes2);
    }
}

THUMB void MaterielMenuPokerChangecard::pokerDealCard()
{
    hopCard(index_);
    gyre_++;
    ang_ += 0x800;
    if (gyre_ == 8) {
        int type = PokerManager::getSingleton()->getCardType(index_);
        int no;
        if (type == TYPE_JOKER) {
            no = 0;
        } else {
            no = PokerManager::getSingleton()->getCardNo(index_);
        }
        CasinoPokerDraw::getSingleton()->setCardTexture(index_, type, no);
    }
    if (gyre_ > 16) {
        index_++;
        gyre_ = 0;
        ang_ = 0x8000;
        SoundManager::playSe(0x15f, 0);
        if (index_ > 4) {
            index_ = 0;
            ang_ = 0;
            animation_ = 0;
            combination_ = PokerManager::getSingleton()->judgementCombination();
            if (combination_ != 0) {
                getCoin_ = PokerManager::getSingleton()->getMultiple() * betCoin_;
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_S);
                combination_ -= 2;
                isPlaySound_ = 1;
                blink_ = 1;
                return;
            }
            combination_ = -1;
        }
    }
}

THUMB void MaterielMenuPokerChangecard::pokerChangeCard()
{
    static int reverse;
    if (!reverse) {
        while (change_[index_] != 0) {
            index_++;
            if (index_ > 4) {
                index_ = 0;
                ang_ = 0x8000;
                reverse = 1;
                return;
            }
        }
        hopCard(index_);
        gyre_++;
        ang_ += 0x800;
        if (gyre_ == 8) {
            int type = PokerManager::getSingleton()->getCardType(index_);
            int no = 0;
            if (type != TYPE_JOKER) {
                no = PokerManager::getSingleton()->getCardNo(index_);
            }
            CasinoPokerDraw::getSingleton()->setCardTexture(index_, type, no);
        }
        if (gyre_ > 16) {
            index_++;
            gyre_ = 0;
            ang_ = 0;
            SoundManager::playSe(0x15f, 0);
            if (index_ > 4) {
                index_ = 0;
                ang_ = 0x8000;
                reverse = 1;
            }
        }
        return;
    }
    while (change_[index_] != 0) {
        index_++;
        if (index_ > 4) {
            index_ = 0;
            ang_ = 0;
            reverse = 0;
            animation_ = 3;
            if (PokerManager::getSingleton()->judgementCombination()) {
                setSoundNo();
            }
            return;
        }
    }
    hopCard(index_);
    gyre_++;
    ang_ += 0x800;
    if (gyre_ > 16) {
        index_++;
        gyre_ = 0;
        ang_ = 0x8000;
        SoundManager::playSe(0x15f, 0);
        if (index_ > 4) {
            index_ = 0;
            ang_ = 0;
            reverse = 0;
            animation_ = 3;
            if (PokerManager::getSingleton()->judgementCombination()) {
                setSoundNo();
            }
        }
    }
}

THUMB void MaterielMenuPokerChangecard::pokerReverseCard(bool doubleupNext)
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
            if (doubleupNext) {
                close();
                if (PokerManager::getSingleton()->groundSlum_) {
                    gMaterielMenu_POKER_HIANDLOW.open();
                } else {
                    gMaterielMenu_POKER_SELECTCARD.open();
                }
                return;
            }
            close();
            gMaterielMenu_POKER_BETCOIN.open();
        }
    }
}

THUMB void MaterielMenuPokerChangecard::hopCard(int index)
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

THUMB void MaterielMenuPokerChangecard::setSoundNo()
{
    isPlaySound_ = 1;
    int getCoin = PokerManager::getSingleton()->getMultiple() * betCoin_;
    if (getCoin >= 10000) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_L);
    } else if (getCoin >= 500) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_M);
    } else {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_S);
    }
}
