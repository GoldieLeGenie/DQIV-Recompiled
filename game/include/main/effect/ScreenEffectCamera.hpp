#pragma once
#include <globaldefs.h>
#include "main/dss/Camera.hpp"

// camera of the screen transition effects (ScreenEffect)
struct ScreenEffectCamera {
    dss::Camera camera_;                        // 0x00

    ScreenEffectCamera();
    ~ScreenEffectCamera();
    static ScreenEffectCamera* getSingleton();
    void initialize();
    void terminate();
    void draw();
};
