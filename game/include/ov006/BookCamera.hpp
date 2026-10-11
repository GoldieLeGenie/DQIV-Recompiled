#pragma once
#include <globaldefs.h>
#include "main/dss/Camera.hpp"

// camera of the monster book 
struct BookCamera {
    dss::DualCamera camera_;                    // 0x00

    BookCamera();
    ~BookCamera();
    static BookCamera* getSingleton();
    void initialize();
    void terminate();
    void draw();
    dss::Camera& getCamera() { return getSingleton()->camera_.unk_004; }
};
