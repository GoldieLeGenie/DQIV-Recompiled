#pragma ipa file
#include "ov003/btl/BattleWindowSystem.hpp"

THUMB BattleWindowSystem::BattleWindowSystem()
{
    static int unk;
}

THUMB BattleWindowSystem* BattleWindowSystem::getSingleton()
{
    static BattleWindowSystem m_singleton;
    return &m_singleton;
}

THUMB void BattleWindowSystem::initialize()
{
}

THUMB void BattleWindowSystem::terminate()
{
}

THUMB void BattleWindowSystem::execute()
{
}

THUMB void BattleWindowSystem::draw()
{
}
