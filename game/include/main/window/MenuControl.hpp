#pragma once
#include "main/window/InputControl.hpp"

namespace window {
    struct MenuControl : InputControl {
        enum {
            OPEN_SETUP = 0,
            OPEN_SETUP1 = 1,
            OPEN_SETUP2 = 2,
            OPEN_WAIT = 3,
            OPEN = 4,
            CLOSE_WAIT = 5,
            RELEASE_WAIT = 6,
        };
        enum {
            OPEN_MENU_ROOT = 0,
            OPEN_MENU_MAGIC = 1,
            OPEN_MENU_ITEM = 2,
            OPEN_MENU_ITEM_CHARA = 3,
        };

        int regist_;                            // 0x08

        static int menu_;

        MenuControl();
        ~MenuControl();
        virtual void execute();
        virtual int getPhase();
        virtual void setup();
        void openMenu();
    };
}
