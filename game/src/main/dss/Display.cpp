#include <nitro/native_video.h>
#include "main/dss/Display.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/VramManager.hpp"
#include "nitro/g2.hpp"

ARM UnkDisplay* UnkDisplay::init()
{
    oam_[0].init();
    oam_[1].init();
    changeDisplayPlugin(NULL);
    setBackdropColor(0);
    unkfunc_02081274(0x7c0f);
    data_0211a7d0.capture_ = 0;
    data_0211a7d0.captureStep_ = 0;
    data_0211a7d0.captureX_ = -4;
    data_0211a7d0.captureY_ = -4;
    data_0211a7d0.blendA_ = 10;
    data_0211a7d0.blendB_ = 5;
    return this;
}

UnkDisplay data_0211a7d0;

ARM void getCaptureOffset(int* x, int* y)
{
    *x = data_0211a7d0.captureX_;
    *y = data_0211a7d0.captureY_;
}

ARM void setCaptureOffset(int x, int y)
{
    data_0211a7d0.captureX_ = x;
    data_0211a7d0.captureY_ = y;
}

ARM void resetCaptureOffset()
{
    data_0211a7d0.captureX_ = -4;
    data_0211a7d0.captureY_ = -4;
}

ARM void changeDisplayPlugin(dss::DisplayPlugin* plugin)
{
    UnkDisplay* display = &data_0211a7d0;
    initScreenBuffer(&data_0211a7d0.screen_, data_0211a7d0.screenData_, 32, 48);
    data_0211a7d0.nextPlugin_ = plugin;
    data_0211a7d0.plugin_ = plugin;
    data_0211a7d0.frame_ = 0;
    data_0211a7d0.blendMode_ = 0;
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
    data_0211a7d0.oam_[0].clear();
    data_0211a7d0.oam_[1].clear();
}

ARM void updateDisplay()
{
    UnkDisplay* display = &data_0211a7d0;
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
    data_0211a7d0.nextPlugin_ = plugin;
}

ARM int isDisplayTextReady()
{
    UnkDisplay* display = &data_0211a7d0;
    if (display->text_.type_ == 0) {
        return 0;
    }
    return display->plugin_ == display->nextPlugin_;
}

ARM int getDisplayFrame()
{
    return data_0211a7d0.frame_;
}

ARM void setBackdropColor(unsigned short color)
{
    data_0211a7d0.backColor_ = color;
}

ARM void unkfunc_02081274(unsigned short value)
{
    data_0211a7d0.unk_0e = value;
}

ARM void setDisplayBlendMode(int mode)
{
    UnkDisplay* display = &data_0211a7d0;
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

ARM UnkPaletteBuffer* getBgPaletteBuffer(int bg)
{
    UnkDisplay* display = &data_0211a7d0;
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

ARM UnkPaletteBuffer* getObjPaletteBuffer(int screen)
{
    UnkDisplay* display = &data_0211a7d0;
    switch (screen) {
    case 0:
        return &display->objPalette_[0];
    case 1:
        return &display->objPalette_[1];
    }
    return NULL;
}

ARM UnkOamBuffer* getOamBuffer(int screen)
{
    UnkDisplay* display = &data_0211a7d0;
    switch (screen) {
    case 0:
        return &display->oam_[0];
    case 1:
        return &display->oam_[1];
    }
    return NULL;
}

ARM UnkOamBuffer* getOamBufferAtY(int y)
{
    UnkDisplay* display = &data_0211a7d0;
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
    UnkDisplay* display = &data_0211a7d0;
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
    UnkDisplay* display = &data_0211a7d0;
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
        ret = y < 192 ? display->text_.unk_0c : display->text_.unk_10;
        break;
    case 2:
        ret = y < 192 ? display->text_.unk_10 : display->text_.unk_0c;
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
        ret = display->text_.unk_0c;
        break;
    }
    return ret;
}

ARM void setDisplayTextBgs(int type, int screen1, int screen2, int unk_0c, int unk_10, int unk_14)
{
    data_0211a7d0.text_.type_ = type;
    data_0211a7d0.text_.screen1_ = screen1;
    data_0211a7d0.text_.screen2_ = screen2;
    data_0211a7d0.text_.unk_0c = unk_0c;
    data_0211a7d0.text_.unk_10 = unk_10;
    data_0211a7d0.text_.unk_14 = unk_14;
}

ARM int unkfunc_0208170c()
{
    return data_0211a7d0.text_.unk_14;
}

ARM UnkScreenBuffer* getDebugScreenBuffer()
{
    return &data_0211a7d0.screen_;
}

ARM void setDisplayTextCounter(int counter)
{
    if (data_0211a7d0.plugin_ != NULL) {
        data_0211a7d0.plugin_->setDisplayText(counter);
    }
}

ARM void setWindow0Enable(int enable)
{
    data_0211a7d0.windowEnable_[0] = enable;
}

ARM void setWindow1Enable(int enable)
{
    data_0211a7d0.windowEnable_[1] = enable;
}

ARM void setWindow0(int plane, int x, int y, int w, int h)
{
    data_0211a7d0.window_[0].plane_ = plane;
    data_0211a7d0.window_[0].x_ = x;
    data_0211a7d0.window_[0].y_ = y;
    data_0211a7d0.window_[0].w_ = w;
    data_0211a7d0.window_[0].h_ = h;
}

ARM void setWindow1(int plane, int x, int y, int w, int h)
{
    data_0211a7d0.window_[1].plane_ = plane;
    data_0211a7d0.window_[1].x_ = x;
    data_0211a7d0.window_[1].y_ = y;
    data_0211a7d0.window_[1].w_ = w;
    data_0211a7d0.window_[1].h_ = h;
}

ARM int getDisplayPluginType()
{
    if (data_0211a7d0.plugin_ == NULL) {
        return 0;
    }
    return data_0211a7d0.plugin_->getType();
}

ARM void requestScreenCapture()
{
    data_0211a7d0.capture_ = 1;
    data_0211a7d0.captureStep_ = 0;
    dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(1);
    dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(0, 0x10);
}

ARM void updateScreenCapture()
{
    UnkDisplay* display = &data_0211a7d0;
    if (data_0211a7d0.capture_ == 0) {
        return;
    }
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    switch (display->captureStep_) {
    case 0:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06860000, 0x8000), 0, 0x8000, 0);
        break;
    case 1:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06868000, 0x8000), 0x8000, 0x8000, 0);
        break;
    case 2:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06870000, 0x8000), 0x10000, 0x8000, 0);
        break;
    case 3:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06840000, 0x8000), 0x20000, 0x8000, 0);
        break;
    case 4:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06848000, 0x8000), 0x28000, 0x8000, 0);
        break;
    case 5:
        data_0211e450.transferToVram(0, VIDEO_LCDC_SOURCE(0x06850000, 0x8000), 0x30000, 0x8000, 0);
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
    return data_0211a7d0.captureStep_ == 6;
}
