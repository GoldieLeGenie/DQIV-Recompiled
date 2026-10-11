#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/task/ExecTask.hpp"

namespace btl {
    struct BattleExecEncount : ExecTask {
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecStatus : ExecTask {
        int monsterCount_;
        int index_;
        virtual void setup();
        virtual bool isEnd();
        void setupLast();
        int isNext();
        void setupSleep();
        void setupConfusion();
        void setupSpazz();
    };

    struct BattleExecFirstAttack : ExecTask {
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory00 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory01 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory02 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory03 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory20 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory30 : ExecTask {
        int monsterIndex_;
        virtual void setup();
    };

    struct BattleExecVictory31 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory31a : ExecTask {
        int itemIndex_;
        virtual void setup();
    };

    struct BattleExecVictory32 : ExecTask {
        int itemIndex_;
        virtual void setup();
    };

    struct BattleExecVictory33 : ExecTask {
        int playerIndex_;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory34 : ExecTask {
        int playerIndex_;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory35 : ExecTask {
        int playerIndex_;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory36 : ExecTask {
        int playerIndex_;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory37 : ExecTask {
        virtual void setup();
        virtual void cleanup();
    };

    struct BattleExecVictory38 : ExecTask {
        int counter_;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecVictory39 : ExecTask {
        virtual void setup();
    };

    struct BattleExecVictory40 : ExecTask {
        int counter_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
    };

    struct BattleExecEvent00 : ExecTask {
        int counter_;
        static int getRealVelorinman();
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
        int setupMonster(int index);
    };

    struct BattleExecEvent00b : ExecTask {
        int counter_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
        void move();
    };

    struct BattleExecEvent01 : ExecTask {
        int counter_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
    };

    struct BattleExecEvent02 : ExecTask {
        int counter_;
        int status_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
        void execChange();
    };

    struct BattleExecEvent03 : ExecTask {
        int counter_;
        int enable_;
        virtual void setup();
        virtual void cleanup();
        void endTransform();
        virtual bool isEnd();
    };

    struct BattleExecEvent11 : ExecTask {
        int counter_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
    };

    struct BattleExecEvent12 : ExecTask {
        int counter_;
        int status_;
        virtual void setup();
        virtual void cleanup();
        virtual bool isEnd();
        void execChange();
    };

    struct BattleExecEvent13 : ExecTask {
        int counter_;
        int enable_;
        virtual void setup();
        virtual void cleanup();
        void endTransform();
        virtual bool isEnd();
    };

    struct BattleExecEvent14 : ExecTask {
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecEvent15 : ExecTask {
        int counter;
        virtual void setup();
        virtual bool isEnd();
    };

    struct BattleExecMonsterEscape : ExecTaskManager {
        BattleExecVictory02 battleExecVictory02;        // 0x4C
        virtual void initialize();
        void terminate();
    };

    struct BattleExecMonsterDisappear : ExecTaskManager {
        BattleExecVictory03 battleExecVictory03;        // 0x4C
        virtual void initialize();
        void terminate();
    };

    struct BattleExecDefeatMonster : ExecTaskManager {
        BattleExecVictory00 battleExecVictory00;        // 0x4C
        BattleExecVictory01 battleExecVictory01;        // 0x54
        virtual void initialize();
        void terminate();
    };

    struct BattleExecGold : ExecTaskManager {
        BattleExecVictory20 battleExecVictory20;        // 0x4C
        virtual void initialize();
        void terminate();
    };

    struct BattleExecItem : ExecTaskManager {
        BattleExecVictory30 battleExecVictory30;        // 0x4C
        BattleExecVictory31 battleExecVictory31;        // 0x58
        BattleExecVictory31a battleExecVictory31a;      // 0x60
        BattleExecVictory32 battleExecVictory32;        // 0x6C
        virtual void initialize();
        void terminate();
    };

    struct BattleExecReorder : ExecTaskManager {
        BattleExecVictory33 battleExecVictory33;        // 0x4C
        BattleExecVictory34 battleExecVictory34;        // 0x58
        BattleExecVictory35 battleExecVictory35;        // 0x64
        BattleExecVictory36 battleExecVictory36;        // 0x70
        virtual void initialize();
        void terminate();
    };

    struct BattleExecDemolition : ExecTaskManager {
        BattleExecVictory37 battleExecVictory37;        // 0x4C
        BattleExecVictory38 battleExecVictory38;        // 0x54
        BattleExecVictory39 battleExecVictory39;        // 0x60
        BattleExecVictory40 battleExecVictory40;        // 0x68
        virtual void initialize();
        void terminate();
    };

    struct BattleExecVelorinman : ExecTaskManager {
        BattleExecEvent00 battleExecEvent00;            // 0x4C
        BattleExecEvent00b battleExecEvent00b;          // 0x58
        virtual void initialize();
    };

    struct BattleExecDeathPissaro : ExecTaskManager {
        BattleExecEvent01 battleExecEvent01;            // 0x4C
        BattleExecEvent02 battleExecEvent02;            // 0x58
        BattleExecEvent03 battleExecEvent03;            // 0x68
        int ctrlDrawId_[3];                             // 0x78
        virtual void initialize();
        void terminate();
    };

    struct BattleExecEvilPriest : ExecTaskManager {
        BattleExecEvent11 battleExecEvent11;            // 0x4C
        BattleExecEvent12 battleExecEvent12;            // 0x58
        BattleExecEvent13 battleExecEvent13;            // 0x68
        virtual void initialize();
        void terminate();
    };

    struct BattleExecDeathPissaroMahokanta : ExecTaskManager {
        BattleExecEvent14 battleExecEvent14;            // 0x4C
        BattleExecEvent15 battleExecEvent15;            // 0x54
        int flag_;                                      // 0x60
        virtual void initialize();
        void terminate();
    };

    struct BattleExecEscape : ExecTask {
        virtual void setup();
        virtual bool isEnd();
    };

    extern BattleExecEncount g_BattleExecEncount;
    extern BattleExecStatus g_BattleExecStatus;
    extern BattleExecFirstAttack g_BattleExecFirstAttack;
    extern BattleExecMonsterEscape g_BattleExecMonsterEscape;
    extern BattleExecMonsterDisappear g_BattleExecMonsterDisappear;
    extern BattleExecDefeatMonster g_BattleExecDefeatMonster;
    extern BattleExecGold g_BattleExecGold;
    extern BattleExecItem g_BattleExecItem;
    extern BattleExecReorder g_BattleExecReorder;
    extern BattleExecDemolition g_BattleExecDemolition;
    extern BattleExecVelorinman g_BattleExecVelorinman;
    extern BattleExecDeathPissaro g_BattleExecDeathPissaro;
    extern BattleExecEvilPriest g_BattleExecEvilPriest;
    extern BattleExecDeathPissaroMahokanta g_BattleExecDeathPissaroMahokanta;
    extern BattleExecEscape g_BattleExecEscape;
}

extern const int velorinmanUnknown;
void InitBattleExecEvent00();
