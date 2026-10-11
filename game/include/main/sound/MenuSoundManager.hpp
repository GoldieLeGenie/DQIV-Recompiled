#pragma once
#include <globaldefs.h>
#include "main/sound/SoundManager.hpp"

struct MenuSoundManager : SoundManager {
    enum MENU_SOUND {
        MENU_SOUND_NONE = 0,
        MENU_SOUND_NOROI = 1,
        MENU_SOUND_SAVE = 2,
        MENU_SOUND_MIRACLE = 3,
        MENU_SOUND_INN = 4,
        MENU_SOUND_AYAKASHI = 5,
        MENU_SOUND_BARON = 6,
        MENU_SOUND_FANFARE_S = 7,
        MENU_SOUND_FANFARE_M = 8,
        MENU_SOUND_FANFARE_L = 9,
        MENU_SOUND_CHAPTER_1 = 10,
        MENU_SOUND_CHAPTER_2 = 11,
        MENU_SOUND_CHAPTER_3 = 12,
        MENU_SOUND_CHAPTER_4 = 13,
        MENU_SOUND_LEVEL_UP = 14,
        MENU_SOUND_BATTLE_INN = 15
    };

    int soundType_;         /* 0x00 */
    int playSoundNo_;       /* 0x04 */
    int waitSoundTime_;     /* 0x08 */
    int playTime_;          /* 0x0C */
    int soundCount_;        /* 0x10 */
    int preSoundIndex_;     /* 0x14 */

    static MenuSoundManager* getSingleton();
    void initialize();
    void setPlaySound(MENU_SOUND sound);
    int isPlaySound();
};
