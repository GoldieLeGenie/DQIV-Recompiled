#include "main/part/BookPart.hpp"
#include "main/dss/Display.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuManager.hpp"
#include "ov006/BookSystem.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"
#include "nitro/os.hpp"
#include "nnsys/g3d.hpp"
#include "main/dss/OverlaySlot.hpp"

BookPart g_BookPart;

ARM void BookPart::initialize()
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
    loadOverlay(DQIV_OVERLAY_ID(6));
    BookSystem::onOverlayLoad();
    loadOverlay(DQIV_OVERLAY_ID(16));
    ov016_entry();
    BookSystem::getSingleton()->initialize();
    func_0206dd70(1);
    OS_Wait();
    g_Global.fadeIn(30);
}

ARM void BookPart::terminate()
{
    dss::g_Pad.setMaskL(0);
    BookSystem::getSingleton()->terminate();
    unloadOverlay(DQIV_OVERLAY_ID(6));
    unloadOverlay(DQIV_OVERLAY_ID(16));
    g_VramTransfer.unkfunc_02086278();
}

ARM void BookPart::onExecutePart()
{
    BookSystem::getSingleton()->execute();
}

ARM void BookPart::onDrawPart()
{
    BookSystem::getSingleton()->draw();
}

ARM void BookPart::onWindowPart()
{
}

ARM void BookPart::onDebugPart()
{
}
