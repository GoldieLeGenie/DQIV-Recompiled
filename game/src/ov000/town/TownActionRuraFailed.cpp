#pragma ipa file
#include "ov000/town/TownActionRuraFailed.hpp"
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/text/TextAPI.hpp"

ARM int TownActionRuraFailed::setup()
{
    return -1;
}

ARM void TownActionRuraFailed::execute()
{
    position_ = tempPos_;
    switch (mode_) {
    case FAILED_RURA_UP1:
    case FAILED_RURA_UP2:
    case FAILED_RURA_DOWN1:
    case FAILED_RURA_DOWN2:
        gMoveToTarget.execute();
        break;
    case FAILED_RURA_TOP:
        counter_++;
        break;
    }
}

ARM int TownActionRuraFailed::update()
{
    static const dss::Fix32 dy(0x3000);
    dss::Fix32Vector3 pos;
    if (gMoveToTarget.update() != -1) {
        switch (mode_) {
        case FAILED_RURA_UP1:
            mode_ = FAILED_RURA_UP2;
            pos = tempPos_;
            pos.vy += dy;
            gMoveToTarget.setAction(tempPos_, pos, TownPlayerAction::ruraSpeed, 0, 0, 11);
            break;
        case FAILED_RURA_UP2:
            TownCamera::getSingleton()->setShake(2, 6);
            TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
            counter_ = 0;
            mode_ = FAILED_RURA_TOP;
            SoundManager::stopSeWithIndex(0x23b, 0);
            SoundManager::playSe(0x14c, 0);
            break;
        case FAILED_RURA_TOP:
            if (counter_ > 60) {
                mode_ = FAILED_RURA_DOWN1;
                pos = startPos_;
                pos.vy.value += 0x1000;
                gMoveToTarget.setAction(tempPos_, pos, TownPlayerAction::walkSpeed, 0, 0, 11);
            }
            break;
        case FAILED_RURA_DOWN1:
            switch (prevAction_) {
            case ACTION_TYPE_SHIP:
                gMoveToTarget.setAction(tempPos_, startPos_, TownPlayerAction::walkSpeed, 1, 2, 11);
                break;
            case ACTION_TYPE_IKADA:
                gMoveToTarget.setAction(tempPos_, startPos_, TownPlayerAction::walkSpeed, 1, 1, 11);
                break;
            default:
                gMoveToTarget.setAction(tempPos_, startPos_, TownPlayerAction::walkSpeed, 1, 0, 11);
                break;
            }
            mode_ = FAILED_RURA_DOWN2;
            break;
        case FAILED_RURA_DOWN2:
            TextAPI::setMACRO0(1, 0x50000000, g_cmnPartyInfo.actorIndex_);
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openCommonMessage();
            TownWindowSystem::getSingleton()->addCommonMessage(0xc3cf5);
            TownPlayerManager::getSingleton()->setRemote(0);
            TownPlayerManager::getSingleton()->shadowSet_ = 0;
            TownCamera::getSingleton()->setCameraLock(false);
            TownPlayerManager::getSingleton()->effectPosFlag_ = 0;
            TownSystem::getSingleton()->scriptLock_ = 0;
            TownCharacterManager::getSingleton()->setAllMotionLock(0);
            return prevAction_;
        }
    }
    dirIdx_ = prev_dirIdx_;
    TownPlayerManager::getSingleton()->setEffectPos(position_);
    tempPos_ = position_;
    position_ = startPos_;
    return -1;
}

ARM int TownActionRuraFailed::startCheck()
{
    prev_dirIdx_ = dirIdx_;
    dss::Fix32Vector3 target = position_;
    startPos_ = position_;
    tempPos_ = position_;
    target.vy.value += 0x1000;
    gMoveToTarget.setAction(position_, target, TownPlayerAction::ruraSpeed, 0, 0, 11);
    TownCamera::getSingleton()->setCameraLock(true);
    TownPlayerManager::getSingleton()->setRemote(1);
    TownPlayerManager::getSingleton()->shadowSet_ = 1;
    if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
        TownPlayerManager::getSingleton()->setPartyToFirst(position_);
    }
    dirIdx_ = prev_dirIdx_;
    mode_ = FAILED_RURA_UP1;
    prevAction_ = TownPlayerManager::getSingleton()->player_.actionType_;
    SoundManager::playSe(0x23b, 0);
    TownPlayerManager::getSingleton()->setEffectPos(position_);
    TownPlayerManager::getSingleton()->effectPosFlag_ = 1;
    return ACTION_TYPE_RURA_FAILED;
}

ARM TownActionRuraFailed* TownActionRuraFailed::getSingleton()
{
    static TownActionRuraFailed townActionRuraFailed;
    return &townActionRuraFailed;
}
