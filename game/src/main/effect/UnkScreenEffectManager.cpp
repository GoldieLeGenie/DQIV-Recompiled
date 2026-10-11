#include "main/effect/UnkScreenEffectManager.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/sound/SoundManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"

int UnkScreenEffectManager::unk_020edc40;

THUMB void UnkScreenEffectManager::unkfunc_0202ace4()
{
    type_ = 0;
    current_ = NULL;
    if (unk_020edc40 != 0) {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(1);
        dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(0, 0x10);
        g_Global.setRanarutaFlag(true);
        cmn::NonBattleActionManager::getSingleton()->setAction(cmn::ACTION_TRAVELDOOR);
    }
}

THUMB void UnkScreenEffectManager::unkfunc_0202ad28()
{
    if (current_ != NULL) {
        if (effect3_.unk_20 != 0) {
            if (!current_->flag_.check(2)) {
                g_Global.fadeOutBlack(1);
            } else if (fadeIn_ == 0) {
                g_Global.fadeInBlack(60);
            }
            fadeIn_ = current_->flag_.check(2);
        }
        current_->unkfunc_0202b498();
        current_->isEnd();
    }
}

THUMB void UnkScreenEffectManager::unkfunc_0202ad98()
{
    if (type_ == 1) {
        unkfunc_0202afcc();
    } else if (current_ != NULL) {
        current_->draw();
    }
}

THUMB void UnkScreenEffectManager::unkfunc_0202adb4()
{
    if (current_ != NULL) {
        current_->unkfunc_0202b474();
    }
}

THUMB UnkScreenEffectManager* UnkScreenEffectManager::getSingleton()
{
    static UnkScreenEffectManager m_singleton;
    return &m_singleton;
}

THUMB void UnkScreenEffectManager::unkfunc_0202aea4()
{
    if (current_ != NULL) {
        current_->unkfunc_0202b474();
        current_ = NULL;
        dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(0);
    }
}

THUMB void UnkScreenEffectManager::unkfunc_0202aec4(int type)
{
    type_ = type;
    switch (type) {
        case 1:
            unkfunc_0202af70();
            break;
        case 3:
            current_ = &effect3_;
            current_->start();
            if (unk_020edc40 != 0) {
                effect3_.unkfunc_02030278(1);
                unk_020edc40 = 0;
            }
            break;
        case 2:
            current_ = &effect2_;
            current_->start();
            break;
        case 5:
            current_ = &effect5_;
            current_->start();
            break;
        case 4:
            current_ = &effect4_;
            current_->start();
            break;
        case 6:
            current_ = &effect6_;
            current_->start();
            break;
        case 7:
            break;
    }
}

THUMB bool UnkScreenEffectManager::unkfunc_0202af54()
{
    if (current_ != NULL) {
        return current_->flag_.check(2);
    }
    return false;
}

THUMB void UnkScreenEffectManager::unkfunc_0202af70()
{
    step_ = 0;
    data_020f21f8.state_ = GlobalFade::FADE_IN_BLACK;
    data_020f21f8.count_ = 0;
    data_020f21f8.frames_ = 30;
    data_020f21f8.sprite_[0].setColor(0x1f, 0x1f, 0x1f);
    data_020f21f8.sprite_[1].setColor(0x1f, 0x1f, 0x1f);
    data_0210bc18.unkfunc_02058294(&data_020f21f8);
    TownPlayerManager::getSingleton()->setLock(1);
    SoundManager::playSe(0x469, 0);
}

THUMB void UnkScreenEffectManager::unkfunc_0202afcc()
{
    if (data_020f21f8.count_ == data_020f21f8.frames_) {
        if (step_ == 0) {
            switch (data_020f21f8.state_) {
                case GlobalFade::FADE_NONE:
                    data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
                    break;
                case GlobalFade::FADE_IN_BLACK:
                    data_020f21f8.state_ = GlobalFade::FADE_OUT_WHITE;
                    break;
                default:
                    data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
                    break;
            }
            data_020f21f8.count_ = 0;
            data_020f21f8.frames_ = 30;
            data_0210bc18.unkfunc_02058294(&data_020f21f8);
            step_ = 1;
        } else {
            SoundManager::playSe(0x1f5, 0);
            TownPlayerManager::getSingleton()->setLock(0);
            type_ = 0;
        }
    }
}
