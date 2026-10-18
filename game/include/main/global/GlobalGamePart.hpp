#pragma once
#include <globaldefs.h>
#include "main/global/GlobalDQ4.hpp"

enum GAME_PART {
    MENU_PART = 0,
    TITLE_PART = 1,
    LOGO_PART = 2,
    GAME_START_PART = 3,
    GAME_END_PART = 4,
    MAP_VIEWER_PART = 5,
    CHARACTER_VIEWER_PART = 6,
    FLD_VIEWER_PART = 7,
    SSA_VIEWER_PART = 8,
    CHR_VIEWER_PART = 9,
    FIELD_VIEWER_PART = 10,
    CARDCHECK_PART = 11,
    TOWN_PART = 12,
    BATTLE_PART = 13,
    FIELD_PART = 14,
    CASINO_PART = 15,
    BOOK_PART = 16,
    BATTLE_MENU_PART = 17,
    MACADDRESS_PART = 18,
    MPEXCHANGE_PART = 19,
    TGS_MESSAGE_PART = 20,
    ISHIKUROTEST_PART = 21,
    MESSAGEDEBUG_PART = 22,
    IMATEST_PART = 23,
    FUKAYATEST_PART = 24,
    SHIKATATEST_PART = 25,
    NONE_PART = 26,
    END_PART = 27
};

// Base of the game parts (TownPart, FieldPart, BattlePart, CasinoPart, BookPart...). DS: it is also the
// GlobalDQ4 task, the UnkGameTask slots call the part virtuals (TU 0x0203ed64, not decompiled)
struct GlobalGamePart : UnkGameTask {
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0c();
    virtual void vf10();
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    virtual void onSwapBuffersPart();

    int unkfunc_0203f210();                     // execute the part (no debug/extra menu)
};

// DS-only: parts run every frame on top of the current game part (fade, wait, debug menu...)
struct UnkGlobalPart {
#ifdef DQIV_NATIVE
    virtual void update() = 0;
    virtual void draw();
    virtual int isEnd() = 0;
#else
    virtual void update();
    virtual void draw();
    virtual int isEnd();
#endif
};

struct GlobalGamePartManager {
    UnkGlobalPart* part_[8];                    // 0x00

    GlobalGamePartManager();
    ~GlobalGamePartManager();
    void unkfunc_020581f4();                    //
    void unkfunc_02058244();                    //
    void unkfunc_02058294(UnkGlobalPart* part);    // add
    void unkfunc_020582b8(UnkGlobalPart* part);    // remove
};

extern GlobalGamePartManager data_0210bc18;
extern int data_020c1b7c;

void setPartFrameCallback(void (*callback)());       // set the per-frame callback

extern "C" {
    void ov001_entry(void);                     // called by the parts right after loading the overlay
    void ov003_entry(void);                     // idem ov003
    void ov015_entry(void);                     // idem ov015
    void ov016_entry(void);             // idem ov016
}
