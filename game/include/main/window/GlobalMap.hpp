#pragma once
#include <globaldefs.h>
#include "main/window/UnkMapBase_020381d0.hpp"
#include "main/dss/DssUtils.hpp"

namespace window {
    struct GlobalMap : UnkMapBase_020381d0 {
        UnkMenuSprite unk_100;                  // 0x100

        GlobalMap();
        ~GlobalMap();
        virtual void setup(Render* render);
        virtual void cleanup();
        virtual void setup();
        virtual void draw();
        void drawVeilMap();
        void veilDraw(int x, int y, int flag);
        virtual void load();
        virtual void clear();
        static dss::Vector2<int> convertMapPos(int x, int y);
        virtual void setAlpha(unsigned char alpha);
        virtual void playerMapPosition();
    };
}
