#pragma once
#include <globaldefs.h>
#include "ov009/casino/Casino_SlotMachine.hpp"

struct Casino_Slot {
    int m_slot_tbl_type;                        // 0x00
    int unk_04;                                 // 0x04
    int m_bet_coin;                             // 0x08
    int m_result_coin;                          // 0x0C
    short m_effect_counter;                     // 0x10
    int m_effect_flag;                          // 0x14
    int m_luck_retry_count;                     // 0x18
    int m_roll_se_player;                       // 0x1C
    Casino_SlotMachine m_slot_machine;          // 0x20

    static Casino_SlotMachine::SLOT_BINGO_LIST bingoList_[9];
    static int avesTable_[5][9];

    Casino_Slot();
    ~Casino_Slot();
    static Casino_Slot* getSingleton();
    void setSlotType(int type);
    void resetSlot();
    void addCoin(int& coin);
    void subCoin(int& coin);
    void unkfunc_02123944();
    int getResultAllCoin();
    bool runningSlot();
    void cashCoin(int& coin);
    void cashAllCoin(int& coin);
    int startSlot();
    bool showEffect();
};
