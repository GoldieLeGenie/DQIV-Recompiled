#include "ov003/btl/BattleEffectManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/object/ModelObject.hpp"
#include "ov003/btl/BattleCamera.hpp"

#pragma profile on

THUMB btl::BattleEffectManager::BattleEffectManager()
{
}

THUMB btl::BattleEffectManager::~BattleEffectManager()
{
}

THUMB btl::BattleEffectManager* btl::BattleEffectManager::getSingleton()
{
    static BattleEffectManager m_singleton;
    return &m_singleton;
}

THUMB void btl::BattleEffectManager::initialize()
{
    storage_.initialize();
    resource_.initialize();
    setCameraPos();
    effectParam_ = status::excelParam.effect_;
    BattleEffectUnit::setControlData(&storage_, &resource_);
    for (int i = 0; i < 8; i++) {
        unit_[i].initialize();
    }
    wait_ = 0;
}

THUMB void btl::BattleEffectManager::terminate()
{
    for (int i = 0; i < 8; i++) {
        unit_[i].terminate();
    }
    storage_.terminate();
    resource_.terminate();
}

THUMB void btl::BattleEffectManager::execute()
{
    for (int i = 0; i < 8; i++) {
        unit_[i].execute();
    }
    if (wait_ > 0) {
        wait_--;
    }
}

THUMB void btl::BattleEffectManager::extraDraw()
{
    DSSAObject::calcType_ = 1;
    for (int i = 0; i < 8; i++) {
        unit_[i].extraDraw();
    }
    DSSAObject::calcType_ = 0;
}

THUMB void btl::BattleEffectManager::draw()
{
    DSSAObject::calcType_ = 1;
    for (int i = 0; i < 8; i++) {
        unit_[i].draw();
    }
    DSSAObject::calcType_ = 0;
}

THUMB void btl::BattleEffectManager::setCameraPos()
{
    ModelObjectWithCamera::camera_ = BattleCamera::getSingleton()->getCamera();
}

THUMB int btl::BattleEffectManager::isEnd()
{
    for (int i = 0; i < 8; i++) {
        if (unit_[i].isEnable()) {
            return 0;
        }
    }
    return 1;
}

THUMB int btl::BattleEffectManager::isAllEnd()
{
    return storage_.effectCounter_ == 0;
}

THUMB int btl::BattleEffectManager::getWeaponEffectID(status::PlayerStatus* player)
{
    int weapon = player->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON);
    if (weapon == 0) {
        weapon = 0x2b;
    }
    return weapon + 300;
}

THUMB param::EffectParam* btl::BattleEffectManager::getEffectParam(int id)
{
    for (unsigned int i = 0; i < param::EffectParam::size_; i++) {
        if (effectParam_[i].index == id) {
            return &effectParam_[i];
        }
    }
    return 0;
}

THUMB int btl::BattleEffectManager::setupEffect(int id)
{
    int index;
    BattleEffectUnit* unit;

    setCameraPos();
    unit = 0;
    for (int i = 0; i < 8; i++) {
        if (!unit_[i].isEnable()) {
            index = i;
            unit = &unit_[i];
            break;
        }
    }
    unit->setup(getEffectParam(id));
    if (!unit->isEnable()) {
        index = -1;
    }
    return index;
}

THUMB int btl::BattleEffectManager::isEndWait()
{
    return wait_ == 0;
}
