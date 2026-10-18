#include <nitro/native_g3.h>
#include "GameInfo.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/ScreenPosition.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/Fog.hpp"
#include "main/dss/SleepControl.hpp"
#include "main/dss/VramRequest.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/MenuSystem.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/sound/SoundPlayer.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/g3.hpp"
#include "nitro/gx.h"
#include "nitro/os.hpp"
#include <nitro/os/interrupt.h>
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#include "Settings.hpp"
#include "main/dss/DisplayPlugin.hpp"
#include "main/effect/ScreenEffectManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/cmn/CommonChapterTitle.hpp"
#endif

extern "C" {
    void func_02065194(void);
    void func_020652c0(void);
}

static void unkfunc_0203f1a8();
static void updatePolygonOverflow();
static void updateRenderedLines();

int data_020c1b7c = 2;
static unsigned int s_swapLine;
static unsigned int s_frameEndLine;
static int s_unk28;
static void (*s_callback)();
static unsigned int s_drawLine;
static unsigned int s_endLine;
static unsigned int s_renderedLines;
static int s_unk14;
static int s_textureSize;
static int s_polygonOverflow;
GlobalGamePart g_GlobalGamePart;
static long long s_taskTick;
static unsigned int s_lineLog[2][16];
static unsigned int s_tickLog[2][16];
GlobalSpriteFade g_SpriteFade;

ARM void GlobalGamePart::vf00()
{
    s_taskTick = getTick();
    initMenuSystem();
    TextAPI::Init();
    g_VramTransfer.unk_814 = 1;
    initialize();
    g_VramTransfer.unk_814 = 0;
    unkfunc_0203f1a8();
    startMenuSystem();
    TextAPI::start();
    s_taskTick = getTick() - s_taskTick;
}

ARM void GlobalGamePart::initialize()
{
}

ARM void GlobalGamePart::vf04()
{
    s_taskTick = getTick();
    g_DebugConsole.unkfunc_0207e810();
    endMenuSystem();
    TextAPI::terminate();
    terminate();
    unkfunc_0203f1a8();
    s_taskTick = getTick() - s_taskTick;
}

ARM void GlobalGamePart::terminate()
{
}

ARM void GlobalGamePart::vf08()
{
    func_02065194();
    func_020652c0();
    dss::g_Pad.update();
    clearOamBuffers();
    updateMenuSystem();
    TextAPI::update();
}

ARM void GlobalGamePart::vf10()
{
    updateSleep();
    if (GX_GetVCount() > 180) {
        while (GX_GetVCount() > 180) {
        }
    }
    s_swapLine = GX_GetVCount();
    drawMenuSystem();
    TextAPI::draw();
    func_0206dcf0();
    onSwapBuffersPart();
    updateScreenPositions();
    updateFog();
    s_drawLine = GX_GetVCount();
    OS_Wait();
    updatePolygonOverflow();
    updateRenderedLines();
    updateScreenCapture();
    s_textureSize = g_VramTransfer.texSize_;
    g_VramTransfer.doTransfers();
    s_frameEndLine = GX_GetVCount();
    if (REG_GFX_VTX_RAM_COUNT == 0) {
        updateDisplay();
        flushVramRequests();
    }
    g_GlobalGamePartManager.unkfunc_02058244();
    s_endLine = GX_GetVCount();
}

ARM void GlobalGamePart::vf0c()
{
    int index = (getDisplayFrame() & 1) ? 0 : 1;
    s_tickLog[index][0] = getTick();
    s_lineLog[index][0] = GX_GetVCount();
    executeMenuSystem();
    TextAPI::execute();
    if (unkfunc_0203f210()) {
        onExecutePart();
        unkfunc_0203f1a8();
    }
    s_tickLog[index][1] = getTick();
    s_lineLog[index][1] = GX_GetVCount();
#ifdef DQIV_NATIVE
    // Towns and dungeons view one scene through the two screens' cameras.
    // Screen transition effects texture each screen with its own capture:
    // they keep the DS presentation.
    ScreenEffectManager* effects = ScreenEffectManager::getSingleton();
    // A chapter title composes its screen from two frames (the picture and the
    // plaque come on alternate frames): it is shown as its own panel.
    const bool chapterTitle = cmn::CommonChapterTitle::getSingleton()->sprite_.isLoaded() != 0;
    pc::setUnifiedFrame(pc::dualScreen().unified && g_Display.plugin_ == &dss::g_DISPLAYPLUGIN_DOUBLE3D &&
                        g_GlobalDQ4.unkfunc_02058114(TOWN_PART) && !effects->isEffectDrawing() && !chapterTitle);
    pc::setWideFieldFrame(pc::dualScreen().unified && g_Display.plugin_ == &dss::g_DISPLAYPLUGIN_DOUBLE3D &&
                          g_GlobalDQ4.unkfunc_02058114(FIELD_PART) && !effects->isEffectDrawing() && !chapterTitle);
    // Battles never draw screen effects; the player may keep their DS layout.
    pc::setWideBattleFrame(pc::dualScreen().unified && pc::gameSettings().battleLayout != pc::battleDsScreens &&
                           g_Display.plugin_ == &dss::g_DISPLAYPLUGIN_SINGLE3D && g_GlobalDQ4.unkfunc_02058114(BATTLE_PART));
    // The large layout needs room beside the enlarged battle screen (16:9).
    pc::setLargeBattle(pc::gameSettings().battleLayout == pc::battleLarge && pc::dualScreen().canvasWidth >= 600);
    // Scenes without a world place their screens as panels; the monster book
    // only uses the bottom screen.
    pc::setPanelLayout(pc::dualScreen().unified && (chapterTitle || (!g_GlobalDQ4.unkfunc_02058114(TOWN_PART) &&
                       !g_GlobalDQ4.unkfunc_02058114(FIELD_PART) && !g_GlobalDQ4.unkfunc_02058114(BATTLE_PART))));
    pc::setPanelFocus(chapterTitle || g_GlobalDQ4.unkfunc_02058114(BOOK_PART) ? 1 : -1);
    pc::setStartScreen(g_GlobalDQ4.unkfunc_02058114(GAME_START_PART));
#endif
    onDrawPart();
    s_tickLog[index][2] = getTick();
    s_lineLog[index][2] = GX_GetVCount();
    setupOrtho2DFine();
    g_SpriteFade.sprite_[0].draw();
    g_SpriteFade.sprite_[1].draw();
    onWindowPart();
    s_tickLog[index][3] = getTick();
    s_lineLog[index][3] = GX_GetVCount();
    onDebugPart();
    s_tickLog[index][4] = getTick();
    s_lineLog[index][4] = GX_GetVCount();
    g_GlobalGamePartManager.unkfunc_020581f4();
    g_SoundSystem.unkfunc_020866d8();
    s_tickLog[index][5] = getTick();
    s_lineLog[index][5] = GX_GetVCount();
    if (s_callback) {
        s_callback();
    }
    while (REG_GFX_STATUS & 0x8000000) {
    }
    s_tickLog[index][6] = getTick();
    s_lineLog[index][6] = GX_GetVCount();
    s_tickLog[index][7] = getTick();
    s_lineLog[index][7] = GX_GetVCount();
    s_tickLog[index][8] = getTick();
    s_lineLog[index][8] = GX_GetVCount();
}

ARM void GlobalGamePart::onExecutePart()
{
}

ARM void GlobalGamePart::onDrawPart()
{
}

ARM void GlobalGamePart::onWindowPart()
{
}

ARM void GlobalGamePart::onDebugPart()
{
}

ARM static void unkfunc_0203f1a8()
{
    unkfunc_0207e7e4();
}

ARM static void updatePolygonOverflow()
{
    if (!(REG_DISP3DCNT & 0x1000)) {
        s_polygonOverflow = 0;
        return;
    }
    REG_DISP3DCNT |= 0x1000;
    s_polygonOverflow = 1;
}

ARM static void updateRenderedLines()
{
    s_renderedLines = REG_RDLINES_COUNT;
}

ARM int GlobalGamePart::unkfunc_0203f210()
{
    if (g_DebugMenu.unkfunc_02058378()) {
        return 0;
    }
    return MenuManager::isExtraMenu() == 0;
}

ARM void GlobalGamePart::onSwapBuffersPart()
{
    BOOL enable = OS_DisableIME();
    G3_WRITE(0x50, 3);
    OS_RestoreIME(enable);
}

ARM void setPartFrameCallback(void (*callback)())
{
    s_callback = callback;
}

ARM int GlobalSpriteFade::isEnd()
{
    return count_ != frames_ ? RESULT_FALSE : RESULT_TRUE;
}

ARM void GlobalSpriteFade::draw()
{
}

ARM void GlobalSpriteFade::update()
{
    count_++;
    count_ = dss::clamp<int>(count_, 0, frames_);
    alpha_ = count_ * 31 / frames_;
    switch (state_) {
    case GlobalFade::FADE_NONE:
        break;
    case GlobalFade::FADE_OUT_BLACK:
        alpha_ = 31 - alpha_;
        break;
    case GlobalFade::FADE_OUT_WHITE:
        alpha_ = 31 - alpha_;
        break;
    case 6:
        alpha_ = 31 - alpha_;
        break;
    }
    alpha_ = dss::clamp<int>(alpha_, 0, 31);
    sprite_[0].setAlpha((unsigned char)alpha_);
    sprite_[1].setAlpha((unsigned char)alpha_);
}
