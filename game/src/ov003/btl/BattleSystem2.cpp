#include "ov003/btl/BattleSystem2.hpp"
#include "ov003/btl/BattleStage.hpp"
#include "ov003/btl/BattleCamera.hpp"
#include "ov003/btl/BattleEffectManager.hpp"
#include "ov003/btl/BattleRoot.hpp"
#include "ov003/btl/BattleActorExec.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/btl/ExecMessageTask.hpp"
#include "ov003/btl/AfterActionTask.hpp"
#include "ov003/status/ExcelParamBis.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/status/HaveAction.hpp"
#include "main/global/Global.hpp"

THUMB btl::BattleSystem2::BattleSystem2()
{
}

THUMB btl::BattleSystem2::~BattleSystem2()
{
}

THUMB btl::BattleSystem2* btl::BattleSystem2::getSingleton()
{
    static BattleSystem2 m_singleton;
    return &m_singleton;
}

THUMB void btl::BattleSystem2::initialize()
{
    dss::Fix32 scale;
    scale.value = 800;
    DSSAObject::setDefaultScale(scale);
    DSSAObject::setPriority(1);
    status::ExcelParamBis::setupBattle(&status::excelParam);
    status::ExcelParamBis::setupBattleInitialize(&status::excelParam);
    render_.unkfunc_02084efc();
    BattleStage::getSingleton()->initialize();
    BattleCamera::getSingleton()->initialize();
    BattleEffectManager::getSingleton()->initialize();
    BattleMonsterMask::getSingleton()->initialize();
    DSSAObjectWithCamera::camera_ = BattleCamera::getSingleton()->getCamera();
    BattleRoot::getSingleton()->initialize();
    
    btl::BattleMonsterDraw2::getSingleton()->setup();
    status::HaveAction::setBattleMode();
    cmn::CommonEffectLocation::getSingleton()->initialize();
    status::ExcelParamBis::cleanupBattleInitialize(&status::excelParam);
    dss::Fix32 scale2;
    scale2.value = 800;
}

THUMB void btl::BattleSystem2::terminate()
{
    cmn::CommonEffectLocation::getSingleton()->terminate();
    BattleRoot::getSingleton()->terminate();
    status::HaveAction::setTownMode();
    
    btl::BattleMonsterDraw2::getSingleton()->cleanup();
    BattleEffectManager::getSingleton()->terminate();
    BattleCamera::getSingleton()->terminate();
    BattleStage::getSingleton()->terminate();
    BattleMonsterMask::getSingleton()->terminate();
    render_.unkfunc_02084f50();
    status::ExcelParamBis::cleanupBattle(&status::excelParam);
    g_Global.partChangeFlag_ = 0;
}

THUMB void btl::BattleSystem2::execute()
{
    cmn::CommonEffectLocation::getSingleton()->execute();
    BattleRoot::getSingleton()->execute();
    BattleStage::getSingleton()->execute();
    BattleEffectManager::getSingleton()->execute();
}

THUMB void btl::BattleSystem2::draw()
{
    BattleCamera::getSingleton()->executeForMap();
    BattleCamera::getSingleton()->draw();
    DSSAObjectWithCamera::camera_ = BattleCamera::getSingleton()->getCamera();
    BattleEffectManager::getSingleton()->extraDraw();
    BattleMonsterMask::getSingleton()->draw();
    render_.unkfunc_02084fa4();
    BattleRoot::getSingleton()->draw();
    
    btl::BattleMonsterDraw2::getSingleton()->draw();
    BattleEffectManager::getSingleton()->draw();
}
