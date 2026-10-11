#include "main/part/CasinoPart.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuManager.hpp"
#include "ov009/casino/CasinoSystem.hpp"
#include "nitro/gx.h"
#include "nitro/gx.hpp"
#include "nitro/os.hpp"
#include "nnsys/g3d.hpp"
#include "main/dss/UnkOverlaySlot.hpp"

CasinoPart g_CasinoPart;

ARM void CasinoPart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_AB);
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    unkfunc_0207e7e8();
    unkfunc_02087590(DQIV_OVERLAY_ID(9));
    CasinoSystem::unkfunc_021227cc();
    unkfunc_02087590(DQIV_OVERLAY_ID(16));
    ov016_entry();
    CasinoSystem::getSingleton()->initialize();
    func_0206dd70(1);
    OS_Wait();
    g_Global.fadeIn(30);
}

ARM void CasinoPart::terminate()
{
    dss::g_Pad.unkfunc_0207f2b4(0);
    CasinoSystem::getSingleton()->terminate();
    unkfunc_020875a4(DQIV_OVERLAY_ID(9));
    unkfunc_020875a4(DQIV_OVERLAY_ID(16));
    data_0211e450.unkfunc_02086278();
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
