#pragma once
#include "main/window/InputControl.hpp"
#include "main/window/ImageMap.hpp"

namespace window {
    struct MapControl : InputControl {
        enum {
            TOWNMAP_WAIT_OPEN = 0,
            TOWNMAP_VIEWING = 1,
            TOWMMAP_WAIT_CLOSE = 2,
            TOWNMAP_MESSAGE = 3,
        };

        ImageMap* imageMap_;                    // 0x08

        virtual void execute();
        virtual int getPhase();
        virtual void setup();
        void initialize();
        void registImageMap(ImageMap* imageMap);
        void openMap();
        void closeMap();
        void showMapMessage();
        void closeMapMessage();
    };
}
