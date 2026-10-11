#include "main/sound/UnkSoundPlayer.hpp"

UnkSoundPlayer data_0210bd4c;

ARM UnkSoundPlayer::UnkSoundPlayer() : enable_(0)
{
    bgmVolumeSys_ = 0x7f;
    bgmVolume_ = 0x7f;
    seVolumeSys_ = 0x7f;
    seVolume_ = 0x7f;
}

ARM UnkSoundPlayer::~UnkSoundPlayer()
{
}

ARM void UnkSoundPlayer::unkfunc_0205c710()
{
    enable_ = 1;
    unk_008.unkfunc_02086750();
    bgmHandle_[0].unkfunc_02086494(-1);
    bgmHandle_[1].unkfunc_02086494(-1);
    for (int i = 0; i < 8; i++) {
        seHandle_[i].unkfunc_02086494(-1);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205c76c(int bgm)
{
    if (enable_) {
        data_0211fc7c.unkfunc_020866e4(bgm);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205c78c(int a, int b)
{
    if (enable_) {
        data_0211fc7c.unkfunc_02086720(a, b);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205c7ac(int player, int bgm)
{
    if (!enable_) {
        return;
    }
    if (unkfunc_0205c824(bgm)) {
        unkfunc_0205c800(0, 30);
    }
    bgm_ = bgm;
    bgmHandle_[player].unkfunc_020864ac(bgm);
}

ARM void UnkSoundPlayer::unkfunc_0205c800(int player, int fade)
{
    if (enable_) {
        bgmHandle_[player].unkfunc_020864e0(fade);
    }
}

ARM int UnkSoundPlayer::unkfunc_0205c824(int player)
{
    return bgmHandle_[player].unkfunc_020864ec();
}

ARM void UnkSoundPlayer::unkfunc_0205c838(int volume)
{
    bgmVolumeSys_ = volume;
    unkfunc_0205c870();
}

ARM void UnkSoundPlayer::unkfunc_0205c848(int volume)
{
    bgmVolume_ = volume;
    unkfunc_0205c870();
}

ARM int UnkSoundPlayer::unkfunc_0205c858()
{
    return bgmVolume_;
}

ARM void UnkSoundPlayer::unkfunc_0205c860(int volume)
{
    seVolumeSys_ = volume;
    unkfunc_0205c8c0();
}

ARM void UnkSoundPlayer::unkfunc_0205c870()
{
    int volume = bgmVolumeSys_ * bgmVolume_ / 127;
    for (int i = 0; i < 2; i++) {
        bgmHandle_[i].unkfunc_020865a4(volume);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205c8c0()
{
    int volume = seVolumeSys_ * seVolume_ / 127;
    for (int i = 0; i < 8; i++) {
        seHandle_[i].unkfunc_020865a4(volume);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205c910(int index)
{
    seHandle_[index].unkfunc_020865a4(seVolumeSys_ * seVolume_ / 127);
}

ARM int UnkSoundPlayer::unkfunc_0205c948(int a, int se)
{
    if (!enable_) {
        return -1;
    }
    int ret = 0;
    for (int i = 0; i < 8; i++) {
        if (seHandle_[i].unkfunc_02086530(-1, -1)) {
            ret = i;
            seHandle_[i].unkfunc_02086508(a, se);
            unkfunc_0205c910(i);
            break;
        }
    }
    return ret;
}

ARM void UnkSoundPlayer::unkfunc_0205c9d0(int a, int se, int fade)
{
    if (!enable_) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (seHandle_[i].unkfunc_02086530(a, se)) {
            seHandle_[i].unkfunc_02086554(fade);
        }
    }
}

ARM void UnkSoundPlayer::unkfunc_0205ca2c(int fade)
{
    if (!enable_) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        seHandle_[i].unkfunc_02086554(fade);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205ca68(int value)
{
    if (enable_) {
        unk_008.unkfunc_0208675c(value);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205ca84(int value)
{
    if (enable_) {
        unk_008.unkfunc_02086784(value);
    }
}

ARM void UnkSoundPlayer::unkfunc_0205caa0()
{
    for (int i = 0; i < 8; i++) {
        if (seHandle_[i].unk_1c) {
            seHandle_[i].unk_1c = 0;
        } else if (!seHandle_[i].unkfunc_02086578()) {
            seHandle_[i].unkfunc_02086554(5);
        }
    }
}
