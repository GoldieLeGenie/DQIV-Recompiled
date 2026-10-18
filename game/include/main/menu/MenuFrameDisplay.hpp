#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// Queued frame piece (one sprite)
struct MenuFrameRequest {
    int chr_;                                   // 0x00
    int shape_;                                 // 0x04
    int x_;                                     // 0x08
    int y_;                                     // 0x0C  0-191: main screen, 192-383: sub screen
    int priority_;                              // 0x10
    int palette_;                               // 0x14
    int effect_;                                // 0x18
    int mode_;                                  // 0x1C

    void draw(OamBuffer* main, OamBuffer* sub);
};

// Frames drawn as sprites, queued each frame (g_MenuDisplays.frames_)
struct MenuFrameDisplay : MenuDisplay {
    MenuFrameRequest requests_[128];         // 0x0030
    int requestCount_;                          // 0x1030

    MenuFrameDisplay();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
    void addFrame(int x, int y, int w, int h, int priority, int palette);   // translucent
    void addRoundedFrame(int x, int y, int w, int h, int priority, int palette);   // rounded corners, translucent
    void addOpaqueFrame(int x, int y, int w, int h, int priority, int palette);   // opaque
    void addFrameSprites(int x, int y, int w, int h, int priority, int palette, int corner, int xlu);
    void addFrameCells(int x, int y, int w, int h, int priority, int palette, int cellW, int cellH, int shape,
                          int corner, int xlu);
};
