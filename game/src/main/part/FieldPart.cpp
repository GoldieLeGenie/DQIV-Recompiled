#include <nitro/native_g3.h>
#include "main/part/FieldPart.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/debug/UnkDebugDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuManager.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"
#include "nitro/os.hpp"
#include "nnsys/g3d.hpp"
#include <nitro/os/interrupt.h>
#include "ov001/fld/FieldSystem.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/dss/UnkOverlaySlot.hpp"

FieldPart g_FieldPart;

ARM void FieldPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    unkfunc_02084dd4(0);
    unkfunc_02087590(DQIV_OVERLAY_ID(1));
    ov001_entry();
    unkfunc_02087590(DQIV_OVERLAY_ID(16));
    ov016_entry();
    FieldSystem::getSingleton()->initialize();
    FieldWindowSystem::getSingleton()->initialize();
    g_Global.fadeIn(30);
    unkfunc_0202c25c();
}

ARM void FieldPart::terminate()
{
    FieldWindowSystem::getSingleton()->terminate();
    FieldSystem::getSingleton()->terminate();
    unkfunc_020875a4(DQIV_OVERLAY_ID(1));
    unkfunc_020875a4(DQIV_OVERLAY_ID(16));
    dss::g_Pad.unkfunc_0207f2b4(0);
    data_0211e450.unkfunc_02086278();
}

ARM void FieldPart::onExecutePart()
{
    FieldSystem::getSingleton()->execute();
    FieldWindowSystem::getSingleton()->execute();
}

ARM void FieldPart::onDrawPart()
{
    FieldSystem::getSingleton()->draw();
}

ARM void FieldPart::onWindowPart()
{
}

ARM void FieldPart::onDebugPart()
{
    unkfunc_0202c284();
}

ARM void FieldPart::onSwapBuffersPart()
{
    BOOL enable = OS_DisableIME();
    G3_WRITE(0x50, 1);
    OS_RestoreIME(enable);
}
