#pragma once
#include <globaldefs.h>
#include "main/dss/UnkSprite2D.hpp"
#include "main/dss/Render.hpp"

namespace window {
    // abstract base of the world maps (window::GlobalMap and the field maps of ov001), virtuals named after mobile GlobalMap
    struct UnkMapBase_020381d0 {
        int unk_04;                             // 0x04
        UnkMenuSprite unk_08;                   // 0x08
        UnkMenuSprite unk_58;                   // 0x58
        UnkMenuSprite unk_a8;                   // 0xA8
        int unk_f8;                             // 0xF8
        Render* render_;                        // 0xFC

        UnkMapBase_020381d0();
        ~UnkMapBase_020381d0();
        virtual void setup(Render* render) = 0;
        virtual void setup() = 0;
        virtual void cleanup() = 0;
        virtual void draw() = 0;
        virtual void load() = 0;
        virtual void clear() = 0;
        virtual void playerMapPosition() = 0;
        virtual void setAlpha(unsigned char alpha) = 0;
        void unkfunc_0203822c(int world);
    };
}
