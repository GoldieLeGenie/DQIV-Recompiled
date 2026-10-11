#pragma once
#include <globaldefs.h>
#include "main/status/HaveItemSack.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"

namespace status{
    struct BasePartyStatus {
        int basha_;
        int bashaEnter_;
        int bashaEnable_;
        int balloon_;
        int ship_;
        int fukuro_;
        HaveItemSack haveItemSack_;
        unsigned int gold_;
        unsigned int bankMoney_;
        unsigned int casinoCoin_;
        unsigned int playerMedalCoin_;
        unsigned int medalCoin_;
        int battleMonsterCount_;
        int battleExp_;
        int battleGold_;
        BasePartyStatus();
        int isBasha() { return basha_; }
        ~BasePartyStatus() {}
        bool isCarriageEnter();
        void setGold(int gold);
        static unsigned int getClampValue(unsigned int value, unsigned int min, unsigned int max);
        static unsigned int getMin(unsigned int a, unsigned int b);
        static unsigned int getMax(unsigned int a, unsigned int b);
        void addGold(int gold);
        void setBankMoney(unsigned int money);
        void addBankMoney(unsigned int money);
        void setCasinoCoin(unsigned int coins);
        void addCasinoCoin(int coin);
        void setMedalCoin(unsigned int coin);
        void addMedalCoin(unsigned int coin);
        void setPlayerMedalCoin(unsigned int coin);
        void addPlayerMedalCoin(unsigned int coin);
        int isFirstMedalCoin();
        virtual void setSaveData(profile::PROFILE_PARTY* data);
        virtual void setLoadData(profile::PROFILE_PARTY *data);
        int getBattleExp();
        int getBattleGold();
    };
}