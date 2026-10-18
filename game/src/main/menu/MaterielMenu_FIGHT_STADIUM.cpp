#include "main/menu/MaterielMenu_FIGHT_STADIUM.hpp"
#include "main/dss/Pad.hpp"
#include "ov016/casino/FightStadiumManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/global/Global.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDrawExtra.hpp"

const int MaterielMenu_FIGHT_STADIUM::DOUBLEUP_COIN_MAX = 10000;
const int MaterielMenu_FIGHT_STADIUM::BET_COIN_MAX = 50;
const int MaterielMenu_FIGHT_STADIUM::BET_CURSOR_Y = 42;
const int MaterielMenu_FIGHT_STADIUM::BET_CURSOR_X = 232;

THUMB void MaterielMenu_FIGHT_STADIUM::menuSetup()
{
    status::g_Party.setPlayerMode();
    betItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_UP);
    monsterItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    betItem_.active_ = 1;
    status_ = FIGHT_STADIUM_START;
    messageCount_ = -1;
    haveCoin_ = status::g_Party.casinoCoin_;
    if (g_Global.fightStadiumResult_ != 0) {
        playBackMenu(g_Global.fightStadiumResult_);
        g_Global.fightStadiumResult_ = 0;
        g_Global.fightStadiumFlag_ = 0;
    }
    blink_ = 0;
    blinkCount_ = 0;
}

THUMB void MaterielMenu_FIGHT_STADIUM::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_MONSTER_LIST(&monsterItem_, monsterItem_.active_, FightStadiumManager::getSingleton()->cardCount_);
    MenuTemplate_materiel::MATERIEL_BET_COIN(&betItem_, betItem_.active_, BET_FIGURE_MAX, BET_CURSOR_X, BET_CURSOR_Y);
}

THUMB void MaterielMenu_FIGHT_STADIUM::menuDraw()
{
    int flag = 0;
    drawFightStadiumBetDigits(haveCoin_, 0, 0, 0);
    if (status_ == FIGHT_STADIUM_BET || status_ == FIGHT_STADIUM_RESULT || (status_ == FIGHT_STADIUM_RETRY && messageCount_ != 0)) {
        flag = 1;
        drawFightStadiumBetDigits(g_Global.betCoin_, 0, 0x20, flag);
        drawFightStadiumPanels(status_ == FIGHT_STADIUM_BET);
        if (status_ == FIGHT_STADIUM_BET) {
            if (blink_ != 0) {
                if (blinkCount_ > 15) {
                    betItem_.drawActive();
                }
                if (blinkCount_ == 0 || blinkCount_ == 16) {
                    redraw_ = 1;
                }
                blinkCount_++;
                if (blinkCount_ > 30) {
                    blinkCount_ = 0;
                }
            } else {
                betItem_.drawActive();
            }
        }
        if (status_ == FIGHT_STADIUM_BET) {
            drawFrameStyleA(0xb8, 0, 0x48, 0x40, -1);
        } else {
            drawFrameStyleA(0xb0, 0, 0x50, 0x40, -1);
        }
    } else {
        drawFrameStyleA(0xb8, 0, 0x48, 0x20, -1);
    }
    if (status_ == FIGHT_STADIUM_CHOICE || status_ == FIGHT_STADIUM_BET) {
        int count = FightStadiumManager::getSingleton()->cardCount_;
        monsterItem_.drawActive();
        for (int i = 0; i < count; i++) {
            int monsterID = FightStadiumManager::getSingleton()->getMonsterID(i);
            int diameter = FightStadiumManager::getSingleton()->getDiameter(i);
            drawFightStadiumEntry(monsterID, diameter, i, FightStadiumManager::getSingleton()->getOrderCount(i));
        }
        drawFrameStyleA(0, 0, 0xb8, count * 16 + 0x30, -1);
        drawFightStadiumLayout(flag);
    }
    if (status_ == FIGHT_STADIUM_BET) {
        drawFightStadiumCursor(g_Global.betOnIndex_);
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::menuUpdate()
{
    if (!messageUpdate()) {
        statusUpdate();
    }
}

THUMB bool MaterielMenu_FIGHT_STADIUM::messageUpdate()
{
    if (!g_TownMenuMessage.isOpen()) {
        return false;
    }
    int stat = g_TownMenuMessage.stat_;
    switch (status_) {
    case FIGHT_STADIUM_START:
    case FIGHT_STADIUM_BATTLE:
    case FIGHT_STADIUM_END:
        closeMessage();
        break;
    case FIGHT_STADIUM_CHOICE:
    case FIGHT_STADIUM_BET:
        if ((unsigned int)(stat - 1) <= 1) {
            g_TownMenuMessage.close();
            g_TownMenuMessage.setMessageLastCursor(false);
        }
        return false;
    case FIGHT_STADIUM_RESULT:
        if (messageCount_ == 1) {
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                g_Global.doubleUpFlag_ = 1;
                status_ = FIGHT_STADIUM_START;
                messageCount_ = -1;
                messageCount_++;
            } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                g_Global.doubleUpFlag_ = 0;
                status_ = FIGHT_STADIUM_RETRY;
                messageCount_ = -1;
                return true;
            }
        } else {
            closeMessage();
        }
        break;
    case FIGHT_STADIUM_RETRY:
        if (messageCount_ != 0) {
            if (g_Global.betCoin_ > 0) {
                if ((dss::g_Pad.edge() & 1) || (dss::g_Pad.edge() & 0x400)) {
                    SoundManager::playSe(0x15e, 0);
                    haveCoin_ += g_Global.betCoin_;
                    g_Global.betCoin_ = 0;
                } else {
                    SoundManager::playSe(0x15e, 0);
                    haveCoin_++;
                    g_Global.betCoin_--;
                }
                if (haveCoin_ > 999999) {
                    haveCoin_ = 999999;
                }
                status::g_Party.setCasinoCoin(haveCoin_);
                redraw_ = 1;
            } else {
                closeMessage();
            }
        } else if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            g_Global.doubleUpFlag_ = 0;
            status_ = FIGHT_STADIUM_START;
            messageCount_ = -1;
            messageCount_++;
        } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            showMessage(0xc8f16);
            status_ = FIGHT_STADIUM_END;
            messageCount_ = -1;
            return true;
        }
        break;
    }
    return true;
}

THUMB void MaterielMenu_FIGHT_STADIUM::statusUpdate()
{
    switch (status_) {
    case FIGHT_STADIUM_START:
        if (messageCount_ == -1) {
            FightStadiumManager::getSingleton()->setup();
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessage(0xc8eda);
            g_TownMenuMessage.addMessage(0xc8edb);
            messageCount_++;
            g_Global.betOnIndex_ = -1;
            g_Global.betCoin_ = 0;
            g_Global.doubleUpFlag_ = 0;
            result_ = RESULT_NONE;
        } else {
            showMessage(0xc8edd);
            g_TownMenuMessage.addMessageWAITKEY();
            monsterItem_.active_ = 0;
            monsterItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_INACTIVE);
            status_ = FIGHT_STADIUM_CHOICE;
            messageCount_ = -1;
        }
        break;
    case FIGHT_STADIUM_CHOICE:
        monsterListUpdate();
        break;
    case FIGHT_STADIUM_BET:
        coinUpdate();
        break;
    case FIGHT_STADIUM_BATTLE:
        battleStart();
        break;
    case FIGHT_STADIUM_RESULT:
        resultUpdate();
        break;
    case FIGHT_STADIUM_RETRY:
        showMessage(0xc8ef8);
        g_TownMenuMessage.setYesNo();
        messageCount_++;
        break;
    case FIGHT_STADIUM_END:
        close();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::monsterListUpdate()
{
    if (!g_TownMenuMessage.isOpen()) {
        return;
    }
    int oldWaitProg = waitProg_;
    waitProg_ = g_TownMenuMessage.isMessageWAITPROG();
    if (waitProg_ == 0) {
        return;
    }
    if (oldWaitProg == 0 && waitProg_ != 0) {
        int active = monsterItem_.active_;
        monsterItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
        monsterItem_.active_ = active;
    }
    monsterItem_.execInput();
    switch (monsterItem_.result_) {
    case 3:
        g_TownMenuMessage.close();
        monsterItem_.result_ = 0;
        monsterItem_.lastresult_ = 0;
        if (g_Global.doubleUpFlag_ != 0) {
            showMessage(0xc8ee3);
            g_TownMenuMessage.addMessageWAITKEY();
        } else {
            showMessage(0xc8f16);
            status_ = FIGHT_STADIUM_END;
            messageCount_ = -1;
        }
        redraw_ = 1;
        break;
    case 2: {
        monsterItem_.result_ = 0;
        monsterItem_.lastresult_ = 0;
        short index = monsterItem_.active_;
        g_Global.betOnIndex_ = index;
        g_Global.betMonsterID_ = FightStadiumManager::getSingleton()->getMonsterID(index);
        g_Global.betMonsterSymbol_ = FightStadiumManager::getSingleton()->getOrderCount(index);
        monsterItem_.active_ = -1;
        status_ = FIGHT_STADIUM_BET;
        messageCount_ = -1;
        if (g_Global.doubleUpFlag_ != 0) {
            if (FightStadiumManager::getSingleton()->orderNumber_[index] != -1) {
                int orderCount = FightStadiumManager::getSingleton()->getOrderCount(index);
                TextAPI::setMACRO0(3, 0x60000000, FightStadiumManager::getSingleton()->getMonsterID(index), orderCount);
            } else {
                TextAPI::setMACRO0(3, 0x60000000, FightStadiumManager::getSingleton()->getMonsterID(index));
            }
            TextAPI::setMACRO0(0x47, 0xf0000000, g_Global.betCoin_);
            showMessage(0xc8ee9);
            status_ = FIGHT_STADIUM_BATTLE;
            messageCount_ = -1;
            status::g_Party.setCasinoCoin(haveCoin_);
            break;
        }
        if (status::g_Party.casinoCoin_ == 0) {
            showMessage(0xc8ee0);
            status_ = FIGHT_STADIUM_END;
            messageCount_ = -1;
        } else {
            showMessage(0xc8ee6);
            g_TownMenuMessage.addMessageWAITKEY();
            g_TownMenuMessage.setMessageLastCursor(true);
            messageCount_++;
        }
        redraw_ = 1;
        break;
    }
    case 5: {
        int count = FightStadiumManager::getSingleton()->cardCount_;
        monsterItem_.result_ = 0;
        monsterItem_.lastresult_ = 0;
        monsterItem_.active_ = count - 1;
        break;
    }
    case 6:
        monsterItem_.result_ = 0;
        monsterItem_.lastresult_ = 0;
        monsterItem_.active_ = 0;
        break;
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::coinUpdate()
{
    int betCoin = g_Global.betCoin_;
    int figure[BET_FIGURE_MAX];
    figure[0] = betCoin / 10;
    figure[1] = betCoin % 10;
    int figureMax[BET_FIGURE_MAX] = { 5, 9 };
    if (g_TownMenuMessage.isOpen()) {
        int oldWaitProg = waitProg_;
        waitProg_ = g_TownMenuMessage.isMessageWAITPROG();
        if (waitProg_ != 0) {
            if (oldWaitProg == 0 && waitProg_ != 0) {
                blink_ = 1;
            }
            betItem_.execInput();
            int result = betItem_.result_;
            if (result != 0) {
                blink_ = 0;
            }
            switch (result) {
            case 3:
                g_Global.betCoin_ = 0;
                haveCoin_ = status::g_Party.casinoCoin_;
                g_TownMenuMessage.close();
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                status_ = FIGHT_STADIUM_START;
                messageCount_ = -1;
                messageCount_ = 0;
                redraw_ = 1;
                return;
            case 2:
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                betItem_.active_ = 1;
                status_ = FIGHT_STADIUM_BET;
                messageCount_ = -1;
                if (g_Global.betCoin_ == 0) {
                    g_TownMenuMessage.close();
                    betItem_.result_ = 0;
                    betItem_.lastresult_ = 0;
                    status_ = FIGHT_STADIUM_START;
                    messageCount_ = -1;
                    messageCount_ = 0;
                } else {
                    int index = g_Global.betOnIndex_;
                    if (FightStadiumManager::getSingleton()->orderNumber_[index] != -1) {
                        int orderCount = FightStadiumManager::getSingleton()->getOrderCount(index);
                        TextAPI::setMACRO0(3, 0x60000000, FightStadiumManager::getSingleton()->getMonsterID(index), orderCount);
                    } else {
                        TextAPI::setMACRO0(3, 0x60000000, FightStadiumManager::getSingleton()->getMonsterID(index));
                    }
                    TextAPI::setMACRO0(0x47, 0xf0000000, g_Global.betCoin_);
                    showMessage(0xc8ee9);
                    status_ = FIGHT_STADIUM_BATTLE;
                    messageCount_ = -1;
                    status::g_Party.setCasinoCoin(haveCoin_);
                }
                redraw_ = 1;
                return;
            case 8:
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                betItem_.active_ = 1;
                redraw_ = 1;
                break;
            case 7:
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                betItem_.active_ = 0;
                redraw_ = 1;
                break;
            case 5:
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                figure[betItem_.active_]++;
                redraw_ = 1;
                break;
            case 6:
                betItem_.result_ = 0;
                betItem_.lastresult_ = 0;
                figure[betItem_.active_]--;
                if (figure[betItem_.active_] < 0) {
                    figure[betItem_.active_] = figureMax[betItem_.active_];
                }
                redraw_ = 1;
                break;
            }
            int coin = figure[0] * 10;
            coin += figure[1] % 10;
            g_Global.betCoin_ = coin;
            if (g_Global.betCoin_ > BET_COIN_MAX) {
                g_Global.betCoin_ = BET_COIN_MAX;
            }
            if (g_Global.betCoin_ > status::g_Party.casinoCoin_) {
                g_Global.betCoin_ = status::g_Party.casinoCoin_;
            }
            haveCoin_ += betCoin - g_Global.betCoin_;
        }
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::resultUpdate()
{
    FightStadiumManager::getSingleton()->setup();
    if (result_ == RESULT_WIN) {
        g_TownMenuMessage.openMessageForTALK();
        if (messageCount_ == -1) {
            TextAPI::setMACRO0(0x48, 0xf0000000, g_Global.betCoin_);
            g_TownMenuMessage.addMessage(0xc8eee);
            messageCount_++;
        } else if (g_Global.betCoin_ > DOUBLEUP_COIN_MAX) {
            g_Global.doubleUpFlag_ = 0;
            status_ = FIGHT_STADIUM_RETRY;
            messageCount_ = -1;
            g_TownMenuMessage.addMessage(0xc8ef1);
        } else {
            messageCount_++;
            g_TownMenuMessage.addMessage(0xc8ef4);
            g_TownMenuMessage.setYesNo();
        }
    } else if (result_ == RESULT_LOSE) {
        showMessage(0xc8efd);
        g_Global.doubleUpFlag_ = 0;
        status_ = FIGHT_STADIUM_RETRY;
        messageCount_ = -1;
    } else if (result_ == RESULT_DRAW) {
        if (g_Global.doubleUpFlag_ != 0) {
            status_ = FIGHT_STADIUM_START;
            messageCount_ = -1;
            messageCount_++;
        } else {
            haveCoin_ += g_Global.betCoin_;
            status::g_Party.setCasinoCoin(haveCoin_);
            g_Global.betCoin_ = 0;
            status_ = FIGHT_STADIUM_RETRY;
            messageCount_ = -1;
        }
    } else if (result_ == RESULT_RETIRE) {
        showMessage(0xc8f04);
        status_ = FIGHT_STADIUM_END;
        messageCount_ = -1;
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::battleStart()
{
    g_Global.diameter_ = FightStadiumManager::getSingleton()->getDiameter(g_Global.betOnIndex_);
    FightStadiumManager::getSingleton()->battleStart();
    close();
    MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
}

THUMB void MaterielMenu_FIGHT_STADIUM::playBackMenu(int result)
{
    status_ = FIGHT_STADIUM_RESULT;
    messageCount_ = -1;
    result_ = result;
    if (result_ == RESULT_WIN) {
        int coin = g_Global.diameter_ * g_Global.betCoin_;
        if (coin % 10 > 4) {
            coin += 10;
        }
        g_Global.betCoin_ = coin / 10;
    } else if (result_ == RESULT_LOSE) {
        g_Global.betCoin_ = 0;
    }
}

THUMB void MaterielMenu_FIGHT_STADIUM::showMessage(int messageID)
{
    g_TownMenuMessage.openMessageForTALK();
    g_TownMenuMessage.addMessage(messageID);
}

THUMB void MaterielMenu_FIGHT_STADIUM::closeMessage()
{
    if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
        g_TownMenuMessage.close();
    }
}
