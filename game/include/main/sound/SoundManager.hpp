#pragma once
#include "nitro/os.hpp"
#include <globaldefs.h>
#include "main/sound/Sound.hpp"

namespace param {
    struct FloorParam;
}


struct SoundManager {
    SoundManager();
    ~SoundManager();
    static void initialize();
    static void update();
    static void play(int bgm, int param);
    static void playStart(int bgm, int param);
    static void stop(int fade);
    static void playRestart(int bgm, int param);
    static void townPlay();
    static void setTownPlayDisable();
    static void setTownPlayEnable();
    static void fieldPlay();
    static void battlePlay();
    static void lastBossPlay();
    static void resetLastBossPlay();
    static void crusingPlay();
    static void battleStop();
    static void playBgm(int bgm, int param);
    static void stopBgm(int fade);
    static int playSe(int seId, int param);
    static void stopSeWithIndex(int seId, int index);
    static int getFloorBgmIndex();

    static int finalFormBGM_;               //data_020ed268 set when the final boss reaches its last form (bgm 0x21)
    static param::FloorParam * g_floor_param; //data_020ed26c
    static int bgmIndex_;       //data_020ed270
    static int nextBgmIndex_;   //data_020ed274
    static int prevBgmIndex_;   //data_020ed278
    static int nextBgmParam_;   //data_020ed27c
    static int townBgmEnable_;  //data_020be6f4
};
