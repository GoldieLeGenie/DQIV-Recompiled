#include "ov003/btl/BattleEffectStorage.hpp"

#pragma profile on
THUMB btl::BattleEffectStorage::BattleEffectStorage()
{
}

THUMB btl::BattleEffectStorage::~BattleEffectStorage()
{
}

THUMB void btl::BattleEffectStorage::initialize()
{
    effectCounter_ = 0;
}

THUMB void btl::BattleEffectStorage::terminate()
{
}

THUMB btl::BattleEffectGroup* btl::BattleEffectStorage::getContainer()
{
    effectCounter_++;
    for (unsigned int i = 0; i < 12; i++) {
        if (!group[i].isEnable()) {
            return &group[i];
        }
    }
    return 0;
}

THUMB void btl::BattleEffectStorage::restoreContainer()
{
    effectCounter_--;
}

THUMB int btl::BattleEffectStorage::getContainerStock()
{
    return 12 - effectCounter_;
}
