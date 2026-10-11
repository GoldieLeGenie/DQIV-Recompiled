#pragma once
#include "main/menu/MenuBase.hpp"

// Menu API test pages (L/R: page, A/X: message window, B: close)
struct CommonMenu_APITEST : menu::MenuBase {
    int page_;                                  /* 0x1C */
    int unk_20;                                 /* 0x20  first value shown */
    const char* title_;                         /* 0x24 */
    int wait_;                                  /* 0x28 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate() {}

    void unkfunc_02025e4c();
    void unkfunc_02025f70();                    // cursor
    void unkfunc_02026020();                    // window
    void unkfunc_02026050();                    // icons
    void unkfunc_020260fc();                    // circle
    void unkfunc_0202612c();                    // background boards and frames
    void unkfunc_0202615c();                    // system font
    void unkfunc_0202618c();                    // auto connection
    void unkfunc_020261bc();                    // face
    void unkfunc_020261f0();                    // texts
    void unkfunc_0202626c();                    // battle background
    void unkfunc_020262a0();                    // gauges
    void unkfunc_02026330();                    // text lists
    void unkfunc_02026380();                    // number lists
    void unkfunc_020263c8();                    // icons
    void unkfunc_02026408();
    void unkfunc_02026448();
    void unkfunc_0202648c();
    void unkfunc_020264d0();                    // frames
    void unkfunc_02026534();                    // connection lines
    void unkfunc_02026570();                    // text concatenation
    void unkfunc_020265d8();                    // half/full width conversion
    void unkfunc_02026668();
    void unkfunc_020266a0();                    // half/full width display
    void unkfunc_020266e0(int x, int y, int w, int h);  // frame
    void unkfunc_02026714();                    // name plates
    void unkfunc_02026760();                    // new frames
    void unkfunc_02026790();                    // 4 faces
};

extern CommonMenu_APITEST data_020ed068;        /* gCommonMenu_APITEST */
