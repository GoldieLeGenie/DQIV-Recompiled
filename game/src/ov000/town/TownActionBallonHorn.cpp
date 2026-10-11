#pragma ipa file
#include "ov000/town/TownActionBallonHorn.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/sound/SoundManager.hpp"

ARM int TownActionBallonHorn::setup()
{
    ballonEnable_ = 0;
    counter_ = 0;
    type_ = 0;
    return -1;
}

ARM void TownActionBallonHorn::execute()
{
}

ARM int TownActionBallonHorn::update()
{
    if (counter_ == 360) {
        if (g_cmnPartyInfo.isBarronArea(&position_) && !g_cmnPartyInfo.isUsedBarron()) {
            ballonEnable_ = 1;
            g_Global.fadeOutWhite(20);
            type_ = 1;
        } else {
            TownPlayerManager::getSingleton()->setRemote(0);
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openCommonMessage();
            TownWindowSystem::getSingleton()->addCommonMessage(0xc3d6d);
            SoundManager::townPlay();
            return prevAction_;
        }
    } else if (counter_ > 360) {
        if (g_GlobalFade.isFadeEnd() == 1) {
            if (type_ == 1) {
                type_ = 2;
                g_Global.fadeInWhite(20);
                g_cmnPartyInfo.callCarriage();
            } else if (type_ == 2) {
                SoundManager::townPlay();
                TownPlayerManager::getSingleton()->setRemote(0);
                return prevAction_;
            }
        }
    }
    counter_++;
    return -1;
}

ARM TownActionBallonHorn* TownActionBallonHorn::getSingleton()
{
    static TownActionBallonHorn townActionBallonHorn;
    return &townActionBallonHorn;
}

ARM void TownActionBallonHorn::startAction(int action)
{
    prevAction_ = action;
    counter_ = 0;
    type_ = 0;
    ballonEnable_ = 0;
    SoundManager::play(0x32, 0xf);
}
