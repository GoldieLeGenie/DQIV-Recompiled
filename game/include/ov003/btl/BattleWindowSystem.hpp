#pragma once
#include <globaldefs.h>


struct BattleWindowSystem {
    int unk_00;                                 // 0x00

    BattleWindowSystem();
    static BattleWindowSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
};
