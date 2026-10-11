#pragma once
#include <globaldefs.h>
#include "main/status/CharacterStatus.hpp"
#include "GameInfo.hpp"

namespace status{
    struct PlayerStatus : CharacterStatus {
        short index_;
        short walkCount_;
        short walkCountBarrier_;
        short walkCountPoison_;
        short walkCountPoisonCondition_;
        short walkCountZombieMail_;
        short walkCountLuckShoes_;
        short walkCountLuckHat_;
        short walkCountGoddessRing_;
        short walkCountLifeRing_;
        PlayerStatus();
        ~PlayerStatus();
        int getCharaIndex() { return haveStatusInfo_.haveStatus_.charaIndex_; }
        bool isAlive() { return !getHaveStatusInfo().isDeath(); }
        void setup(int index);
        void levelup(int level);
        void setBestCondition();
        bool walkDamage(StageDamage damage);
        bool walkNormal();
        bool setWalkEffectPoison();
        bool setWalkEffectZombiMail();
        void setWalkEffectLuckShoes();
        void setWalkEffectLuckHat();
        void setWalkEffectGoddessRing();
        void setWalkEffectLifeRing();
        void clearWalkDamage();
        void setNearDeath(int damage);
        void execStartOfRound();
        void execOfRound();
        virtual void print();
        virtual void printAction();
        void setSaveDataForPlayer(profile::PROFILE_PLAYER* dat);
        void setLoadDataForPlayer(profile::PROFILE_PLAYER* dat);

    };
}
