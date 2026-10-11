#include "ov009/casino/Casino_Slot.hpp"
#include "ov009/casino/CasinoSlot.hpp"
#include "ov009/casino/CasinoCamera.hpp"
#include "main/dss/Random.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

static short unk_02125330[6] = {0, 0, 0, 0, -910, 0};

Casino_SlotMachine::SLOT_BINGO_LIST Casino_Slot::bingoList_[9] = {
    Casino_SlotMachine::LOSE,
    Casino_SlotMachine::CHERRY2,
    Casino_SlotMachine::CHERRY3,
    Casino_SlotMachine::POT_HIT,
    Casino_SlotMachine::SWORD_HIT,
    Casino_SlotMachine::RING_HIT,
    Casino_SlotMachine::CROWN_HIT,
    Casino_SlotMachine::BAR_HIT,
    Casino_SlotMachine::SEVEN_HIT,
};

int Casino_Slot::avesTable_[5][9] = {
    {8733, 640, 320, 160, 80, 40, 18, 6, 3},
    {8733, 640, 320, 160, 80, 40, 18, 6, 3},
    {8733, 640, 320, 160, 80, 40, 18, 6, 3},
    {8733, 640, 320, 160, 80, 40, 18, 6, 3},
    {9239, 480, 120, 80, 40, 24, 10, 5, 2},
};

ARM Casino_Slot* Casino_Slot::getSingleton()
{
    static Casino_Slot casinoSlot;
    return &casinoSlot;
}

ARM Casino_Slot::Casino_Slot()
{
    m_result_coin = 1;
    m_bet_coin = 0;
    m_slot_tbl_type = -1;
    m_roll_se_player = 0;
}

ARM Casino_Slot::~Casino_Slot()
{
}

ARM void Casino_Slot::setSlotType(int type)
{
    m_slot_tbl_type = type;
    m_bet_coin = 0;
    m_result_coin = 0;
    m_slot_machine.setupSlot(type);
}

ARM void Casino_Slot::resetSlot()
{
    m_result_coin = 0;
    unkfunc_02123944();
    m_slot_machine.resetSlot();
}

ARM void Casino_Slot::addCoin(int& coin)
{
    int bet = m_bet_coin;
    if (bet >= 5) {
        return;
    }
    if (coin <= 0) {
        return;
    }
    CasinoSlot::getSingleton()->setLineLamp(bet, true);
    SoundManager::playSe(0x160, 0);
    coin--;
    m_bet_coin++;
}

ARM void Casino_Slot::subCoin(int& coin)
{
    if (m_bet_coin <= 0) {
        return;
    }
    coin++;
    m_bet_coin--;
    CasinoSlot::getSingleton()->setLineLamp(m_bet_coin, false);
    SoundManager::playSe(0x161, 0);
}

ARM void Casino_Slot::unkfunc_02123944()
{
    m_bet_coin = 0;
    for (int i = 0; i < 5; i++) {
        CasinoSlot::getSingleton()->setLineLamp(i, false);
    }
}

ARM int Casino_Slot::getResultAllCoin()
{
    m_effect_flag = 0;
    for (int i = 0; i < m_bet_coin; i++) {
        int coin = m_slot_machine.getResultCoin(i);
        if (coin == 1000) {
            m_effect_flag = 1;
        }
        m_result_coin += coin;
    }
    return m_result_coin;
}

ARM bool Casino_Slot::runningSlot()
{
    bool stop = m_slot_machine.scrollSlot();
    if (stop) {
        SoundManager::stopSeWithIndex(0x162, 0);
    }
    return stop;
}

ARM void Casino_Slot::cashCoin(int& coin)
{
    if (m_result_coin <= 0) {
        return;
    }
    coin++;
    if (coin > 999999) {
        coin = 999999;
    }
    SoundManager::playSe(0x15e, 0);
    m_result_coin--;
}

ARM void Casino_Slot::cashAllCoin(int& coin)
{
    coin += m_result_coin;
    if (coin > 999999) {
        coin = 999999;
    }
    m_result_coin = 0;
}

ARM int Casino_Slot::startSlot()
{
    status::g_Party.setPlayerMode();
    int luck = status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.getLuck();
    m_luck_retry_count = (luck - 40) / 100;
    int random = dssrand::rand(10000);
    int total = 0;
    int bingo = 0;
    while (bingo < 9) {
        total += avesTable_[m_slot_tbl_type][bingo];
        if (total >= random) {
            bool retry = false;
            if (m_luck_retry_count > 0) {
                retry = dssrand::rand(0x400) < luck;
            }
            if (bingo != 0 || !retry) {
                break;
            }
            random = dssrand::rand(10000);
            total = 0;
            bingo = 0;
            m_luck_retry_count--;
        } else {
            bingo++;
        }
    }
    m_slot_machine.resetSlot();
    m_slot_machine.setLotResult(bingoList_[bingo]);
    m_slot_machine.setStopPosition();
    m_result_coin = 0;
    m_roll_se_player = SoundManager::playSe(0x162, 0);
    return bingo;
}

ARM bool Casino_Slot::showEffect()
{
    if (m_effect_flag) {
        int angle = -0x50000;
        if (m_effect_counter < 120) {
            angle = (m_effect_counter << 14) - 0x50000;
            if (angle >= 0x10000) {
                angle = 0x10000;
            }
        } else {
            int down = 0x10000 - ((m_effect_counter - 120) << 14);
            if (down > angle) {
                angle = down;
            }
        }
        CasinoCamera::getSingleton()->camera_.setRotXYZ(dss::Vector3<short>(angle / 360, unk_02125330[1], unk_02125330[2]));
        if (m_effect_counter >= 240 && m_result_coin == 0) {
            CasinoCamera::getSingleton()->camera_.setRotXYZ(dss::Vector3<short>(unk_02125330[4], unk_02125330[3], unk_02125330[0]));
            m_effect_counter = 0;
            CasinoSlot::getSingleton()->stopEventAnim();
            return true;
        }
    } else if (m_effect_counter >= 90 && m_result_coin == 0) {
        m_effect_counter = 0;
        CasinoSlot::getSingleton()->stopEventAnim();
        return true;
    }
    m_effect_counter++;
    return false;
}
