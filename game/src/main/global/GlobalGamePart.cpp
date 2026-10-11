#include <nitro/native_g3.h>
#include "GameInfo.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/ScreenPosition.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/UnkFog.hpp"
#include "main/dss/UnkSleep.hpp"
#include "main/dss/UnkVramRequest.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/UnkMenuSystem.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/sound/UnkSoundPlayer.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/g3.hpp"
#include "nitro/gx.h"
#include "nitro/os.hpp"
#include <nitro/os/interrupt.h>
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#include "main/dss/DisplayPlugin.hpp"
#include "main/effect/UnkScreenEffectManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/cmn/CommonChapterTitle.hpp"
#endif

extern "C" {
    void func_02065194(void);
    void func_020652c0(void);
}

static void unkfunc_0203f1a8();
static void unkfunc_0203f1b4();
static void unkfunc_0203f1f4();

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
GlobalGamePart data_020f20cc;
static long long s_taskTick;
static unsigned int s_lineLog[2][16];
static unsigned int s_tickLog[2][16];
UnkSpriteFade data_020f21f8;

ARM void GlobalGamePart::vf00()
{
    s_taskTick = unkfunc_0207e7e8();
    unkfunc_02052430();
    TextAPI::Init();
    data_0211e450.unk_814 = 1;
    initialize();
    data_0211e450.unk_814 = 0;
    unkfunc_0203f1a8();
    unkfunc_02052440();
    TextAPI::unkfunc_020547f0();
    s_taskTick = unkfunc_0207e7e8() - s_taskTick;
}

ARM void GlobalGamePart::initialize()
{
}

ARM void GlobalGamePart::vf04()
{
    s_taskTick = unkfunc_0207e7e8();
    data_02116ce0.unkfunc_0207e810();
    unkfunc_02052450();
    TextAPI::unkfunc_020547f4();
    terminate();
    unkfunc_0203f1a8();
    s_taskTick = unkfunc_0207e7e8() - s_taskTick;
}

ARM void GlobalGamePart::terminate()
{
}

ARM void GlobalGamePart::vf08()
{
    func_02065194();
    func_020652c0();
    dss::g_Pad.unkfunc_0207f1f8();
    unkfunc_02080f3c();
    unkfunc_02052454();
    TextAPI::unkfunc_0205487c();
}

ARM void GlobalGamePart::vf10()
{
    unkfunc_02089470();
    if (GX_GetVCount() > 180) {
        while (GX_GetVCount() > 180) {
        }
    }
    s_swapLine = GX_GetVCount();
    unkfunc_02052478();
    TextAPI::unkfunc_02054888();
    func_0206dcf0();
    onSwapBuffersPart();
    unkfunc_0205714c();
    unkfunc_020832d8();
    s_drawLine = GX_GetVCount();
    OS_Wait();
    unkfunc_0203f1b4();
    unkfunc_0203f1f4();
    unkfunc_02081814();
    s_textureSize = data_0211e450.texSize_;
    data_0211e450.unkfunc_02086454();
    s_frameEndLine = GX_GetVCount();
    if (REG_GFX_VTX_RAM_COUNT == 0) {
        unkfunc_02080f5c();
        unkfunc_02082724();
    }
    data_0210bc18.unkfunc_02058244();
    s_endLine = GX_GetVCount();
}

ARM void GlobalGamePart::vf0c()
{
    int index = (unkfunc_02081254() & 1) ? 0 : 1;
    s_tickLog[index][0] = unkfunc_0207e7e8();
    s_lineLog[index][0] = GX_GetVCount();
    unkfunc_02052464();
    TextAPI::unkfunc_02054884();
    if (unkfunc_0203f210()) {
        onExecutePart();
        unkfunc_0203f1a8();
    }
    s_tickLog[index][1] = unkfunc_0207e7e8();
    s_lineLog[index][1] = GX_GetVCount();
#ifdef DQIV_NATIVE
    // Towns and dungeons view one scene through the two screens' cameras.
    // Screen transition effects texture each screen with its own capture:
    // they keep the DS presentation.
    UnkScreenEffectManager* effects = UnkScreenEffectManager::getSingleton();
    // A chapter title composes its screen from two frames (the picture and the
    // plaque come on alternate frames): it is shown as its own panel.
    const bool chapterTitle = cmn::CommonChapterTitle::getSingleton()->sprite_.unkfunc_02057e74() != 0;
    pc::setUnifiedFrame(pc::dualScreen().unified && data_0211a7d0.plugin_ == &dss::g_DISPLAYPLUGIN_DOUBLE3D &&
                        data_0210bb94.unkfunc_02058114(TOWN_PART) && !effects->unkfunc_0202af54() && !chapterTitle);
    pc::setWideFieldFrame(pc::dualScreen().unified && data_0211a7d0.plugin_ == &dss::g_DISPLAYPLUGIN_DOUBLE3D &&
                          data_0210bb94.unkfunc_02058114(FIELD_PART) && !effects->unkfunc_0202af54());
    pc::setWideBattleFrame(pc::dualScreen().unified && data_0211a7d0.plugin_ == &dss::g_DISPLAYPLUGIN_SINGLE3D &&
                           data_0210bb94.unkfunc_02058114(BATTLE_PART)); // battles never draw screen effects
    // Scenes without a world place their screens as panels; the monster book
    // only uses the bottom screen.
    pc::setPanelLayout(pc::dualScreen().unified && (chapterTitle || (!data_0210bb94.unkfunc_02058114(TOWN_PART) &&
                       !data_0210bb94.unkfunc_02058114(FIELD_PART) && !data_0210bb94.unkfunc_02058114(BATTLE_PART))));
    pc::setPanelFocus(chapterTitle || data_0210bb94.unkfunc_02058114(BOOK_PART) ? 1 : -1);
    pc::setStartScreen(data_0210bb94.unkfunc_02058114(GAME_START_PART));
#endif
    onDrawPart();
    s_tickLog[index][2] = unkfunc_0207e7e8();
    s_lineLog[index][2] = GX_GetVCount();
    unkfunc_020848a8();
    data_020f21f8.sprite_[0].draw();
    data_020f21f8.sprite_[1].draw();
    onWindowPart();
    s_tickLog[index][3] = unkfunc_0207e7e8();
    s_lineLog[index][3] = GX_GetVCount();
    onDebugPart();
    s_tickLog[index][4] = unkfunc_0207e7e8();
    s_lineLog[index][4] = GX_GetVCount();
    data_0210bc18.unkfunc_020581f4();
    data_0211fc7c.unkfunc_020866d8();
    s_tickLog[index][5] = unkfunc_0207e7e8();
    s_lineLog[index][5] = GX_GetVCount();
    if (s_callback) {
        s_callback();
    }
    while (REG_GFX_STATUS & 0x8000000) {
    }
    s_tickLog[index][6] = unkfunc_0207e7e8();
    s_lineLog[index][6] = GX_GetVCount();
    s_tickLog[index][7] = unkfunc_0207e7e8();
    s_lineLog[index][7] = GX_GetVCount();
    s_tickLog[index][8] = unkfunc_0207e7e8();
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

ARM static void unkfunc_0203f1b4()
{
    if (!(REG_DISP3DCNT & 0x1000)) {
        s_polygonOverflow = 0;
        return;
    }
    REG_DISP3DCNT |= 0x1000;
    s_polygonOverflow = 1;
}

ARM static void unkfunc_0203f1f4()
{
    s_renderedLines = REG_RDLINES_COUNT;
}

ARM int GlobalGamePart::unkfunc_0203f210()
{
    if (data_0210bc40.unkfunc_02058378()) {
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

ARM void unkfunc_0203f268(void (*callback)())
{
    s_callback = callback;
}

ARM int UnkSpriteFade::isEnd()
{
    return count_ != frames_ ? RESULT_FALSE : RESULT_TRUE;
}

ARM void UnkSpriteFade::draw()
{
}

ARM void UnkSpriteFade::update()
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
