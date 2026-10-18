#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"
#include "main/text/TextAPI.hpp"

struct MenuTextDisplay;
struct MenuWindowFrame;

// Message window layout, one per eMessageWindow type
struct MessageWindowConfig {
    int type_;                                  // 0x00
    int x_;                                     // 0x04
    int y_;                                     // 0x08
    int width_;                                 // 0x0C
    int height_;                                // 0x10
    int margin_;                                // 0x14
    int nameX_;                                 // 0x18
    int nameY_;                                 // 0x1C
    int lineCount_;                             // 0x20
    int extraHeight_;                                 // 0x24
    int intervalCursor_;                        // 0x28
    int lastCursor_;                            // 0x2C
    int font_;                                  // 0x30
    int fontHeight_;                            // 0x34
    int lineSpace_;                             // 0x38
    int scrollSpeed_;                           // 0x3C
    int frameType_;                             // 0x40
    int printSpeed_;                            // 0x44
    int wait_;                                  // 0x48
    int keyDelay_;                                 // 0x4C
    int sound_;                                 // 0x50
};

// Message window drawn on the sub screen (g_MenuDisplays.message_)
struct MessageWindow : MenuDisplay {
    int windowType_;                            // 0x030
    MessageWindowConfig* config_;               // 0x034
    int lineCount_;                             // 0x038
    int lineHeight_;                            // 0x03C
    int scrollSpeed_;                           // 0x040
    int width_;                                 // 0x044
    int height_;                                // 0x048
    MenuWindowFrame* frame_;                 // 0x04C
    MenuTextDisplay* namePlate_;             // 0x050
    MenuTextDisplay* lines_[4];              // 0x054
    MenuTextDisplay* freeLines_[4];          // 0x064
    char name_[0x104];                          // 0x074
    char text_[0x800];                          // 0x178
    int unk_978;                                // 0x978
    Utf8Iterator iterator_;                     // 0x97C
    int state_;                                 // 0x9A0
    MenuTextDisplay* waitLine_;              // 0x9A4
    int intervalCursor_;                        // 0x9A8
    int lastCursor_;                            // 0x9AC
    int cursor_;                                // 0x9B0
    int scroll_;                                // 0x9B4
    int keyWait_;                               // 0x9B8
    int keySound_;                              // 0x9BC
    int keyTimer_;                                // 0x9C0
    int wait_;                                  // 0x9C4
    int keyDelay_;                                // 0x9C8
    int waitProgFlag_;                                // 0x9CC
    int shake_;                                 // 0x9D0
    int shakeCount_;                            // 0x9D4
    int shakeX_;                                // 0x9D8
    int shakeY_;                                // 0x9DC
    int sound_;                                 // 0x9E0
    int soundTimer_;                            // 0x9E4
    int soundSpeed_;                            // 0x9E8
    int nameIndex_;                             // 0x9EC
    int nameCount_;                             // 0x9F0
    char names_[32][0x80];                      // 0x9F4

    MessageWindow();
    virtual void setup(int id);
    virtual void update(OamBuffer* main, OamBuffer* sub);
    virtual void execute(OamBuffer* main, OamBuffer* sub);
    virtual void draw(OamBuffer* main, OamBuffer* sub);
    void setName(const char* name);
    void setText(const char* text);
    void appendText(const char* text);
    void setWindowType(int type);
    void startWindow();
    void updateNameFrame();
    void executeState();
    void stateIdle();
    void stateNextLine();
    void statePrint();
    void stateScroll();
    void stateWaitKey();
    void stateWaitProg();
    void stateFinish();
    void initLines();
    MenuTextDisplay* allocLine();
    MenuTextDisplay* getFirstLine();
    MenuTextDisplay* getLastLine();
    void releaseFirstLine();
    int isEndChar(int c);
    int isNewlineChar(int c);
    int isControlCode(int c);
    void skipNewlines(TextIterator* it);
    void openWindow(int type, const char* name, const char* text);
    void addText(const char* text);
    void addWaitKey();
    void addWaitProg();
    void addControlCode(int code);
    void clearNames();
    void addName(const char* name);
    void nextName();
    const char* getCurrentName();
    void restart();
    int isWaitTrigger();
    int isFinish();
    int isEnd();
    bool isWaitProg();
    void clearWaitTriggerSE();
    void clearWaitTriggerNoSE();
    void setWaitProg(int flag);

    TextIterator* getIterator() { return &iterator_; }
};

extern int g_msgWindowShakeOffsetX[8];             // shake offsets x
extern int g_msgWindowShakeOffsetY[8];             // shake offsets y
