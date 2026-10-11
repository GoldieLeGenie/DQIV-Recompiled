#pragma once
#include <globaldefs.h>
#include "ov009/casino/Casino_SlotReel.hpp"

struct Casino_SlotMachine {
    enum SLOT_BINGO_LIST {
        NONE = -1,
        LOSE = 0,
        CHERRY2 = 1,
        CHERRY3 = 2,
        POT_HIT = 3,
        SWORD_HIT = 4,
        RING_HIT = 5,
        CROWN_HIT = 6,
        BAR_HIT = 7,
        SEVEN_HIT = 8,
    };

    SLOT_BINGO_LIST lotResult_;                 // 0x00
    int m_slot_tbl_type;                        // 0x04
    int unk_08;                                 // 0x08
    Casino_SlotReel m_reel_l;                   // 0x0C
    Casino_SlotReel m_reel_c;                   // 0x44
    Casino_SlotReel m_reel_r;                   // 0x7C

    static int bingoBonusTable_[8];
    static int slotBingoTable_[5][3];
    static char Reel_Tbl_L[5][32];
    static char Reel_Tbl_C[5][32];
    static char Reel_Tbl_R[5][32];

    Casino_SlotMachine();
    ~Casino_SlotMachine();
    void setupSlot(int type);
    void resetSlot();
    void setLotResult(SLOT_BINGO_LIST result);
    bool scrollSlot();
    void setStopPosition();
    void setCoordinateImage(int image);
    void setCherryImage();
    bool isCherryBingo(int a_t, int a_u, int pos);
    void setNothingBingo();
    int getResultCoin(int line);
};
