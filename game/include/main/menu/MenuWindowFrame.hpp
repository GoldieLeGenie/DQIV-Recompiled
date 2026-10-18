#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"

// Window frame drawn as sprites on the sub screen, with its name tab, blinking key cursor and arrow
// (g_MenuDisplays.windowFrame1_, windowFrame2_)
struct UnkMenuWindowFrame : UnkMenuDisplay {
    int cursor_;                                // 0x30  key cursor shown
    int arrow_;                                 // 0x34  arrow shown
    int unk_38;                                 // 0x38
    int arrowX_;                                // 0x3C
    int arrowY_;                                // 0x40
    int nameWidth_;                             // 0x44  name tab width, <= 16: no tab
    int blink_;                                 // 0x48
    int frameType_;                             // 0x4C  1: translucent

    UnkMenuWindowFrame();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void setNameWidth(int width);
    void setCursor(int cursor);
    void setArrow(int x, int y, int arrow);
    void setFrameType(int type);
    void drawBorder(UnkOamBuffer* oam, int x, int y, int w, int h);
    void drawBorderWithTab(UnkOamBuffer* oam, int x, int y, int w, int h, int tabW, int tabH);
    void drawTopEdge(UnkOamBuffer* oam, int x, int y, int length);    // top edge
    void drawBottomEdge(UnkOamBuffer* oam, int x, int y, int length);    // bottom edge
    void drawLeftEdge(UnkOamBuffer* oam, int x, int y, int length);    // left edge
    void drawRightEdge(UnkOamBuffer* oam, int x, int y, int length);    // right edge
    void drawSprite(UnkOamBuffer* oam, int x, int y, int chr, int size);
};
