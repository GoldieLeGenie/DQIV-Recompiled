#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"
#include "main/dss/BgBuffer.hpp"

// One line of text drawn as sprites (g_MenuDisplays.texts_: message window lines and name plate)
struct MenuTextDisplay : MenuDisplay {
    unsigned char charWidths_[0x100];           // 0x030
    int visibleWidth_;                          // 0x130
    int visibleCount_;                          // 0x134
    int width_;                                 // 0x138
    int count_;                                 // 0x13C
    int xlu_;                                   // 0x140
    BgCharBuffer char_;                        // 0x144
    char charData_[0x800];                      // 0x154
    int frame_;                                 // 0x954  draw the frame
    int frameWidth_;                            // 0x958
    int unk_95c;                                // 0x95C
    int palette_;                               // 0x960
    int priority_;                              // 0x964
    int timer_;                                 // 0x968
#ifdef DQIV_NATIVE
    int letterPalette_ = -1;                    // the letters' palette when not palette_ (name plates' health)
#endif

    MenuTextDisplay();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
    void drawHiddenMask(OamBuffer* oam, int x, int y, int width, int height);
    void setText(int font, const char* text, int x, int y, int clear);
    int setFramedText(int font, const char* text, int priority, int flag);
    void setVisibleCount(int count);
    void setVisibleWidth(int width);
    int getVisibleCount();
    int getCharCount();
    int getTextWidth();
    void clearScrolledLines(int scroll);
    void setTranslucent(int flag);
    void drawFrameSprite32(OamBuffer* oam, int x, int y, int chr);
    void drawFrameSprite16(OamBuffer* oam, int x, int y, int chr);
};

