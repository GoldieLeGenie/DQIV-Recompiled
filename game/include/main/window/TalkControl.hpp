#pragma once
#include "main/window/InputControl.hpp"

namespace window {
    struct TalkControl : InputControl {
        TalkControl();
        ~TalkControl();
        virtual void execute();
        virtual int getPhase();
        virtual void setup();
        void openTalk();
    };
}
