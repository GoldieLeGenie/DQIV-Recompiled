#pragma once
#include <globaldefs.h>
#include "main/dss/Sprite2D.hpp"
#include "main/dss/Render.hpp"

namespace window {
    // abstract base of the world maps (window::GlobalMap and the field maps of ov001), virtuals named after mobile GlobalMap
    struct WorldMapBase {
        int unk_04;                             // 0x04
        MenuSprite mapSprite_;                   // 0x08
        MenuSprite symbolSprite_;                   // 0x58
        MenuSprite markSprite_;                   // 0xA8
        int blinkCounter_;                             // 0xF8
        Render* render_;                        // 0xFC

        WorldMapBase();
        ~WorldMapBase();
        virtual void setup(Render* render) = 0;
        virtual void setup() = 0;
        virtual void cleanup() = 0;
        virtual void draw() = 0;
        virtual void load() = 0;
        virtual void clear() = 0;
        virtual void playerMapPosition() = 0;
        virtual void setAlpha(unsigned char alpha) = 0;
        void drawSymbols(int world);
    };
}
