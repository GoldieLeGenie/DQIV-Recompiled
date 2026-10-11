#pragma ipa file
#include "main/part/LogoPart.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/data/FileLoader.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/Render.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/fld/FldStage.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/object/ModelObject.hpp"
#include "main/sound/Sound.hpp"
#include "main/status/StoryStatus.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"

static const int s_animFileNum = 2;
static const float s_distanceF = 39.55f;

// bound to a reference: the constant is folded but s_animFileNum still gets emitted in .rodata
static inline const int& getAnimFileNum() { return s_animFileNum; }

LogoPart g_LogoPart;
static dss::DualCamera s_camera;
static Render s_render;
static FldStage s_stage;
static const char* s_mapPath = "data/map";
static dss::Fix32Vector3 s_target(0, 0, 0);
static dss::Vector3<short> s_angle(-0x1e00, 0, 0);
static dss::Fix32 s_distance(s_distanceF);
static dss::Fix32 s_offset(0.8f);
static int s_frame;
static ModelObject s_model;
static UnkMenuSprite s_sprite;
static short s_fov = 10;

ARM static void unkfunc_020099f0()
{
    char path[128];
    int index = 0;

    dss::sprintf_s(path, sizeof(path), "data/chr/%s.nsbmd", "msdr");
    s_model.setup(path);
    for (int i = 0; i < getAnimFileNum(); i++) {
        dss::sprintf_s(path, sizeof(path), "data/chr/%s_%d.nsbma", "msdr", i);
        if (dss::g_File.isExist(path)) {
            s_model.unkfunc_020587d4(path, index);
            index++;
        }
        dss::sprintf_s(path, sizeof(path), "data/chr/%s_%d.nsbta", "msdr", i);
        if (dss::g_File.isExist(path)) {
            s_model.unkfunc_020587d4(path, index);
            index++;
        }
        dss::sprintf_s(path, sizeof(path), "data/chr/%s_%d.nsbca", "msdr", i);
        if (dss::g_File.isExist(path)) {
            s_model.unkfunc_020587d4(path, index);
            index++;
        }
    }
    s_model.startAnimation(1, 1);
    dss::Fix32Vector3 pos;
    pos.vx.value = -0x1666;
    pos.vy.value = 0xe99a;
    pos.vz.value = 0xc59a;
    s_model.setPosition(pos);
    dss::Vector3<short> rot;
    rot.vx = 0x1b00;
    rot.vy = 0x6000;
    rot.vz = 0x800;
    s_model.setRotationIdx(rot);
}

ARM static void unkfunc_02009b90()
{
    s_model.cleanup(1);
}

ARM void LogoPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    s_render.unkfunc_02084efc();
    s_stage.setRender(&s_render);
    s_stage.setPath(s_mapPath);
    s_stage.load("ev01");
    s_stage.setup();
    s_sprite.unkfunc_02057d60("data/2d/opening.tex", 0);
    s_sprite.unkfunc_02057e58(&s_render);
    s_sprite.unkfunc_02057edc();
    s_camera.unk_004.setup();
    s_camera.unk_068.setup();
    s_camera.m_dirOffset = 0x5b0;
    s_camera.setTarget(s_target, 0);
    s_camera.setDistance(s_distance);
    s_camera.setRotXYZ(s_angle);
    s_camera.setOffset(s_offset);
    s_camera.m_cameraNo = 1;
    s_camera.unk_d0 = 0;
    s_camera.unk_d4 = 0;
    int fov = s_fov;
    s_camera.unk_004.setFOV2(fov);
    s_camera.unk_068.setFOV2(fov);
    unkfunc_020099f0();
    unkfunc_02009f10(0);
    Sound::unkfunc_02055980(0);
    g_Global.fadeIn(30);
}

ARM void LogoPart::terminate()
{
    unkfunc_02009b90();
    s_sprite.unkfunc_02057e34();
    s_stage.cleanup();
    g_Global.setMapName("caf1");
    status::g_Story.setChapter(1);
    Sound::unkfunc_02055998(0);
    s_render.unkfunc_02084f50();
}

ARM void LogoPart::onExecutePart()
{
    s_camera.applyCamera();
    switch (state_) {
    case 0:
        if (frame_ == 120) {
            unkfunc_02009f10(1);
        }
        break;
    case 1:
        if (dss::g_Pad.edge() & 0xcf3) {
            unkfunc_02009f10(2);
            Sound::unkfunc_02055998(15);
            g_Global.fadeOutBlack(30);
        }
        break;
    case 2:
        if (frame_ == 30) {
            g_Global.startGame();
            unkfunc_02009f10(3);
        }
        break;
    }
    frame_++;
}

ARM void LogoPart::onDrawPart()
{
    s_render.unkfunc_02084fa4();
    if (s_frame % 2) {
        s_model.pause(1);
    } else {
        s_model.pause(0);
    }
    s_model.draw();
    s_frame++;
}

ARM void LogoPart::onWindowPart()
{
}

ARM void LogoPart::onDebugPart()
{
}

ARM void LogoPart::unkfunc_02009f10(int state)
{
    state_ = state;
    frame_ = 0;
}
