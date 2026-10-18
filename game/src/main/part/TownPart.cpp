#include "main/part/TownPart.hpp"
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
#include "nitro/fs.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/menu/MaterielMenuOverlay.hpp"

TownPart g_TownPart;
static int s_reloadOverlay;
static unsigned int s_minFreeSize = 0x100000;

ARM void TownPart::initialize()
{
    if (s_reloadOverlay != 0) {
        FS_LoadOverlay(0, 0);
        FS_UnloadOverlay(0, 0);
    }
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    unkfunc_0207e7e8();
    unkfunc_02087590(DQIV_OVERLAY_ID(0));
    TownSystem::overlayEntry();
    unkfunc_02087590(DQIV_OVERLAY_ID(16));
    ov016_entry();
    unkfunc_0207e7e8();
    TownSystem::getSingleton()->initialize();
    TownWindowSystem::getSingleton()->initialize();
    func_0206dd70(1);
    OS_Wait();
    g_Global.fadeIn(30);
    if (unkfunc_0207f87c(&data_0211a60c) < s_minFreeSize) {
        s_minFreeSize = unkfunc_0207f87c(&data_0211a60c);
    }
    unkfunc_0202c25c();
}

ARM void TownPart::terminate()
{
    TownWindowSystem::getSingleton()->terminate();
    TownSystem::getSingleton()->terminate();
    unkfunc_0207e7e8();
    unkfunc_020875a4(DQIV_OVERLAY_ID(0));
    unkfunc_020875a4(DQIV_OVERLAY_ID(16));
    data_020efc58.unkfunc_02087564();
    data_0211e450.unkfunc_02086278();
}

ARM void TownPart::onExecutePart()
{
    TownSystem::getSingleton()->execute();
    TownWindowSystem::getSingleton()->execute();
}

ARM void TownPart::onDrawPart()
{
    TownSystem::getSingleton()->draw();
    TownWindowSystem::getSingleton()->draw();
}

ARM void TownPart::onWindowPart()
{
}

ARM void TownPart::onDebugPart()
{
    unkfunc_0202c284();
}
