#include "main/sound/SoundPlayer.hpp"
#include "main/data/DataObject.hpp"
#include "nnsys/snd.hpp"
#ifdef DQIV_NATIVE
#include "main/sound/Sound.hpp"
#include "MobileSound.hpp"
#endif

int data_0211fc68[2];
int data_020c4514 = 0x7f;

ARM UnkSoundHandle::UnkSoundHandle()
{
    unk_08 = 0x7f;
    unk_0c = 0x7f;
    unk_10 = 0;
    unk_14 = 0;
    unk_18 = 0;
    unk_1c = 0;
}

ARM UnkSoundHandle::~UnkSoundHandle()
{
}

ARM void UnkSoundHandle::unkfunc_02086494(int a)
{
    func_0207344c(this);
    unk_04 = a;
}

ARM void UnkSoundHandle::unkfunc_020864ac(int bgm)
{
#ifdef DQIV_NATIVE
    // The smartphone recording instead of the sequence, when there is one.
    if (pc::mobileSoundStart(this, pc::mobileBgm, bgm, unk_08 * unk_0c / 127, Sound::nativeBgmVolume(bgm))) {
        func_0207343c(this, 0);
        unk_10 = bgm;
        return;
    }
#endif
    func_0207581c(this, bgm);
    unk_10 = bgm;
    func_020734e0(this, data_020c4514);
    unkfunc_020865b4();
}

ARM void UnkSoundHandle::unkfunc_020864e0(int fade)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, fade);
#endif
    func_0207343c(this, fade);
}

ARM int UnkSoundHandle::unkfunc_020864ec()
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundOwns(this)) {
        return pc::mobileSoundPlaying(this);
    }
#endif
    return func_02073478(unk_10) != 0;
}

ARM void UnkSoundHandle::unkfunc_02086508(int a, int se)
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundStart(this, pc::mobileEffect, se, unk_08 * unk_0c / 127, 0x7f)) {
        func_0207343c(this, 0);
    } else {
        func_02075864(this, a, se);
    }
#else
    func_02075864(this, a, se);
#endif
    unk_14 = a;
    unk_18 = se;
    unk_1c = 1;
}

ARM int UnkSoundHandle::unkfunc_02086530(int a, int se)
{
    if (unk_14 != a) {
        return 0;
    }
    return unk_18 == se;
}

ARM void UnkSoundHandle::unkfunc_02086554(int fade)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, fade);
#endif
    func_0207343c(this, fade);
    unk_14 = -1;
    unk_18 = -1;
    unk_1c = 0;
}

ARM int UnkSoundHandle::unkfunc_02086578()
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundOwns(this)) {
        return pc::mobileSoundPlaying(this) || unk_1c == 1;
    }
#endif
    if (func_02073564(this) != 0) {
        return 1;
    }
    return unk_1c == 1;
}

ARM void UnkSoundHandle::unkfunc_020865a4(int volume)
{
    unk_0c = volume;
#ifdef DQIV_NATIVE
    pc::mobileSoundVolume(this, unk_08 * volume / 127);
#endif
    func_020734cc(this, volume);
}

ARM void UnkSoundHandle::unkfunc_020865b4()
{
#ifdef DQIV_NATIVE
    pc::mobileSoundVolume(this, unk_08 * unk_0c / 127);
#endif
    func_020734cc(this, unk_08 * unk_0c / 127);
}

ARM UnkSoundSystem::UnkSoundSystem()
{
    unk_98 = 0;
}

ARM UnkSoundSystem::~UnkSoundSystem()
{
}

ARM void UnkSoundSystem::unkfunc_020865f4(void* file, int flag)
{
    unk_9c = file;
    unk_a0 = flag;
    unk_00 = dssAlloc(&data_0211a60c, 0x9b000, 0x20);
    unk_04 = func_02074bd8(unk_00, 0x9b000);
    unkfunc_02086634();
}

ARM void UnkSoundSystem::unkfunc_02086634()
{
    func_02074c60(unk_04);
    func_02074550(unk_08, unk_9c, unk_04, 0);
    func_02075780(unk_04);
    if (unk_a0 != 0) {
        func_02075aa4(0x14, unk_04);
    }
    func_020741d0(3);
    func_02073370(0, 5);
    func_02073370(1, 5);
    func_02073370(2, 5);
    func_02073370(3, 5);
    func_02073370(4, 5);
    func_02073370(5, 5);
    func_02073370(6, 5);
}

ARM void UnkSoundSystem::unkfunc_020866d8()
{
    func_020731e4();
}

ARM void UnkSoundSystem::unkfunc_020866e4(int bgm)
{
    if (bgm == 0x22) {
        unkfunc_02086634();
    }
    if (func_02074e24(bgm, unk_04) == 0) {
        func_02074e24(bgm, unk_04);
    }
}

ARM void UnkSoundSystem::unkfunc_02086720(int a, int b)
{
    func_02074e50(a, unk_04);
    func_02074e7c(b, unk_04);
}

ARM UnkSoundMember::UnkSoundMember()
{
}

ARM UnkSoundMember::~UnkSoundMember()
{
}

ARM void UnkSoundMember::unkfunc_02086750()
{
    func_02075d44(this);
}

ARM void UnkSoundMember::unkfunc_0208675c(int value)
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundStart(this, pc::mobileStream, value, data_020c4518, 0x7f)) {
        func_02075d14(this, 0);
        return;
    }
#endif
    func_02075cec(this, value, 0);
    func_02075d30(this, data_020c4518);
}

ARM void UnkSoundMember::unkfunc_02086784(int value)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, value);
#endif
    func_02075d14(this, value);
}

UnkSoundSystem data_0211fc7c;
int data_020c4518 = 0x7f;
