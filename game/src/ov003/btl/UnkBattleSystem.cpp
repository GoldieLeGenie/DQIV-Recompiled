#pragma ipa file
#include "ov003/btl/UnkBattleSystem.hpp"

THUMB UnkBattleSystem::UnkBattleSystem()
{
    static int unk;
}

THUMB UnkBattleSystem* UnkBattleSystem::getSingleton()
{
    static UnkBattleSystem m_singleton;
    return &m_singleton;
}

THUMB void UnkBattleSystem::initialize()
{
}

THUMB void UnkBattleSystem::terminate()
{
}

THUMB void UnkBattleSystem::execute()
{
}

THUMB void UnkBattleSystem::draw()
{
}
