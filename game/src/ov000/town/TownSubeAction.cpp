#pragma ipa file
#include "ov000/town/TownSubeAction.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/fld/FldStage.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/sound/Sound.hpp"

const dss::Fix32 TownSubeAction::subeR(0x59a);
const dss::Fix32 TownSubeAction::subeSpeed(0x19a);

ARM int TownSubeAction::setup()
{
    subeDir4_ = 0;
    prev_subeDir4_ = 0;
    vec[0] = dss::Fix32Vector3(0L, 0L, subeSpeed * -1);
    vec[1] = dss::Fix32Vector3(subeSpeed, 0L, 0L);
    vec[2] = dss::Fix32Vector3(0L, 0L, subeSpeed);
    vec[3] = dss::Fix32Vector3(subeSpeed * -1, 0L, 0L);
    count_ = 0;
    return -1;
}

ARM void TownSubeAction::execute()
{
    dss::Fix32Vector3 subePos;
    dss::Fix32Vector3 tempPos;
    dss::Fix32 height;
    tempPos = position_;
    tempPos.vy += subeR;
    TownStageManager::getSingleton()->compute(tempPos, tempPos, subeR, subeR, subeR * 2, height);
    subeDir4_ = TownStageManager::getSingleton()->getHitSurfaceIdByType(8);
    if (subeDir4_ != -1) {
        subeDir4_ = (subeDir4_ & 0xf) - 1;
        subePos = TownStageManager::getSingleton()->getHitSurfacePosByType(8);
        if (subeDir4_ == 0 || subeDir4_ == 2) {
            if (subePos.vx != position_.vx) {
                dss::Fix32 len = subePos.vx - position_.vx;
                if (unkfunc_02031e84(len.value) < subeSpeed.value) {
                    position_.vx += len;
                } else {
                    position_.vx += len.value >= 0 ? subeSpeed : subeSpeed * -1;
                }
            } else {
                position_ += vec[subeDir4_];
            }
        } else {
            if (subePos.vz != position_.vz) {
                dss::Fix32 len = subePos.vz - position_.vz;
                if (unkfunc_02031e84(len.value) < subeSpeed.value) {
                    position_.vz += len;
                } else {
                    position_.vz += len.value >= 0 ? subeSpeed : subeSpeed * -1;
                }
            } else {
                position_ += vec[subeDir4_];
            }
        }
    } else {
        if (count_ <= 3) {
            subeDir4_ = prev_subeDir4_;
            position_ += vec[subeDir4_];
        }
        count_++;
    }
    prev_subeDir4_ = subeDir4_;
    if (TownStageManager::getSingleton()->getExitIndex() != -1) {
        SoundManager::stopSeWithIndex(0x14a, 0);
    }
}

ARM int TownSubeAction::update()
{
    if (count_ == 3) {
        if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == -1 || TownStageManager::getSingleton()->getHitSurfaceIdByType(0xb) != -1) {
            TownFallAction::getSingleton()->setCollFall();
            TownFallAction::getSingleton()->count_ = 3;
            SoundManager::stopSeWithIndex(0x14a, 0);
            return ACTION_TYPE_FALL;
        }
        TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
    } else if (count_ > 3) {
        if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ == 0) {
            TownPlayerManager::getSingleton()->setRemote(0);
            TownPlayerManager::getSingleton()->partyDraw_.setAnimation(1);
            SoundManager::stopSeWithIndex(0x14a, 0);
            return ACTION_TYPE_WALK;
        }
    }
    return -1;
}

ARM int TownSubeAction::startCheck()
{
    int ret = -1;
    if ((g_cmnPartyInfo.prev_position_ == position_)) {
        return ret;
    }
    int surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(8);
    if (surfaceId != -1) {
        count_ = 0;
        subeDir4_ = 0;
        prev_subeDir4_ = 0;
        ret = ACTION_TYPE_SUBE;
        TownPlayerManager::getSingleton()->setRemote(1);
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(0);
        Sound::sePlayDirect(0x14a);
    }
    return ret;
}

ARM TownSubeAction* TownSubeAction::getSingleton()
{
    static TownSubeAction townSubeAction;
    return &townSubeAction;
}
