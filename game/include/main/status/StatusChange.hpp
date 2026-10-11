#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/StatusChangeOne.hpp"


namespace status{
    struct StatusChange {
        StatusChangeOne status_[35];
        int actionIndex_[35];

        enum Status
        {
            StatusNone = 0x0,
            StatusDamage = 0x1,
            StatusRecove = 0x2,
            StatusInstantDeath = 0x3,
            StatusRebirth = 0x4,
            StatusAddMp = 0x5,
            StatusSubMp = 0x6,
            StatusRelease = 0x7,
            StatusAstoron = 0x8,
            StatusSpazz = 0x9,
            StatusSleep = 0xA,
            StatusManusa = 0xB,
            StatusBaikiruto = 0xC,
            StatusFubaha = 0xD,
            StatusMahokanta = 0xE,
            StatusMahosute = 0xF,
            StatusMosyasu = 0x10,
            StatusPowerSave = 0x11,
            StatusMahoton = 0x12,
            StatusDragoram = 0x13,
            StatusConfusion = 0x14,
            StatusPath1 = 0x15,
            StatusPoison = 0x16,
            StatusLight = 0x17,
            StatusDefenceChange = 0x18,
            StatusAgilityChange = 0x19,
            StatusFeather = 0x1A,
            StatusDefence = 0x1B,
            StatusAlive = 0x1C,
            StatusDead = 0x1D,
            StatusTimeStop = 0x1E,
            StatusFizzleZone = 0x1F,
            StatusFog = 0x20,
            StatusSpell = 0x21,
            StatusPoison2 = 0x22,
            StatusMax = 0x23,
        };
        static StatusChangeOne statusFizzleZone_;
        static int actionIndexFizzleZone_;
        
        StatusChange();
        ~StatusChange();
        bool setup(int actionIndex, bool flag);
        void setup2(Status status, int flag);
        void clear();
        int canPerformAction(int damageType);
        void checkValidAfter(int damageType);
        int isEnable(Status status);
        void setTurn(Status status, char turn);
        void execStartOfTurn();
        void execEndOfTurn();
        void execEndOfRound();
        void execEndOfBattle();
        int isRelease();
        int getExecMessage1(Status status);
        void release(Status status);
        int getActionIndex(Status status);
        int getExecMessage(Status status);
        int getResultMessage1(Status status);
        int getResultMessage2(Status status, CharacterType type);
        int getResultMessage3(Status status, CharacterType type);
        int getAgainEnableMessage(Status status);
        int getAgainDisableMessage(Status status);
        int getReleaseMessage();
        static void setupFizzleZone();
        void print();
    };
}