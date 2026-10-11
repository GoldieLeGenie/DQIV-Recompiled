#pragma once
#include "main/window/InputControl.hpp"

namespace window {
    struct MessageControl : InputControl {
        enum {
            MESSAGE = 0,
            CLOSE_WAIT = 1,
        };

        int back_;                              // 0x08

        virtual void execute();
        virtual int getPhase();
        virtual void setup();
    };
}
