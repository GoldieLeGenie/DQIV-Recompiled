#pragma once
#include "main/window/InputControl.hpp"

namespace window {
    struct ShopMenuControl : InputControl {
        enum {
            OPEN_SETUP = 0,
            OPEN_SETUP1 = 1,
            OPEN_SETUP2 = 2,
            OPEN_WAIT = 3,
            OPEN = 4,
            CLOSE_WAIT = 5,
            RELEASE_WAIT = 6,
        };

        int menuType_;                          // 0x08

        virtual void execute();
        virtual int getPhase();
        virtual void setup();
    };
}
