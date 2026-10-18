#pragma once
#include "main/dss/Sprite2D.hpp"

namespace cmn {
    struct CommonChapterTitle {
        MenuSprite sprite_;      // 0x00

        CommonChapterTitle();
        ~CommonChapterTitle();
        static CommonChapterTitle* getSingleton();
        void setup(int chapter, int flag);
        void cleanup();
        void draw();
    };
}
