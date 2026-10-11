#pragma once
#include "main/dss/UnkSprite2D.hpp"

namespace cmn {
    struct CommonChapterTitle {
        UnkMenuSprite sprite_;      // 0x00

        CommonChapterTitle();
        ~CommonChapterTitle();
        static CommonChapterTitle* getSingleton();
        void setup(int chapter, int flag);
        void cleanup();
        void draw();
    };
}
