#include "main/part/CasinoPart.hpp"
#include "main/dss/Display.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuManager.hpp"
#include "ov009/casino/CasinoSystem.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"
#include "nitro/os.hpp"
#include "nnsys/g3d.hpp"
#include "main/dss/OverlaySlot.hpp"

CasinoPart g_CasinoPart;

ARM void CasinoPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    g_VramTransfer.initVramManagers(0x40000, 0x4000);
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    getTick();
    loadOverlay(DQIV_OVERLAY_ID(9));
    CasinoSystem::overlayEntry();
    loadOverlay(DQIV_OVERLAY_ID(16));
    ov016_entry();
    CasinoSystem::getSingleton()->initialize();
    func_0206dd70(1);
    OS_Wait();
    g_Global.fadeIn(30);
}

ARM void CasinoPart::terminate()
{
    dss::g_Pad.setMaskL(0);
    CasinoSystem::getSingleton()->terminate();
    unloadOverlay(DQIV_OVERLAY_ID(9));
    unloadOverlay(DQIV_OVERLAY_ID(16));
    g_VramTransfer.unkfunc_02086278();
}

ARM void CasinoPart::onExecutePart()
{
    CasinoSystem::getSingleton()->execute();
}

ARM void CasinoPart::onDrawPart()
{
    CasinoSystem::getSingleton()->draw();
}

ARM void CasinoPart::onWindowPart()
{
}

ARM void CasinoPart::onDebugPart()
{
}
