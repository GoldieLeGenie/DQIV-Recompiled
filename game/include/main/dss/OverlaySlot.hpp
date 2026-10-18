#pragma once
#include <globaldefs.h>

// Overlay loaded on demand (one at a time)
struct OverlaySlot {
    int loaded_;                                // 0x00
    int id_;                                    // 0x04

    OverlaySlot();
    ~OverlaySlot();
    void unkfunc_0208753c(int id);              // load (unloads the previous one)
    void unkfunc_02087564();                    // unload
};

void loadOverlay(int id);                  // FS_LoadOverlay
void unloadOverlay(int id);                  // FS_UnloadOverlay
