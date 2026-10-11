#include "ov016/casino/MaterielMenuPoker/MaterielMenuPokerBetcoin.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/MaterielMenu_WINDOW_MANAGER/MaterielMenu_WINDOW_MANAGER.hpp"
#include "ov009/casino/PokerManager.hpp"
#include "ov009/casino/CasinoPokerDraw.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"

THUMB void MaterielMenuPokerBetcoin::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    betCoin_ = PokerManager::getSingleton()->betCoin_;
    for (int i = 0; i < 5; i++) {
        CasinoPokerDraw::getSingleton()->setCardTexture(i, 4, 1);
    }
    haveCoin_ = status::g_Party.casinoCoin_ - betCoin_;
    messageMode_ = 0;
    unk_28 = 0;
    MenuSoundManager::getSingleton()->initialize();
}

THUMB void MaterielMenuPokerBetcoin::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_POKER_BET_COIN(&menuItem_, menuItem_.active_);
}

THUMB void MaterielMenuPokerBetcoin::menuDraw()
{
    if (unk_28 > 15) {
        unkfunc_02177d24(haveCoin_, 0xffff, 0, 0);
    } else {
        unkfunc_02177d24(haveCoin_, betCoin_, 0, 0);
    }
    unkfunc_02177bac(0x90, 0, 0x70, 0x30, -1);
    unkfunc_02177e34(betCoin_, -1, 0);
    unkfunc_02177bac(0, 0, 0x100, 0xc0, -1);
    menuItem_.drawActive();
}

THUMB void MaterielMenuPokerBetcoin::menuUpdate()
{
    if (!unkfunc_021700f0()) {
        unkfunc_02170024();
    }
}

THUMB bool MaterielMenuPokerBetcoin::unkfunc_02170024()
{
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK) {
            if (messageMode_ != 2) {
                data_020ed1bc.close();
            }
            if (messageMode_ == 3) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
            if (messageMode_ == 1) {
                if (status::g_Party.casinoCoin_ == 0) {
                    showMessage(0xc92ca);
                    messageMode_ = 3;
                } else {
                    data_020ed1bc.openMessageForMENU();
                    data_020ed1bc.addMessageNOWAIT(0xc92cd);
                    data_020ed1bc.addMessageWAITKEY();
                    messageMode_ = 2;
                }
            }
            return true;
        }
        if (stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (messageMode_ == 1) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        }
        return true;
    }
    if (messageMode_ == 0) {
        if (status::g_Party.casinoCoin_ == 0) {
            showMessage(0xc92ca);
            messageMode_ = 3;
        } else {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessageNOWAIT(0xc92cd);
            data_020ed1bc.addMessageWAITKEY();
            messageMode_ = 2;
        }
        return true;
    }
    return false;
}

THUMB bool MaterielMenuPokerBetcoin::unkfunc_021700f0()
{
    if (!data_020ed1bc.isOpen()) {
        return false;
    }
    if (!data_020ed1bc.isMessageWAITPROG()) {
        return false;
    }
    if (unk_28 > 30) {
        unk_28 = 0;
        redraw_ = 1;
    } else if (++unk_28 > 15) {
        redraw_ = 1;
    }
    cursor_.setup(1, 1, 1);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, cursor_);
    if (result != 0) {
        if (result == 4) {
            haveCoin_--;
            betCoin_++;
            if (betCoin_ > status::g_Party.casinoCoin_ || betCoin_ > 10) {
                betCoin_--;
                haveCoin_++;
                PokerManager::getSingleton()->setBetCoin(betCoin_, status::g_Party.casinoCoin_);
                PokerManager::getSingleton()->cardPosition_ = 0;
                data_020ed1bc.close();
                data_020ed1bc.clearMessageWAITPROG();
                close();
                gMaterielMenu_POKER_CHANGECARD.open();
            } else {
                SoundManager::playSe(0x15e, 0);
            }
        }
        if (result == 5) {
            haveCoin_++;
            if (--betCoin_ < 0) {
                haveCoin_--;
                betCoin_ = 0;
            } else {
                SoundManager::playSe(0x15e, 0);
            }
        }
        if (result == 2) {
            int bet = betCoin_;
            if (bet == 0) {
                showMessage(0xc92c5);
                messageMode_ = 3;
                unk_28 = 0;
                return false;
            }
            PokerManager::getSingleton()->setBetCoin(bet, status::g_Party.casinoCoin_);
            PokerManager::getSingleton()->cardPosition_ = 0;
            data_020ed1bc.close();
            data_020ed1bc.clearMessageWAITPROG();
            close();
            gMaterielMenu_POKER_CHANGECARD.open();
        }
        if (result == 3) {
            showMessage(0xc92f9);
            data_020ed1bc.setYesNo();
            data_020ed1bc.setYesNoPosition(0xc0, 0x40);
            messageMode_ = 1;
            unk_28 = 0;
            return false;
        }
        redraw_ = 1;
    }
    return true;
}

THUMB void MaterielMenuPokerBetcoin::showMessage(int mes)
{
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(mes);
}
