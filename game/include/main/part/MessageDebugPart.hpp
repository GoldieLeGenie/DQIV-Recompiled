#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"
#include "main/data/DataObject.hpp"

// Message viewer + screen snapshot debug part 
struct MessageDebugPart : GlobalGamePart {
    int messageNo_;                             // 0x04
    int messageType_;                           // 0x08
    int language_;                              // 0x0C
    int step_;                                  // 0x10
    int back_;                                  // 0x14
    int isChanged_;                             // 0x18
    int isDisp_;                                // 0x1C
    int unk_20;                                 // 0x20
    int snapState_;                             // 0x24
    DataObject snap_;                           // 0x28

    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    int unkfunc_0200a644(int key);              // keys held (0: always true)
};

extern MessageDebugPart g_MessageDebugPart;
