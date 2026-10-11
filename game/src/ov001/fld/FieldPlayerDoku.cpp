#pragma ipa file
#include "ov001/fld/FieldPlayerDoku.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldStage.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/sound/Sound.hpp"

ARM FieldPlayerDoku* FieldPlayerDoku::getSingleton()
{
    static FieldPlayerDoku fieldPlayerDoku;
    return &fieldPlayerDoku;
}

ARM void FieldPlayerDoku::setup()
{
    CommonWalkDamage::setup();
    topStride_ = 16;
    partyStride_ = 13;
}

ARM int FieldPlayerDoku::checkBarrier()
{
    return false;
}

ARM int FieldPlayerDoku::checkPoison()
{
    return blockAttr_ == 10;
}

ARM void FieldPlayerDoku::setPartyMemberColor(int index, int type)
{
    int color = FieldPlayerManager::getSingleton()->getDamageColor(type);
    status::g_Party.setDisplayMode();
    int count = status::g_Party.getCount();
    int temp = 0;
    for (int i = 0; i < count; i++) {
        if (index == status::g_Party.getPlayerIndex(i)) {
            temp = i;
            break;
        }
    }
    if (status::g_Party.basha_ == 1 && temp > 0) {
        temp += 2;
    }
    FieldPlayerManager::getSingleton()->partyDraw_.partyCharacter_[temp].setColor(color);
    if (isPlaySe() == true) {
        nextSe_ = 0;
        if (type == 1) {
            Sound::sePlayDirect(0x13b);
        }
        seCounter_ = 0;
    } else {
        setNextSe(type);
    }
    status::g_Party.isBattleModeEnabled();
}

ARM void FieldPlayerDoku::checkDokuDamage(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos)
{
    checkWalk(nowPos, nextPos);
    if (nowPos != nextPos) {
        fld::FieldStage::getSingleton()->ChangeTime(0);
    }
}

ARM void FieldPlayerDoku::setBlockAttr(int attr)
{
    blockAttr_ = attr;
}
