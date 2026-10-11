#pragma ipa file
#include "ov000/town/TownDamageFloor.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/sound/SoundManager.hpp"

static const dss::Fix32 DAMAGE_DISTANCE(0.5f);

ARM TownDamageFloor* TownDamageFloor::getSingleton()
{
    static TownDamageFloor townDamageFloor;
    return &townDamageFloor;
}

ARM void TownDamageFloor::setup()
{
    topStride_ = 16;
    partyStride_ = 8;
    CommonWalkDamage::setup();
}

ARM int TownDamageFloor::checkBarrier()
{
    return TownStageManager::getSingleton()->getHitSurfaceIdByType(3) != -1;
}

ARM int TownDamageFloor::checkPoison()
{
    return TownStageManager::getSingleton()->getHitSurfaceIdByType(2) != -1;
}

ARM void TownDamageFloor::setPartyMemberColor(int index, int type)
{
    int rgb = TownPlayerManager::getSingleton()->getDamageColor(type);
    status::g_Party.setDisplayMode();
    int count = status::g_Party.getCount();
    int temp = -1;
    for (int i = 0; i < count; i++) {
        if (index == status::g_Party.getPlayerIndex(i)) {
            temp = i;
            break;
        }
    }
    if (g_Stage.isBashaEnter() && status::g_Party.basha_ != 0) {
        if (temp > 0) {
            temp += 2;
        }
    }
    TownPlayerManager::getSingleton()->partyDraw_.partyCharacter_[temp].setColor(rgb);
    if (isPlaySe() == true) {
        nextSe_ = 0;
        if (type == 0) {
            SoundManager::playSe(0x13c, 0);
        } else if (type == 1) {
            SoundManager::playSe(0x13b, 0);
        }
        seCounter_ = 0;
    } else {
        setNextSe(type);
    }
    status::g_Party.setBattleMode();
}

ARM void TownDamageFloor::checkDamageFloor(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos)
{
    checkWalk(nowPos, nextPos);
}
