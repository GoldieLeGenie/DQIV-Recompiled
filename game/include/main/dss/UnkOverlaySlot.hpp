#pragma once
#include <globaldefs.h>

// Overlay loaded on demand (one at a time)
struct UnkOverlaySlot {
    int loaded_;                                // 0x00
    int id_;                                    // 0x04

    UnkOverlaySlot();
    ~UnkOverlaySlot();
    void unkfunc_0208753c(int id);              // load (unloads the previous one)
    void unkfunc_02087564();                    // unload
};

void unkfunc_02087590(int id);                  // FS_LoadOverlay
void unkfunc_020875a4(int id);                  // FS_UnloadOverlay
