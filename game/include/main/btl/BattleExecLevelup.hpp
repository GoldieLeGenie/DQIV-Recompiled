#pragma once
#include <globaldefs.h>
#include "main/task/ExecTask.hpp"

void setMessage(int mes0, int mes1, int mes2, int mes3);

struct BattleExecVictory10 : ExecTask {
    int index_;                         // 0x08
    int level_;                         // 0x0C

    virtual void setup();
};

struct BattleExecVictory13 : ExecTask {
    int index_;                         // 0x08

    virtual void setup();
};

struct BattleExecVictory12a : ExecTask {
    int index_;                         // 0x08

    virtual void setup();
};

struct BattleExecVictory16 : ExecTask {
    int counter_;                       // 0x08

    virtual void setup();
    virtual bool isEnd();
};

struct BattleExecVictory15 : ExecTask {
    int playerIndex_;                   // 0x08
    int actionIndex_[5];                // 0x0C
    int index_;                         // 0x20

    virtual void setup();
    virtual void exec();
    void setActionIndex(int index, int action) { actionIndex_[index] = action; }
};

struct BattleExecVictory11 : ExecTask {
    int index_;                         // 0x08

    virtual void setup();
};

struct BattleExecVictory12 : ExecTask {
    int index_;                         // 0x08

    virtual void setup();
};

struct BattleExecLevelup : ExecTaskManager {
    BattleExecVictory10 battleExecVictory10;     // 0x4C
    BattleExecVictory11 battleExecVictory11;     // 0x5C
    BattleExecVictory12 battleExecVictory12;     // 0x68
    BattleExecVictory12a battleExecVictory12a;   // 0x74
    BattleExecVictory13 battleExecVictory13;     // 0x80
    BattleExecVictory15 battleExecVictory15;     // 0x8C
    BattleExecVictory16 battleExecVictory16;     // 0xB0

    ~BattleExecLevelup();
    virtual void initialize();
    void terminate();
};

extern BattleExecLevelup g_BattleExecLevelup;
