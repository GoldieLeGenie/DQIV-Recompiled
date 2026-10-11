#include "main/sound/MenuSoundManager.hpp"

THUMB MenuSoundManager* MenuSoundManager::getSingleton()
{
    static MenuSoundManager menuSoundManager;
    return &menuSoundManager;
}

THUMB void MenuSoundManager::initialize()
{
    soundType_ = MENU_SOUND_NONE;
    playSoundNo_ = 0;
    waitSoundTime_ = 0;
    playTime_ = 0;
    preSoundIndex_ = 0;
    soundCount_ = -1;
}

THUMB void MenuSoundManager::setPlaySound(MENU_SOUND sound)
{
    soundType_ = sound;
    soundCount_ = 0;
    waitSoundTime_ = 0x1e;
    preSoundIndex_ = SoundManager::bgmIndex_;
    switch (soundType_) {
        case MENU_SOUND_NOROI:
            playSoundNo_ = 0x2b;
            playTime_ = 0xc8;
            break;
        case MENU_SOUND_SAVE:
            playSoundNo_ = 0x27;
            playTime_ = 0x1a4;
            break;
        case MENU_SOUND_MIRACLE:
            playSoundNo_ = 0x28;
            playTime_ = 0x1e0;
            break;
        case MENU_SOUND_INN:
        case MENU_SOUND_BATTLE_INN:
            waitSoundTime_ = 0x3c;
            playSoundNo_ = 0x29;
            playTime_ = 0xf0;
            break;
        case MENU_SOUND_AYAKASHI:
            playSoundNo_ = 0x23;
            playTime_ = 0x2d0;
            break;
        case MENU_SOUND_BARON:
            playSoundNo_ = 0x23;
            playTime_ = 0x2d0;
            break;
        case MENU_SOUND_FANFARE_S:
            playSoundNo_ = 0x2c;
            playTime_ = 0xc8;
            break;
        case MENU_SOUND_FANFARE_M:
            playSoundNo_ = 0x2d;
            playTime_ = 0x212;
            break;
        case MENU_SOUND_FANFARE_L:
            playSoundNo_ = 0x2e;
            playTime_ = 0x528;
            break;
        case MENU_SOUND_CHAPTER_1:
            playSoundNo_ = 6;
            playTime_ = 0x294;
            break;
        case MENU_SOUND_CHAPTER_2:
            playSoundNo_ = 8;
            playTime_ = 0x12c;
            break;
        case MENU_SOUND_CHAPTER_3:
            playSoundNo_ = 0xa;
            playTime_ = 0x1e0;
            break;
        case MENU_SOUND_CHAPTER_4:
            playSoundNo_ = 0xd;
            playTime_ = 0x438;
            break;
        case MENU_SOUND_LEVEL_UP:
            playSoundNo_ = 0x2f;
            playTime_ = 0xb4;
            break;
    }
    SoundManager::stopBgm(0);
}

THUMB int MenuSoundManager::isPlaySound()
{
    int result = 0;
    int count = soundCount_;
    if (count == -1) {
        return result;
    }
    if (count == waitSoundTime_) {
        SoundManager::playBgm(playSoundNo_, 0);
        soundCount_++;
        result = 1;
    } else if (count < playTime_) {
        soundCount_ = count + 1;
        result = 1;
    } else if (count == playTime_) {
        SoundManager::stopBgm(0);
        soundCount_++;
        result = 1;
    } else if (count > playTime_) {
        soundCount_ = -1;
        if (soundType_ == MENU_SOUND_INN || soundType_ == MENU_SOUND_LEVEL_UP ||
            (soundType_ >= MENU_SOUND_CHAPTER_1 && soundType_ <= MENU_SOUND_CHAPTER_4)) {
            return 0;
        }
        SoundManager::play(preSoundIndex_, 0xf);
    }
    return result;
}
