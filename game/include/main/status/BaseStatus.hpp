#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"

namespace status{
    struct BaseStatus {
        int exp_;
        unsigned short strength_;
        unsigned short protection_;
        unsigned char agility_;
        unsigned char wisdom_;
        unsigned short hp_;
        unsigned short hpMax_;
        unsigned short mp_;
        unsigned short mpMax_;
        unsigned char luck_;
        BaseStatus();
        ~BaseStatus();
        static unsigned int getClampValue(unsigned int value, unsigned int min, unsigned int max);
        static unsigned int getMin(unsigned int a, unsigned int b);
        static unsigned int getMax(unsigned int a, unsigned int b);
        void addStrength(char str); 
        void addAgility(char agi); 
        void addProtection(char prot); 
        void addWisdom(char wisd);
        static unsigned short getClampValue2(unsigned short value, unsigned short min, unsigned short max);
        static unsigned short getMin2(unsigned short a, unsigned short b);
        static unsigned short getMax2(unsigned short a, unsigned short b);
        void setHpMax(unsigned short hp); 
        void addHpMax(char hp); 
        void setMpMax(unsigned short mp); 
        void addMpMax(char mp);
        
    };
}

