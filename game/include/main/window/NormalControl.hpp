#pragma once
#include "main/window/InputControl.hpp"

namespace window {
    struct NormalControl : InputControl {
        enum {
            CONTROL_NORMAL = 0,
            CONTROL_NPC_MESSAGE = 1,
            CONTROL_PARTY_TALK = 2,
        };

        int state_;                             // 0x08

        virtual void execute();
        virtual int getPhase();
        virtual void setup();
        void executePlayer();
        void checkCamera();
        void openMap();
        void openShopList();
        void openMenu();
        bool isNPCParty();
        bool isDeadParty();
        bool NPCPulling();
    };
}
