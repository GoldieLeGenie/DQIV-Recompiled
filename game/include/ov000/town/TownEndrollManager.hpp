#pragma once
#include "globaldefs.h"
#include "main/data/DataObject.hpp"
#include "main/dss/UnkSprite2D.hpp"

struct TownEndrollManager {
    static const int STAFF_MAX = 3;

    UnkMenuSprite staff_[STAFF_MAX];            // 0x000
    DataObject theEndData_;                     // 0x0F0
    UnkSprite2D theEnd_;                        // 0x100
    void* theEndTexture_;                       // 0x138
    DataObject textureData_;                    // 0x13C
    void* texture_;                             // 0x14C
    int staffCount_;                            // 0x150
    int staffPos_[STAFF_MAX];                   // 0x154
    int staffIndex_[STAFF_MAX];                 // 0x160
    int enable_;                                // 0x16C
    int enableScroll_;                          // 0x170
    int enableTheEnd_;                          // 0x174
    int staffDone_;                             // 0x178
    int frame_;                                 // 0x17C
    int unk_180;                                // 0x180
    int theEndFrame_;                           // 0x184
    int unk_188;                                // 0x188
    int unk_18c;                                // 0x18C
    char unk_190[0x200];                        // 0x190

    TownEndrollManager();
    ~TownEndrollManager();
    static TownEndrollManager* getSingleton();
    void setup();
    void cleanup();
    void animTheEnd();
    void drawStaffRoll();
    void startTheEnd();
    bool isEndTheEnd();
    void draw();
    void execute();
    int isStaffRollEnd();
};
