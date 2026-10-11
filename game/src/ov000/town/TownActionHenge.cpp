#pragma ipa file
#include "ov000/town/TownActionHenge.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/sound/SoundManager.hpp"

ARM int TownActionHenge::setup()
{
    counter_ = 0;
    return -1;
}

ARM void TownActionHenge::execute()
{
    if (counter_ % 4 < 2) {
        TownPlayerManager::getSingleton()->partyDraw_.setPlayerAlpha(0);
    } else {
        TownPlayerManager::getSingleton()->partyDraw_.setPlayerAlpha(31);
    }
}

ARM int TownActionHenge::update()
{
    if (counter_ >= 60) {
        int nextAction = g_HengeNoTsue.getNextAction();
        TownPlayerManager::getSingleton()->player_.actionType_ = (TOWN_PLAYER_ACTION_TYPE)nextAction;
        TownPlayerManager::getSingleton()->resetParty();
        TownPlayerManager::getSingleton()->setRemote(0);
        TownCharacterManager::getSingleton()->setAllMotionLock(0);
        TownSystem::getSingleton()->scriptLock_ = 0;
        return nextAction;
    }
    counter_++;
    return -1;
}

ARM TownActionHenge* TownActionHenge::getSingleton()
{
    static TownActionHenge townActionHenge;
    return &townActionHenge;
}

ARM void TownActionHenge::setChangeAction()
{
    counter_ = 0;
    changeCharaNo = g_HengeNoTsue.getChangeCharaNo();
    g_HengeNoTsue.setCounter();
    mode_ = CHANGE;
    TownPlayerManager::getSingleton()->setRemote(1);
    TownCharacterManager::getSingleton()->setAllMotionLock(1);
    SoundManager::playSe(0x169, 0);
    TownSystem::getSingleton()->scriptLock_ = 1;
}

ARM void TownActionHenge::resetParty()
{
    SoundManager::playSe(0x169, 0);
    counter_ = 0;
    mode_ = RESET;
    TownPlayerManager::getSingleton()->setRemote(1);
    TownCharacterManager::getSingleton()->setAllMotionLock(1);
    TownSystem::getSingleton()->scriptLock_ = 1;
}
