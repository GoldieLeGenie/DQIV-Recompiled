#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuDisplay.hpp"
#include "main/text/TextAPI.hpp"

struct UnkMenuTextDisplay;
struct UnkMenuWindowFrame;

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
    int unk_24;                                 // 0x24
    int intervalCursor_;                        // 0x28
    int lastCursor_;                            // 0x2C
    int font_;                                  // 0x30
    int fontHeight_;                            // 0x34
    int lineSpace_;                             // 0x38
    int scrollSpeed_;                           // 0x3C
    int frameType_;                             // 0x40
    int printSpeed_;                            // 0x44
    int wait_;                                  // 0x48
    int unk_4c;                                 // 0x4C
    int sound_;                                 // 0x50
};

// Message window drawn on the sub screen (data_020f530c.message_)
struct MessageWindow : UnkMenuDisplay {
    int windowType_;                            // 0x030
    MessageWindowConfig* config_;               // 0x034
    int lineCount_;                             // 0x038
    int lineHeight_;                            // 0x03C
    int scrollSpeed_;                           // 0x040
    int width_;                                 // 0x044
    int height_;                                // 0x048
    UnkMenuWindowFrame* frame_;                 // 0x04C
    UnkMenuTextDisplay* namePlate_;             // 0x050
    UnkMenuTextDisplay* lines_[4];              // 0x054
    UnkMenuTextDisplay* freeLines_[4];          // 0x064
    char name_[0x104];                          // 0x074
    char text_[0x800];                          // 0x178
    int unk_978;                                // 0x978
    Utf8Iterator iterator_;                     // 0x97C
    int state_;                                 // 0x9A0
    UnkMenuTextDisplay* waitLine_;              // 0x9A4
    int intervalCursor_;                        // 0x9A8
    int lastCursor_;                            // 0x9AC
    int cursor_;                                // 0x9B0
    int scroll_;                                // 0x9B4
    int keyWait_;                               // 0x9B8
    int keySound_;                              // 0x9BC
    int unk_9c0;                                // 0x9C0
    int wait_;                                  // 0x9C4
    int unk_9c8;                                // 0x9C8
    int unk_9cc;                                // 0x9CC
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
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void unkfunc_0204d3a4(const char* name);
    void unkfunc_0204d3c8(const char* text);
    void unkfunc_0204d3d8(const char* text);
    void unkfunc_0204d578(int type);
    void unkfunc_0204d5b4();
    void unkfunc_0204d70c();
    void unkfunc_0204d7c4();
    void unkfunc_0204d818();
    void unkfunc_0204d81c();
    void unkfunc_0204da24();
    void unkfunc_0204dab4();
    void unkfunc_0204db14();
    void unkfunc_0204dc10();
    void unkfunc_0204dc34();
    void unkfunc_0204dcc0();
    UnkMenuTextDisplay* unkfunc_0204dcf0();
    UnkMenuTextDisplay* unkfunc_0204dd38();
    UnkMenuTextDisplay* unkfunc_0204dd3c();
    void unkfunc_0204dd60();
    int unkfunc_0204dda4(int c);
    int unkfunc_0204ddb0(int c);
    int unkfunc_0204ddc4(int c);
    void unkfunc_0204ddf4(UnkTextIterator* it);
    void unkfunc_0204de2c(int type, const char* name, const char* text);
    void unkfunc_0204de50(const char* text);
    void unkfunc_0204de6c();
    void unkfunc_0204de7c();
    void unkfunc_0204de8c(int code);
    void unkfunc_0204def4();
    void unkfunc_0204df1c(const char* name);
    void unkfunc_0204df48();
    const char* unkfunc_0204df94();
    void unkfunc_0204dfc0();
    int unkfunc_0204dfd8();
    int unkfunc_0204e004();
    int unkfunc_0204e018();
    bool unkfunc_0204e02c();
    void unkfunc_0204e040();
    void unkfunc_0204e050();
    void unkfunc_0204e064(int flag);

    UnkTextIterator* getIterator() { return &iterator_; }
};

extern int data_020c1e54[8];             // shake offsets x
extern int data_020c1e74[8];             // shake offsets y
