#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"

namespace status {
    struct OptionStatus {
        char sackSort_;
        char bgmVolume_;
        char seVolume_;
        char stereo_;
        char speaker_;
        char battleSpeed_;
        char button_;
        char unk;
        OptionStatus();
        ~OptionStatus();
        void initialize();
        void setSackSort(char id);
        char getSackSort();
        void setBgmVolume(char id);
        char getBgmVolume();
        void setSeVolume(char id);
        char getSeVolume();
        void setBattleSpeed(char id);
        char getBattleSpeed();
        char getButton();
    };
    
}

extern status::OptionStatus g_Option;
