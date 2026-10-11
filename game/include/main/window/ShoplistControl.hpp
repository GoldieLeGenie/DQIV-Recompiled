#pragma once
#include "main/window/InputControl.hpp"
#include "main/window/ImageMap.hpp"

namespace window {
    struct ShoplistControl : InputControl {
        enum {
            TOWNMAP_WAIT_OPEN = 0,
            TOWNMAP_VIEWING = 1,
            TOWMMAP_WAIT_CLOSE = 2,
            TOWMMAP_CLOSE = 3,
            TOWNMAP_MESSAGE = 4,
            TOWNMAP_WAIT_CLOSE_TO_MAP = 5,
            RELEASE_WAIT = 6,
        };

        ImageMap* imageMap_;                    // 0x08

        virtual void execute();
        virtual int getPhase();
        virtual void setup();
        void openList();
        void closeList();
        int unkfunc_0202a2d8();
        void closeListMessage();
        void registShopList(ImageMap* shoplist);
    };
}
