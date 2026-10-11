#pragma once
#include "globaldefs.h"
#include "main/dss/Render.hpp"
#include "main/dss/DssUtils.hpp"

struct FieldSystem {
    Render render_;                                                                 // 0x000
    int cameraLock_;                                                                // 0x608
    int encountStart_;                                                              // 0x60C
    int unk_610;                                                                    // 0x610
    int exitSound_;                                                                 // 0x614
    int scriptLock_;                                                                // 0x618

    FieldSystem();
    static FieldSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void execEncount();
    void setLookAtPos(dss::Fix32Vector3 pos);
};
