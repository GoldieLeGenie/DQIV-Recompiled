#include "main/sound/Sound.hpp"

static unsigned char s_bgm_volume_tbl[] = {
    0x00, 0x7f, 0x5f, 0x46, 0x46, 0x5f, 0x64, 0x64, 0x5a, 0x69, 0x6e, 0x64, 0x55, 0x5f, 0x50, 0x69,
    0x6e, 0x5f, 0x5f, 0x55, 0x69, 0x55, 0x5a, 0x5f, 0x5f, 0x7f, 0x5f, 0x69, 0x64, 0x55, 0x6e, 0x6e,
    0x7f, 0x7f, 0x7f, 0x5f, 0x5a, 0x55, 0x4b, 0x7d, 0x6e, 0x5a, 0x46, 0x50, 0x5f, 0x5f, 0x69, 0x69,
    0x4b, 0x55, 0x64, 0x5f, 0x64, 0x64, 0x64,
};

ARM void Sound::unkfunc_0205590c(int bgm)
{
    g_SoundPlayer.unkfunc_0205c848(s_bgm_volume_tbl[bgm]);
    g_SoundPlayer.unkfunc_0205c76c(bgm);
    g_SoundPlayer.unkfunc_0205c7ac(0, bgm);
}

#ifdef DQIV_NATIVE
int Sound::nativeBgmVolume(int bgm)
{
    if (bgm < 0 || bgm >= (int)sizeof(s_bgm_volume_tbl)) {
        return 0x7f;
    }
    return s_bgm_volume_tbl[bgm];
}
#endif

ARM void Sound::unkfunc_0205594c(int fade)
{
    g_SoundPlayer.unkfunc_0205c800(0, fade);
}

ARM int Sound::unkfunc_02055968()
{
    return g_SoundPlayer.unkfunc_0205c824(0);
}

ARM void Sound::unkfunc_02055980(int value)
{
    g_SoundPlayer.unkfunc_0205ca68(value);
}

ARM void Sound::unkfunc_02055998(int value)
{
    g_SoundPlayer.unkfunc_0205ca84(value);
}

ARM int Sound::sePlay(int se)
{
    return g_SoundPlayer.unkfunc_0205c948(0, se);
}

ARM void Sound::unkfunc_020559cc(int se, int index)
{
    g_SoundPlayer.unkfunc_0205c9d0(0, se, index);
}

ARM void Sound::unkfunc_020559ec(int value)
{
    g_SoundPlayer.unkfunc_0205ca2c(value);
}

ARM int Sound::sePlayDirect(int se)
{
    int ret = g_SoundPlayer.unkfunc_0205c948(0, se);
    g_SoundSystem.unkfunc_020866d8();
    return ret;
}

ARM void Sound::setBgmVolume(int volume)
{
    g_SoundPlayer.unkfunc_0205c848(volume);
}

ARM int Sound::getBgmVolume()
{
    return g_SoundPlayer.unkfunc_0205c858();
}

ARM void Sound::setBgmVolumeSys(int volume)
{
    g_SoundPlayer.unkfunc_0205c838(volume);
}

ARM void Sound::setSeVolumeSys(int volume)
{
    g_SoundPlayer.unkfunc_0205c860(volume);
}
