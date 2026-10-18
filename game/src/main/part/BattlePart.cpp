#include "main/part/BattlePart.hpp"
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
#include "main/encount/Encount.hpp"
#include "ov003/btl/BattleSystem2.hpp"
#include "ov003/btl/BattleWindowSystem.hpp"
#include "main/menu/MaterielMenuOverlay.hpp"

BattlePart g_BattlePart;

ARM void BattlePart::initialize()
{
    func_020648cc();
    func_020648b8();
    GX_ResetBankForTex();
    GX_ResetBankForSubObj();
    GX_ResetBankForSubBg();
    GX_SetBankForTex(GX_VRAM_ABCD);
    g_VramTransfer.initVramManagers(0x80000, 0x4000);
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_SINGLE3D);
    loadOverlay(DQIV_OVERLAY_ID(3));
    ov003_entry();
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        loadOverlay(DQIV_OVERLAY_ID(15));
        ov015_entry();
    } else {
        loadOverlay(DQIV_OVERLAY_ID(16));
    }
    g_MaterielOverlaySlot.unkfunc_02087564();
    btl::BattleSystem2::getSingleton()->initialize();
    BattleWindowSystem::getSingleton()->initialize();
    func_0206dd70(1);
    g_Global.fadeIn(30);
    dss::g_Pad.setMaskL(0);
    initDebugDisplay();
}

ARM void BattlePart::terminate()
{
    dss::g_Pad.setMaskL(0);
    BattleWindowSystem::getSingleton()->terminate();
    btl::BattleSystem2::getSingleton()->terminate();
    unloadOverlay(DQIV_OVERLAY_ID(3));
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        unloadOverlay(DQIV_OVERLAY_ID(15));
    } else {
        unloadOverlay(DQIV_OVERLAY_ID(16));
    }
    encount::Encount::getSingleton()->battleMode_ = encount::Encount::Normal;
    g_VramTransfer.unkfunc_02086278();
}

ARM void BattlePart::onExecutePart()
{
    btl::BattleSystem2::getSingleton()->execute();
}

ARM void BattlePart::onDrawPart()
{
    btl::BattleSystem2::getSingleton()->draw();
}

ARM void BattlePart::onWindowPart()
{
    BattleWindowSystem::getSingleton()->execute();
    BattleWindowSystem::getSingleton()->draw();
}

ARM void BattlePart::onDebugPart()
{
    updateDebugDisplay();
}
