#pragma once
#include <globaldefs.h>
#include "main/dss/Camera.hpp"

// camera of the screen transition effects (UnkScreenEffect)
struct UnkEffectCamera {
    dss::Camera camera_;                        // 0x00

    UnkEffectCamera();
    ~UnkEffectCamera();
    static UnkEffectCamera* getSingleton();
    void initialize();
    void terminate();
    void draw();
};
