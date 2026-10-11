#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp" 
#include "main/dss/DssUtils.hpp"
#include "main/profile/Profile.hpp"

namespace status{
    struct HaveEquipment {
        int playerIndex_;
        unsigned char weapon_;
        unsigned char armor_;
        unsigned char shield_;
        unsigned char helmet_;
        unsigned char accessory_;
        dss::BitFlaguint medicine_;
        unsigned char strength_;
        unsigned char agility_;
        unsigned char protection_;
        unsigned char wisdom_;
        unsigned char luck_;
        unsigned short hp_;
        unsigned short mp_;
        unsigned short attack_;
        unsigned short defence_;
        HaveEquipment();
        ~HaveEquipment();
        void setup(unsigned int index);
        void clear();
        void setEquipment2(ItemType type, unsigned int itemIndex);
        void setEquipment(unsigned int itemIndex);
        void resetEquipment(ItemType type);
        void removeItem(unsigned int ItemIndex);
        unsigned char getEquipment(ItemType type);
        bool isEquipment(int itemIndex);
        void recalculateStats();
        void calcEffect(int itemIndex);
        static int getAbsoluteValue(int ItemIndex);
        virtual void beSpell(ItemType type, int itemIndex);
        void breakSpell(ItemType type);
        int isSpell(ItemType type);
        int hasSpellEffect();
        void setSaveDataForPlayer(profile::PROFILE_PLAYER* data);
        void setLoadDataForPlayer(profile::PROFILE_PLAYER* data);
    };
}