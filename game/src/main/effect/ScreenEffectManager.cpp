#include "main/effect/ScreenEffectManager.hpp"
#include "main/dss/Display.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/sound/SoundManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"

int ScreenEffectManager::unk_020edc40;

THUMB void ScreenEffectManager::initialize()
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

THUMB void ScreenEffectManager::execute()
{
    if (current_ != NULL) {
        if (effect3_.reverse_ != 0) {
            if (!current_->flag_.check(2)) {
                g_Global.fadeOutBlack(1);
            } else if (fadeIn_ == 0) {
                g_Global.fadeInBlack(60);
            }
            fadeIn_ = current_->flag_.check(2);
        }
        current_->update();
        current_->isEnd();
    }
}

THUMB void ScreenEffectManager::draw()
{
    if (type_ == 1) {
        updateWhiteFlash();
    } else if (current_ != NULL) {
        current_->draw();
    }
}

THUMB void ScreenEffectManager::terminate()
{
    if (current_ != NULL) {
        current_->terminate();
    }
}

THUMB ScreenEffectManager* ScreenEffectManager::getSingleton()
{
    static ScreenEffectManager m_singleton;
    return &m_singleton;
}

THUMB void ScreenEffectManager::stopEffect()
{
    if (current_ != NULL) {
        current_->terminate();
        current_ = NULL;
        dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(0);
    }
}

THUMB void ScreenEffectManager::startEffect(int type)
{
    type_ = type;
    switch (type) {
        case 1:
            startWhiteFlash();
            break;
        case 3:
            current_ = &effect3_;
            current_->start();
            if (unk_020edc40 != 0) {
                effect3_.setReverse(1);
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

THUMB bool ScreenEffectManager::isEffectDrawing()
{
    if (current_ != NULL) {
        return current_->flag_.check(2);
    }
    return false;
}

THUMB void ScreenEffectManager::startWhiteFlash()
{
    step_ = 0;
    g_SpriteFade.state_ = GlobalFade::FADE_IN_BLACK;
    g_SpriteFade.count_ = 0;
    g_SpriteFade.frames_ = 30;
    g_SpriteFade.sprite_[0].setColor(0x1f, 0x1f, 0x1f);
    g_SpriteFade.sprite_[1].setColor(0x1f, 0x1f, 0x1f);
    g_GlobalGamePartManager.unkfunc_02058294(&g_SpriteFade);
    TownPlayerManager::getSingleton()->setLock(1);
    SoundManager::playSe(0x469, 0);
}

THUMB void ScreenEffectManager::updateWhiteFlash()
{
    if (g_SpriteFade.count_ == g_SpriteFade.frames_) {
        if (step_ == 0) {
            switch (g_SpriteFade.state_) {
                case GlobalFade::FADE_NONE:
                    g_SpriteFade.state_ = GlobalFade::FADE_OUT_BLACK;
                    break;
                case GlobalFade::FADE_IN_BLACK:
                    g_SpriteFade.state_ = GlobalFade::FADE_OUT_WHITE;
                    break;
                default:
                    g_SpriteFade.state_ = GlobalFade::FADE_OUT_BLACK;
                    break;
            }
            g_SpriteFade.count_ = 0;
            g_SpriteFade.frames_ = 30;
            g_GlobalGamePartManager.unkfunc_02058294(&g_SpriteFade);
            step_ = 1;
        } else {
            SoundManager::playSe(0x1f5, 0);
            TownPlayerManager::getSingleton()->setLock(0);
            type_ = 0;
        }
    }
}
