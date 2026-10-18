#include <nitro/native_video.h>
#include "main/dss/DisplayPlugin.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/SystemText.hpp"
#include "nitro/gx.hpp"
#include "nitro/g2.hpp"

#define REG16(addr) (*VIDEO_REG(volatile unsigned short, (addr)))
#define REG32(addr) (*VIDEO_REG(volatile unsigned int, (addr)))

dss::DisplayPlugin_STOP dss::g_DISPLAYPLUGIN_STOP;
dss::DisplayPlugin_TEXTONLY dss::g_DISPLAYPLUGIN_TEXTONLY;
dss::DisplayPlugin_DOUBLE3D dss::g_DISPLAYPLUGIN_DOUBLE3D;
dss::DisplayPlugin_SINGLE3D dss::g_DISPLAYPLUGIN_SINGLE3D;
dss::DisplayPlugin_CAPTURE dss::g_DISPLAYPLUGIN_CAPTURE;

// plugins by type_ (unreferenced)
static dss::DisplayPlugin* const s_pluginTable[] = {
    &dss::g_DISPLAYPLUGIN_STOP, &dss::g_DISPLAYPLUGIN_CAPTURE, &dss::g_DISPLAYPLUGIN_SINGLE3D, &dss::g_DISPLAYPLUGIN_DOUBLE3D,
    &dss::g_DISPLAYPLUGIN_TEXTONLY,
};

ARM void dss::DisplayPlugin::unkfunc_020819a8(int chr)
{
    UnkOamBuffer* oam = unkfunc_0208141c(1);
    REG32(0x04001000) = (REG32(0x04001000) & ~0x60) | 0x20;
    for (int y = 0; y < 192; y += 64) {
        for (int x = 0; x < 256; x += 64) {
            G2_SetOBJAttr(oam->unkfunc_02082694(), x, y, 3, 3, 0, GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_64x64, GX_OAM_COLORMODE_16,
                          chr + (y / 8) * 32 + x / 8, 15, 0);
        }
    }
}

ARM void dss::DisplayPlugin::unkfunc_02081a38(int textType, int screen1, int screen2, int bg0, int bg1, int unk_10, int palette)
{
    unkfunc_02086e70(textType, screen1, screen2);
    if (palette) {
        loadSystemTextPalettes(textType, screen1, screen2);
    }
    unkfunc_020816dc(textType, screen1, screen2, bg0, bg1, unk_10);
    displayTextCounter_ = 2;
}

ARM void dss::DisplayPlugin::updateDisplayText()
{
    if (displayTextCounter_ > 0) {
        displayTextCounter_--;
    }
}

ARM void dss::DisplayPlugin::setDisplayText(int counter)
{
    displayTextCounter_ = counter;
}

ARM void dss::DisplayPlugin_CAPTURE::initialize()
{
    type_ = 1;
    unkfunc_02081a38(2, 1, 5, 0, 4, 1, 0);
    displayTextCounter_ = 0;
}

ARM void dss::DisplayPlugin_CAPTURE::finalize()
{
}

ARM void dss::DisplayPlugin_CAPTURE::update(int frame)
{
    int plane = frame < 2 ? 0x1c : 0x1f;
    GX_SetDispSelect(0);
    func_020648b8();
    func_020648cc();
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    func_02063f50(0x10, 4);
    GX_SetBankForObj((GXVRam)0x20);
    GX_SetBankForSubBg((GXVRam)0x80);
    GX_SetBankForSubObj((GXVRam)8);
    GX_SetGraphicsMode((GXDisplayMode)1, (GXBGMode)5, (GX2D3D)0);
    if (displayTextCounter_ != 0) {
        GX_SetVisiblePlane(4);
    } else {
        GX_SetVisiblePlane(plane & 0x1f);
    }
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    GX_SetOBJVRamModeBmp(0x20);
    G2_SetBG0Control(0, 0, 0x1f, 2, 0);
    G2_SetBG1Control(0, 0, 0x1e, 2, 0);
    G2_SetBG2ControlDCBmp(1, 0, 8);
    G2_SetBG3Control256x16Pltt(1, 0, 0xf, 0);
    GXS_SetGraphicsMode(4);
    if (displayTextCounter_ != 0) {
        GXS_SetVisiblePlane(0x10);
    } else {
        GXS_SetVisiblePlane(plane & 0x13);
    }
    unkfunc_020819a8(0);
    G2S_SetBG0Control(0, 0, 0xf, 0, 0);
    G2S_SetBG1Control(0, 0, 0xe, 0, 0);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(1);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    unkfunc_02081284(3);
    GX_SetVisibleWnd(7);
    G2_SetWndOBJInsidePlane(0x1e, 1);
    G2_SetWndOutsidePlane(0x1f, 1);
    int x;
    int y;
    unkfunc_02080e48(&x, &y);
    MtxFx22 mtx;
    func_02061b70(&mtx);
    func_02064f40((volatile void*)VIDEO_REG(u32, 0x04000030), &mtx, 128, 96, x, y);
}

ARM void dss::DisplayPlugin_DOUBLE3D::initialize()
{
    type_ = 3;
    unkfunc_02081a38(3, 1, 5, 2, 6, 0, 1);
    blur_ = 0;
    eva_ = 0x10;
    evb_ = 0;
}

ARM void dss::DisplayPlugin_DOUBLE3D::finalize()
{
}

#pragma push
#pragma opt_propagation off
ARM void dss::DisplayPlugin_DOUBLE3D::update(int frame)
{
    int main = (frame & 1) == 0 ? 1 : 0;
    if (main) {
        GX_SetDispSelect(1);
    } else {
        GX_SetDispSelect(0);
    }
    func_020648b8();
    func_020648cc();
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    GX_SetBankForBg((GXVRam)0x10);
    GX_SetBankForObj((GXVRam)0x20);
    if (main) {
        GX_SetBankForSubBg((GXVRam)4);
    } else {
        GX_SetBankForSubBg((GXVRam)0);
    }
    if (main) {
        GX_SetBankForSubObj((GXVRam)0);
    } else {
        GX_SetBankForSubObj((GXVRam)8);
    }
    if (main) {
        GX_SetBankForLcdc((GXVRam)8);
    } else {
        GX_SetBankForLcdc((GXVRam)4);
    }
    GXCaptureSrcB srcB;
    u32 dest;
    if (main) {
        srcB = 0;
        dest = 3;
        GX_SetCapture(3, 2, 0, srcB, dest, eva_, evb_);
    } else {
        srcB = 0;
        dest = 2;
        GX_SetCapture(3, 2, 0, srcB, dest, eva_, evb_);
    }
    if (main) {
        GX_SetGraphicsMode((GXDisplayMode)0xe, (GXBGMode)0, (GX2D3D)1);
    } else {
        GX_SetGraphicsMode((GXDisplayMode)0xa, (GXBGMode)0, (GX2D3D)1);
    }
    if (displayTextCounter_ != 0) {
        GX_SetVisiblePlane(1);
    } else {
        GX_SetVisiblePlane(0x17);
    }
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    GX_SetOBJVRamModeBmp(0x20);
    if (main) {
        G2_SetBG1Control(0, 0, 0xe, 0, 0);
        G2_SetBG2ControlText(0, 0, 0xf, 0);
    } else {
        G2_SetBG1Control(0, 0, 0x1e, 2, 0);
        G2_SetBG2ControlText(0, 0, 0x1f, 2);
    }
    if (main) {
        GXS_SetGraphicsMode(3);
        GXS_SetVisiblePlane(8);
        G2S_SetBG3ControlDCBmp(1, 0, 0);
    } else {
        GXS_SetGraphicsMode(3);
        GXS_SetVisiblePlane(0x10);
        unkfunc_020819a8(0);
    }
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(1);
    G2_SetBG3Priority(0);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(1);
    G2S_SetBG3Priority(3);
    GX_SetVisibleWnd(7);
    G2_SetWndOBJInsidePlane(0x1b, 1);
    G2_SetWndOutsidePlane(0x1f, 1);
    unkfunc_02081284(2);
}

#pragma pop

ARM void dss::DisplayPlugin_DOUBLE3D::ReqBlurMode(int blur)
{
    blur_ = blur;
}

ARM void dss::DisplayPlugin_DOUBLE3D::SetBlur(int eva, int evb)
{
    eva_ = eva;
    evb_ = evb;
}

ARM void dss::DisplayPlugin_SINGLE3D::initialize()
{
    type_ = 2;
    unkfunc_02081a38(2, 1, 5, 2, 6, 1, 1);
}

ARM void dss::DisplayPlugin_SINGLE3D::finalize()
{
}

ARM void dss::DisplayPlugin_SINGLE3D::update(int frame)
{
    GX_SetDispSelect(0);
    func_020648b8();
    func_020648cc();
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    GX_SetBankForBg((GXVRam)0x10);
    GX_SetBankForObj((GXVRam)0x20);
    GX_SetBankForSubBg((GXVRam)0x80);
    GX_SetBankForSubObj((GXVRam)0x100);
    GX_SetGraphicsMode((GXDisplayMode)1, (GXBGMode)3, (GX2D3D)1);
    if (displayTextCounter_ != 0) {
        GX_SetVisiblePlane(1);
    } else {
        GX_SetVisiblePlane(0x1f);
    }
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    GX_SetOBJVRamModeBmp(0x20);
    G2_SetBG1Control(0, 0, 0x1e, 2, 0);
    G2_SetBG2ControlText(0, 0, 0x1f, 2);
    G2_SetBG3Control256x16Pltt(1, 0, 0xf, 0);
    GXS_SetGraphicsMode(0);
    GXS_SetVisiblePlane(0x16);
    G2S_SetBG1Control(0, 0, 0xf, 0, 0);
    G2S_SetBG2ControlText(0, 0, 0xe, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(1);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GX_SetVisibleWnd(7);
    G2_SetWndOBJInsidePlane(0x1b, 1);
    G2_SetWndOutsidePlane(0x1f, 1);
    unkfunc_02081284(2);
    int x;
    int y;
    unkfunc_02080e48(&x, &y);
    MtxFx22 mtx;
    func_02061b70(&mtx);
    func_02064f40((volatile void*)VIDEO_REG(u32, 0x04000030), &mtx, 128, 96, x, y);
}

ARM void dss::DisplayPlugin_STOP::initialize()
{
    type_ = 0;
    unkfunc_02081a38(1, 1, 5, 2, 6, 0, 1);
}

ARM void dss::DisplayPlugin_STOP::finalize()
{
}

ARM void dss::DisplayPlugin_STOP::update(int frame)
{
}

ARM void dss::DisplayPlugin_TEXTONLY::initialize()
{
    type_ = 4;
    unkfunc_02081a38(2, 1, 5, 2, 6, 0, 1);
}

ARM void dss::DisplayPlugin_TEXTONLY::finalize()
{
}

ARM void dss::DisplayPlugin_TEXTONLY::update(int frame)
{
    GX_SetDispSelect(0);
    func_020648b8();
    func_020648cc();
    GX_ResetBankForSubBg();
    GX_ResetBankForSubObj();
    GX_SetBankForBg((GXVRam)0x10);
    GX_SetBankForObj((GXVRam)0x20);
    GX_SetBankForSubBg((GXVRam)0x80);
    GX_SetBankForSubObj((GXVRam)0x100);
    GX_SetGraphicsMode((GXDisplayMode)1, (GXBGMode)0, (GX2D3D)1);
    GX_SetVisiblePlane(0x16);
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    GX_SetOBJVRamModeBmp(0x20);
    G2_SetBG1Control(0, 0, 0x1e, 2, 0);
    G2_SetBG2ControlText(0, 0, 0x1f, 2);
    GXS_SetGraphicsMode(3);
    GXS_SetVisiblePlane(0x16);
    G2S_SetBG1Control(0, 0, 0xe, 0, 0);
    G2S_SetBG2ControlText(0, 0, 0xf, 0);
    G2S_SetBG3ControlDCBmp(1, 0, 0);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(1);
    G2S_SetBG0Priority(1);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(3);
    GX_SetVisibleWnd(7);
    G2_SetWndOBJInsidePlane(0x1b, 1);
    G2_SetWndOutsidePlane(0x1f, 1);
    unkfunc_02081284(2);
}
