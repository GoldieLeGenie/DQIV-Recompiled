#pragma ipa file
#include "main/part/MenuPart.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/Display.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Render.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/dss/Sprite2D.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"

static int s_frame;
MenuPart g_MenuPart;
static int s_isEnd;
static Render s_render;
static MenuSprite s_logo[2];

ARM void MenuPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    g_VramTransfer.initVramManagers(0x40000, 0x4000);
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    SoundManager::stop(0);
    if (getGameRegion() == 2) {
        s_logo[0].loadFile("data/2d/sqex_logo_na.tex", 0);
    } else {
        s_logo[0].loadFile("data/2d/sqex_logo_eu.tex", 0);
    }
    s_logo[0].setSubScreen();
    s_logo[0].setPolygonID(1);
    s_logo[0].setAlpha(0x1f);
    if (getGameRegion() == 2) {
        s_logo[1].loadFile("data/2d/arte_logo_na.tex", 0);
    } else {
        s_logo[1].loadFile("data/2d/arte_logo_eu.tex", 0);
    }
    s_logo[1].setMainScreen();
    s_logo[1].setPolygonID(1);
    s_logo[1].setAlpha(0x1f);
    s_isEnd = 0;
    s_frame = 0;
    g_Global.fadeInBlack(60);
}

ARM void MenuPart::terminate()
{
    s_logo[0].unload();
    s_logo[1].unload();
    g_VramTransfer.unkfunc_02086278();
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
    setupOrtho2DBackground();
    s_logo[0].draw();
    s_logo[1].draw();
}

ARM void MenuPart::onWindowPart()
{
}

ARM void MenuPart::onDebugPart()
{
}
