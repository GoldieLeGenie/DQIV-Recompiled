#include <nitro/native_video.h>
#include "main/dss/Display.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/VramManager.hpp"
#include "nitro/g2.hpp"

ARM DssDisplay* DssDisplay::init()
{
    oam_[0].init();
    oam_[1].init();
    changeDisplayPlugin(NULL);
    setBackdropColor(0);
    unkfunc_02081274(0x7c0f);
    g_Display.capture_ = 0;
    g_Display.captureStep_ = 0;
    g_Display.captureX_ = -4;
    g_Display.captureY_ = -4;
    g_Display.blendA_ = 10;
    g_Display.blendB_ = 5;
    return this;
}

DssDisplay g_Display;

ARM void getCaptureOffset(int* x, int* y)
{
    *x = g_Display.captureX_;
    *y = g_Display.captureY_;
}

ARM void setCaptureOffset(int x, int y)
{
    g_Display.captureX_ = x;
    g_Display.captureY_ = y;
}

ARM void resetCaptureOffset()
{
    g_Display.captureX_ = -4;
    g_Display.captureY_ = -4;
}

ARM void changeDisplayPlugin(dss::DisplayPlugin* plugin)
{
    DssDisplay* display = &g_Display;
    initScreenBuffer(&g_Display.screen_, g_Display.screenData_, 32, 48);
    g_Display.nextPlugin_ = plugin;
    g_Display.plugin_ = plugin;
    g_Display.frame_ = 0;
    g_Display.blendMode_ = 0;
    setDisplayBlendMode(2);
    setBackdropColor(0);
    if (plugin == NULL) {
        return;
    }
    plugin->initialize();
    VIDEO_FILL32(0xc0, 0x07000000, 0x400);
    VIDEO_FILL32(0xc0, 0x07000400, 0x400);
    display->oam_[0].clear();
    display->oam_[1].clear();
}

ARM void clearOamBuffers()
{
    g_Display.oam_[0].clear();
    g_Display.oam_[1].clear();
}

ARM void updateDisplay()
{
    DssDisplay* display = &g_Display;
    if (isDisplayTextReady()) {
        int odd = getDisplayFrame() & 1;
        int bg = getTextBgAtY(odd * 192);
        if (bg != -1) {
            requestVramCopy(getBgScreenVramType(bg), 0, display->screenData_[odd], sizeof(display->screenData_[0]));
        }
    }
    if (!(display->frame_ & 1) && display->plugin_ != display->nextPlugin_) {
        if (display->plugin_ != NULL) {
            display->plugin_->finalize();
        }
        setWindow0Enable(0);
        setWindow1Enable(0);
        changeDisplayPlugin(display->nextPlugin_);
    }
    display->plugin_->update(display->frame_);
    display->plugin_->updateDisplayText();
    display->frame_++;
    getBgPaletteBuffer(1)->setColor(0, 0, display->backColor_);
    getBgPaletteBuffer(5)->setColor(0, 0, display->backColor_);
    display->bgPalette_[0].requestTransfer(0xf);
    display->bgPalette_[1].requestTransfer(0x1f);
    display->objPalette_[0].requestTransfer(0xe);
    display->objPalette_[1].requestTransfer(0x1e);
    display->oam_[0].requestTransfer(0x12);
    display->oam_[1].requestTransfer(0x22);
    *VIDEO_REG(volatile unsigned short, 0x0400004a) = (*VIDEO_REG(volatile unsigned short, 0x0400004a) & ~0x3f) | 0x3f;
    int window0 = display->windowEnable_[0];
    int window1 = display->windowEnable_[1];
    if (display->plugin_ == &dss::g_DISPLAYPLUGIN_DOUBLE3D && (getDisplayFrame() & 1) == 1) {
        window0 = 0;
        window1 = 0;
    }
    if (window0 != 0) {
        *VIDEO_REG(volatile unsigned short, 0x04000048) = (*VIDEO_REG(volatile unsigned short, 0x04000048) & ~0x3f) | display->window_[0].plane_ | 0x20;
        G2_SetWnd0Position(display->window_[0].x_, display->window_[0].y_, display->window_[0].x_ + display->window_[0].w_,
                           display->window_[0].y_ + display->window_[0].h_);
    } else {
        *VIDEO_REG(volatile unsigned short, 0x04000048) = (*VIDEO_REG(volatile unsigned short, 0x04000048) & ~0x3f) | 0x3f;
        G2_SetWnd0Position(0, 0, 0, 0);
    }
    if (window1 != 0) {
        *VIDEO_REG(volatile unsigned short, 0x04000048) = (*VIDEO_REG(volatile unsigned short, 0x04000048) & ~0x3f00) | (display->window_[1].plane_ << 8) | 0x2000;
        G2_SetWnd1Position(display->window_[1].x_, display->window_[1].y_, display->window_[1].x_ + display->window_[1].w_,
                           display->window_[1].y_ + display->window_[1].h_);
    } else {
        *VIDEO_REG(volatile unsigned short, 0x04000048) = (*VIDEO_REG(volatile unsigned short, 0x04000048) & ~0x3f00) | 0x3f00;
        G2_SetWnd1Position(0, 0, 0, 0);
    }
}

ARM void requestDisplayPlugin(dss::DisplayPlugin* plugin)
{
    g_Display.nextPlugin_ = plugin;
}

ARM int isDisplayTextReady()
{
    DssDisplay* display = &g_Display;
    if (display->text_.type_ == 0) {
        return 0;
    }
    return display->plugin_ == display->nextPlugin_;
}

ARM int getDisplayFrame()
{
    return g_Display.frame_;
}

ARM void setBackdropColor(unsigned short color)
{
    g_Display.backColor_ = color;
}

ARM void unkfunc_02081274(unsigned short value)
{
    g_Display.unk_0e = value;
}

ARM void setDisplayBlendMode(int mode)
{
    DssDisplay* display = &g_Display;
    *VIDEO_REG(volatile unsigned short, 0x04000050) = 0;
    *VIDEO_REG(volatile unsigned short, 0x04001050) = 0;
    if (display->blendMode_ == 1) {
        mode = 1;
    }
    int eva = display->blendA_;
    int evb = display->blendB_;
    switch (mode) {
    case 0:
        break;
    case 1:
        func_0206500c(VIDEO_REG(unsigned int, 0x04001050), 0x3d, -8);
        break;
    case 2:
        _G2_SetBlend(VIDEO_REG(unsigned int, 0x04000050), 5, 0x3b, eva, evb);
        _G2_SetBlend(VIDEO_REG(unsigned int, 0x04001050), 5, 0x3b, eva, evb);
        break;
    case 3:
        _G2_SetBlend(VIDEO_REG(unsigned int, 0x04000050), 1, 0x3e, eva, evb);
        _G2_SetBlend(VIDEO_REG(unsigned int, 0x04001050), 1, 0x3e, eva, evb);
        break;
    }
}

ARM PaletteBuffer* getBgPaletteBuffer(int bg)
{
    DssDisplay* display = &g_Display;
    switch (bg) {
    case 0:
        return &display->bgPalette_[0];
    case 1:
        return &display->bgPalette_[0];
    case 2:
        return &display->bgPalette_[0];
    case 3:
        return &display->bgPalette_[0];
    case 4:
        return &display->bgPalette_[1];
    case 5:
        return &display->bgPalette_[1];
    case 6:
        return &display->bgPalette_[1];
    case 7:
        return &display->bgPalette_[1];
    }
    return NULL;
}

ARM PaletteBuffer* getObjPaletteBuffer(int screen)
{
    DssDisplay* display = &g_Display;
    switch (screen) {
    case 0:
        return &display->objPalette_[0];
    case 1:
        return &display->objPalette_[1];
    }
    return NULL;
}

ARM OamBuffer* getOamBuffer(int screen)
{
    DssDisplay* display = &g_Display;
    switch (screen) {
    case 0:
        return &display->oam_[0];
    case 1:
        return &display->oam_[1];
    }
    return NULL;
}

ARM OamBuffer* getOamBufferAtY(int y)
{
    DssDisplay* display = &g_Display;
    int screen = 0;
    if (display->plugin_ == NULL) {
        return NULL;
    }
    if (display->nextPlugin_ == NULL) {
        return NULL;
    }
    if (y < 0) {
        return NULL;
    }
    if (y >= 384) {
        return NULL;
    }
    switch (display->text_.type_) {
    case 0:
        return NULL;
    case 1:
        if (y >= 192) {
            screen = 1;
        }
        break;
    case 2:
        if (y < 192) {
            screen = 1;
        }
        break;
    case 3:
        if ((getDisplayFrame() & 1) == 0) {
            if (y >= 192) {
                return NULL;
            }
        } else {
            if (y < 192) {
                return NULL;
            }
        }
        screen = 0;
        break;
    }
    switch (screen) {
    case 0:
        return &display->oam_[0];
    case 1:
        return &display->oam_[1];
    }
    return NULL;
}

ARM int getTextBgAtY(int y)
{
    DssDisplay* display = &g_Display;
    int ret = 0;
    if (display->plugin_ == NULL) {
        return -1;
    }
    if (display->nextPlugin_ == NULL) {
        return -1;
    }
    if (y < 0) {
        return -1;
    }
    if (y >= 384) {
        return -1;
    }
    switch (display->text_.type_) {
    case 0:
        ret = -1;
        break;
    case 1:
        ret = y < 192 ? display->text_.screen1_ : display->text_.screen2_;
        break;
    case 2:
        ret = y < 192 ? display->text_.screen2_ : display->text_.screen1_;
        break;
    case 3:
        if ((getDisplayFrame() & 1) == 0) {
            if (y >= 192) {
                return -1;
            }
        } else {
            if (y < 192) {
                return -1;
            }
        }
        ret = display->text_.screen1_;
        break;
    }
    return ret;
}

ARM int unkfunc_02081608(int y)
{
    DssDisplay* display = &g_Display;
    int ret = 0;
    if (display->plugin_ == NULL) {
        return -1;
    }
    if (display->nextPlugin_ == NULL) {
        return -1;
    }
    if (y < 0) {
        return -1;
    }
    if (y >= 384) {
        return -1;
    }
    switch (display->text_.type_) {
    case 0:
        ret = -1;
        break;
    case 1:
        ret = y < 192 ? display->text_.backScreen1_ : display->text_.backScreen2_;
        break;
    case 2:
        ret = y < 192 ? display->text_.backScreen2_ : display->text_.backScreen1_;
        break;
    case 3:
        if ((getDisplayFrame() & 1) == 0) {
            if (y >= 192) {
                return -1;
            }
        } else {
            if (y < 192) {
                return -1;
            }
        }
        ret = display->text_.backScreen1_;
        break;
    }
    return ret;
}

ARM void setDisplayTextBgs(int type, int screen1, int screen2, int backScreen1, int backScreen2, int iconEnable)
{
    g_Display.text_.type_ = type;
    g_Display.text_.screen1_ = screen1;
    g_Display.text_.screen2_ = screen2;
    g_Display.text_.backScreen1_ = backScreen1;
    g_Display.text_.backScreen2_ = backScreen2;
    g_Display.text_.iconEnable_ = iconEnable;
}

ARM int unkfunc_0208170c()
{
    return g_Display.text_.iconEnable_;
}

ARM BgScreenBuffer* getDebugScreenBuffer()
{
    return &g_Display.screen_;
}

ARM void setDisplayTextCounter(int counter)
{
    if (g_Display.plugin_ != NULL) {
        g_Display.plugin_->setDisplayText(counter);
    }
}

ARM void setWindow0Enable(int enable)
{
    g_Display.windowEnable_[0] = enable;
}

ARM void setWindow1Enable(int enable)
{
    g_Display.windowEnable_[1] = enable;
}

ARM void setWindow0(int plane, int x, int y, int w, int h)
{
    g_Display.window_[0].plane_ = plane;
    g_Display.window_[0].x_ = x;
    g_Display.window_[0].y_ = y;
    g_Display.window_[0].w_ = w;
    g_Display.window_[0].h_ = h;
}

ARM void setWindow1(int plane, int x, int y, int w, int h)
{
    g_Display.window_[1].plane_ = plane;
    g_Display.window_[1].x_ = x;
    g_Display.window_[1].y_ = y;
    g_Display.window_[1].w_ = w;
    g_Display.window_[1].h_ = h;
}

ARM int getDisplayPluginType()
{
    if (g_Display.plugin_ == NULL) {
        return 0;
    }
    return g_Display.plugin_->getType();
}

ARM void requestScreenCapture()
{
    g_Display.capture_ = 1;
    g_Display.captureStep_ = 0;
    dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(1);
    dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(0, 0x10);
}

ARM void updateScreenCapture()
{
    DssDisplay* display = &g_Display;
    if (g_Display.capture_ == 0) {
        return;
    }
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    switch (display->captureStep_) {
    case 0:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06860000, 0x8000), 0, 0x8000, 0);
        break;
    case 1:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06868000, 0x8000), 0x8000, 0x8000, 0);
        break;
    case 2:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06870000, 0x8000), 0x10000, 0x8000, 0);
        break;
    case 3:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06840000, 0x8000), 0x20000, 0x8000, 0);
        break;
    case 4:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06848000, 0x8000), 0x28000, 0x8000, 0);
        break;
    case 5:
        g_VramTransfer.transferToVram(0, VIDEO_LCDC_SOURCE(0x06850000, 0x8000), 0x30000, 0x8000, 0);
        break;
    }
    if (display->captureStep_ < 6) {
        display->captureStep_++;
    }
    if (display->captureStep_ == 6) {
        display->capture_ = 0;
    }
}

ARM int isScreenCaptureDone()
{
    return g_Display.captureStep_ == 6;
}
