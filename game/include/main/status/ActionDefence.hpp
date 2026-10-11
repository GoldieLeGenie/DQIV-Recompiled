#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"

namespace status{
    
    struct ActionDefence {
        CharacterType characterType_;
        unsigned char io_;
        unsigned char gira_;
        unsigned char mera_;
        unsigned char dein_;
        unsigned char bagi_;
        unsigned char hyado_;
        unsigned char manusa_;
        unsigned char rariho_;
        unsigned char zaki_;
        unsigned char nifram_;
        unsigned char mahotora_;
        unsigned char mahotone_;
        unsigned char medapani_;
        unsigned char rukani_;
        unsigned char megante_;
        unsigned char dragon_;
        unsigned char metal_;
        unsigned char zombi_;
        unsigned char air_;
        unsigned char slime_;
        unsigned char poison_;
        unsigned char rest_;
        unsigned char antidance_;
        unsigned char fire_;
        unsigned char blizzard_;
        unsigned char clap_;
        unsigned char spazz_;
        unsigned char army_;
        ActionDefence();
        ~ActionDefence();
        void setup(unsigned short index, int flag);
        void setupPlayer();
        void setupMonster(int index);
        int getEffect(ActionDefenceType type,unsigned char value);
        int exec(ActionDefenceKind kind, ActionDefenceType type);
        int getEffectAI(ActionDefenceType type,unsigned char value);
        int execAI(ActionDefenceKind kind, ActionDefenceType type);
    };
    
}

