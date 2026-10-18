#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// 48x48 face icon (face48.mpt) or loading animation (loading48.mpt) drawn as 32x32 sprites (g_MenuDisplays.face_)
struct MenuFaceDisplay : MenuDisplay {
    int priority_;                              // 0x30
    int mode_;                                  // 0x34  1: whole icon, 2-10: one part, 11: loading animation
    int animFrame_;                             // 0x38

    MenuFaceDisplay();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
    void drawPart(OamBuffer* oam, int x, int y, int mode);
    void setPriority(int priority);
    void loadFace(int index);           // face icon
    void loadLoadingAnim();                    // loading animation
    void resetAnim();
    void loadIcon(int type, int index);
};
