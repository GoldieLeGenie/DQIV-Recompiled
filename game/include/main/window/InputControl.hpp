#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

namespace window {
    enum {
        PHASE_NORMAL = 1,
        PHASE_SHOPMENU = 2,
        PHASE_MENU = 4,
        PHASE_MESSAGE = 8,
        PHASE_MAP = 16,
        PHASE_SHOPLIST = 32,
        PHASE_EVENT = 64,
        PHASE_GLOBALMAP = 128,
        PHASE_PARTY_TALK = 256,
    };

    struct InputControl {
        virtual void execute() = 0;
        virtual int getPhase() = 0;
        virtual void setup() = 0;

        int state_;                             // 0x04

        static int prev_;
        static int next_;
        static dss::BitFlag<unsigned int>* permit_;
        static dss::BitFlag<unsigned char>* icon_;

        InputControl();
        ~InputControl();
        void initialize(dss::BitFlag<unsigned int>* permit, dss::BitFlag<unsigned char>* icon);
        void playerLock(bool flag);
        bool isPlayerLock();
        void setupIcon();
        void setNextPhase(int phase);
        bool goNext(int phase);
        bool isNext();
    };
}

