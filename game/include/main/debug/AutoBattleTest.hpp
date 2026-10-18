#pragma once
#include <globaldefs.h>

// automatic battle test: plays the encounters of the tiles firstTile_..lastTile_, playCount_ times
struct UnkBattleTest {
    int unk_00;                                 // 0x00
    int firstTile_;                             // 0x04
    int lastTile_;                              // 0x08
    int playCount_;                             // 0x0C
    int command_;                               // 0x10 battle command of the players 1 and 2
    int command2_;                              // 0x14 battle command of the other players
    int noDamage_;                              // 0x18
    int hpRate_;                                // 0x1C % of the max HP at the battle start
    int noDamageTurn_;                          // 0x20 monsters take no damage until this turn
    int endTurn_;                               // 0x24 the battle is stopped after this turn
    int state_;                                 // 0x28
    int counter_;                               // 0x2C frames in the state
    int tile_;                                  // 0x30
    int play_;                                  // 0x34
    int unk_38;                                 // 0x38
    int unk_3c;                                 // 0x3C
    int unk_40;                                 // 0x40
    int act_;                                   // 0x44

    UnkBattleTest();
    void unkfunc_02089b34();                    // update
};

extern UnkBattleTest data_02121094;
