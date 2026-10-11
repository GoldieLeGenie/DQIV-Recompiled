#pragma once
#include <globaldefs.h>
#include "main/sound/UnkSoundPlayer.hpp"

struct Sound {
    static void unkfunc_0205590c(int bgm);
    static void unkfunc_0205594c(int fade);
    static int unkfunc_02055968();
    static void unkfunc_02055980(int value);
    static void unkfunc_02055998(int value);
    static int sePlay(int se);
    static void unkfunc_020559cc(int se, int index);
    static void unkfunc_020559ec(int value);
    static int sePlayDirect(int se);
    static void setBgmVolume(int volume);
    static int getBgmVolume();
    static void setBgmVolumeSys(int volume);
    static void setSeVolumeSys(int volume);
#ifdef DQIV_NATIVE
    // The volume a music is meant to be heard at (its entry in the volume table).
    static int nativeBgmVolume(int bgm);
#endif
};
