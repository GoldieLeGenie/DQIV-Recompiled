#pragma once
#include <globaldefs.h>
#include "nitro/fx.hpp"

namespace dss {
    // display mode of the two screens (mobile dss_displayplugin.h)
    struct DisplayPlugin {
#ifdef DQIV_NATIVE
        // Only concrete display plugins implement these interfaces in the game.
        virtual void initialize() = 0;
        virtual void finalize() = 0;
        virtual void update(int frame) = 0;
#else
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
#endif

        int displayTextCounter_;                // 0x04
        int type_;                              // 0x08

        void drawCaptureObjs(int chr);         // OAM of the captured screen
        void setupDisplayText(int textType, int screen1, int screen2, int bg0, int bg1, int unk_10, int palette);   // setup the text
        void updateDisplayText();
        void setDisplayText(int counter);
        int getType() { return type_; }
    };

    struct DisplayPlugin_TEXTONLY : DisplayPlugin {
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
    };

    struct DisplayPlugin_STOP : DisplayPlugin {
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
    };

    struct DisplayPlugin_SINGLE3D : DisplayPlugin {
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
    };

    struct DisplayPlugin_DOUBLE3D : DisplayPlugin {
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
        void ReqBlurMode(int blur);
        void SetBlur(int eva, int evb);

        int blur_;                              // 0x0C
        int eva_;                               // 0x10
        int evb_;                               // 0x14
    };

    struct DisplayPlugin_CAPTURE : DisplayPlugin {
        virtual void initialize();
        virtual void finalize();
        virtual void update(int frame);
    };

    extern DisplayPlugin_STOP g_DISPLAYPLUGIN_STOP;
    extern DisplayPlugin_TEXTONLY g_DISPLAYPLUGIN_TEXTONLY;
    extern DisplayPlugin_DOUBLE3D g_DISPLAYPLUGIN_DOUBLE3D;
    extern DisplayPlugin_SINGLE3D g_DISPLAYPLUGIN_SINGLE3D;
    extern DisplayPlugin_CAPTURE g_DISPLAYPLUGIN_CAPTURE;
}

extern "C" {
    void func_02063f50(int bg, int unk);        // BG VRAM bank
    void func_02064f40(volatile void* reg, const MtxFx22* mtx, int centerX, int centerY, int x, int y);   // BG affine
}
