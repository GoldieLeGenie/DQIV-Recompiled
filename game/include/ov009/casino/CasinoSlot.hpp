#pragma once
#include <globaldefs.h>
#include "ov009/casino/CasinoMiniGameBase.hpp"

struct CasinoSlot : CasinoMiniGameBase {
    int m_slot_type;                            // 0x08 (DS-only)
    int unk_0c;                                 // 0x0C
    int unk_10;                                 // 0x10
    int m_reel_id[3];                           // 0x14
    int m_bingo_line[5];                        // 0x20
    int m_bingo_counter[5];                     // 0x34

    static char* stageSlot;

    CasinoSlot();
    ~CasinoSlot() {}
    static CasinoSlot* getSingleton();
    virtual void initialize();
    virtual void terminate();
    virtual void execute();
    virtual void draw();
    void setSlotType(int type);
    void rotReel(int reel, unsigned short position);
    virtual char* getStageName();
    void setLineLamp(int line, bool flag);
    void setLineBingo(int line);
    void setBingoAnim(int anim);
    void stopEventAnim();
};
