#include "main/part/TownPart.hpp"
#include "main/dss/Display.hpp"
#include "main/debug/DebugDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/VramManager.hpp"
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
    g_VramTransfer.initVramManagers(0x40000, 0x4000);
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    getTick();
    loadOverlay(DQIV_OVERLAY_ID(0));
    TownSystem::overlayEntry();
    loadOverlay(DQIV_OVERLAY_ID(16));
    ov016_entry();
    getTick();
    TownSystem::getSingleton()->initialize();
    TownWindowSystem::getSingleton()->initialize();
    func_0206dd70(1);
    OS_Wait();
    g_Global.fadeIn(30);
    if (getAppHeapTotalFreeSize(&g_appHeap) < s_minFreeSize) {
        s_minFreeSize = getAppHeapTotalFreeSize(&g_appHeap);
    }
    initDebugDisplay();
}

ARM void TownPart::terminate()
{
    TownWindowSystem::getSingleton()->terminate();
    TownSystem::getSingleton()->terminate();
    getTick();
    unloadOverlay(DQIV_OVERLAY_ID(0));
    unloadOverlay(DQIV_OVERLAY_ID(16));
    g_MaterielOverlaySlot.unkfunc_02087564();
    g_VramTransfer.unkfunc_02086278();
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
    updateDebugDisplay();
}
