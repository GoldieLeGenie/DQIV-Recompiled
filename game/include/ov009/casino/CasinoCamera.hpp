#pragma once
#include <globaldefs.h>
#include "main/dss/Camera.hpp"

struct CasinoCamera {
    dss::DualCamera camera_;                    // 0x00

    CasinoCamera();
    ~CasinoCamera();
    static CasinoCamera* getSingleton();
    void initialize();
    void terminate();
    void draw();
};
