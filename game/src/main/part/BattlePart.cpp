#include "main/part/BattlePart.hpp"
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
#include "main/encount/Encount.hpp"
#include "ov003/btl/BattleSystem2.hpp"
#include "ov003/btl/UnkBattleSystem.hpp"
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
    data_0211e450.unkfunc_020861c4(0x80000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_SINGLE3D);
    unkfunc_02087590(DQIV_OVERLAY_ID(3));
    ov003_entry();
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        unkfunc_02087590(DQIV_OVERLAY_ID(15));
        ov015_entry();
    } else {
        unkfunc_02087590(DQIV_OVERLAY_ID(16));
    }
    data_020efc58.unkfunc_02087564();
    btl::BattleSystem2::getSingleton()->initialize();
    UnkBattleSystem::getSingleton()->initialize();
    func_0206dd70(1);
    g_Global.fadeIn(30);
    dss::g_Pad.unkfunc_0207f2b4(0);
    unkfunc_0202c25c();
}

ARM void BattlePart::terminate()
{
    dss::g_Pad.unkfunc_0207f2b4(0);
    UnkBattleSystem::getSingleton()->terminate();
    btl::BattleSystem2::getSingleton()->terminate();
    unkfunc_020875a4(DQIV_OVERLAY_ID(3));
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        unkfunc_020875a4(DQIV_OVERLAY_ID(15));
    } else {
        unkfunc_020875a4(DQIV_OVERLAY_ID(16));
    }
    encount::Encount::getSingleton()->battleMode_ = encount::Encount::Normal;
    data_0211e450.unkfunc_02086278();
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
    UnkBattleSystem::getSingleton()->execute();
    UnkBattleSystem::getSingleton()->draw();
}

ARM void BattlePart::onDebugPart()
{
    unkfunc_0202c284();
}
