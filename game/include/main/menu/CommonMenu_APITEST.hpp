#pragma once
#include "main/menu/MenuBase.hpp"

// Menu API test pages (L/R: page, A/X: message window, B: close)
struct CommonMenu_APITEST : menu::MenuBase {
    int page_;                                  /* 0x1C */
    int baseValue_;                                 /* 0x20  first value shown */
    const char* title_;                         /* 0x24 */
    int wait_;                                  /* 0x28 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate() {}

    void unkfunc_02025e4c();
    void drawCursorTestPage();                    // cursor
    void drawWindowTestPage();                    // window
    void drawIconTestPage();                    // icons
    void drawCircleTestPage();                    // circle
    void drawPlateFrameTestPage();                    // background boards and frames
    void drawSystemFontTestPage();                    // system font
    void drawAutoConnectTestPage();                    // auto connection
    void drawFaceTestPage();                    // face
    void drawTextTestPage();                    // texts
    void drawBattleBgTestPage();                    // battle background
    void drawMeterTestPage();                    // gauges
    void drawStringPlateTestPage();                    // text lists
    void drawNumberPlateTestPage();                    // number lists
    void drawIconTestPage2();                    // icons
    void drawIconTestPage3();
    void drawIconTestPage4();
    void drawIconTestPage5();
    void drawStackedWindowsTestPage();                    // frames
    void drawConnectLineTestPage();                    // connection lines
    void drawTextConcatTestPage();                    // text concatenation
    void drawWidthConvertTestPage();                    // half/full width conversion
    void drawWidthConvertTestPage2();
    void drawWidthDisplayTestPage();                    // half/full width display
    void drawTestWindow(int x, int y, int w, int h);  // frame
    void drawNamePlateTestPage();                    // name plates
    void drawNewFrameTestPage();                    // new frames
    void drawFourFacesTestPage();                    // 4 faces
};

extern CommonMenu_APITEST g_CommonMenuApiTest;        /* gCommonMenu_APITEST */
