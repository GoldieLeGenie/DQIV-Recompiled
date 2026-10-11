#pragma ipa file
#include "main/part/MenuPart.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Render.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"

static int s_frame;
MenuPart g_MenuPart;
static int s_isEnd;
static Render s_render;
static UnkMenuSprite s_logo[2];

ARM void MenuPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    SoundManager::stop(0);
    if (unkfunc_0208a104() == 2) {
        s_logo[0].unkfunc_02057d60("data/2d/sqex_logo_na.tex", 0);
    } else {
        s_logo[0].unkfunc_02057d60("data/2d/sqex_logo_eu.tex", 0);
    }
    s_logo[0].unkfunc_02057ee8();
    s_logo[0].unkfunc_02057f18(1);
    s_logo[0].unkfunc_02057f00(0x1f);
    if (unkfunc_0208a104() == 2) {
        s_logo[1].unkfunc_02057d60("data/2d/arte_logo_na.tex", 0);
    } else {
        s_logo[1].unkfunc_02057d60("data/2d/arte_logo_eu.tex", 0);
    }
    s_logo[1].unkfunc_02057edc();
    s_logo[1].unkfunc_02057f18(1);
    s_logo[1].unkfunc_02057f00(0x1f);
    s_isEnd = 0;
    s_frame = 0;
    g_Global.fadeInBlack(60);
}

ARM void MenuPart::terminate()
{
    s_logo[0].unkfunc_02057e34();
    s_logo[1].unkfunc_02057e34();
    data_0211e450.unkfunc_02086278();
}

ARM void MenuPart::onExecutePart()
{
    s_frame++;
    if (s_isEnd == 0 && s_frame > 240) {
        g_Global.startTitle();
        s_isEnd = 1;
    }
}

ARM void MenuPart::onDrawPart()
{
    unkfunc_02084964();
    s_logo[0].unkfunc_02057ec0();
    s_logo[1].unkfunc_02057ec0();
}

ARM void MenuPart::onWindowPart()
{
}

ARM void MenuPart::onDebugPart()
{
}
