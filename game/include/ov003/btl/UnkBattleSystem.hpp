#pragma once
#include <globaldefs.h>


struct UnkBattleSystem {
    int unk_00;                                 // 0x00

    UnkBattleSystem();
    static UnkBattleSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
};
