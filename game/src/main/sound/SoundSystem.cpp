#include "main/sound/SoundPlayer.hpp"
#include "main/data/DataObject.hpp"
#include "nnsys/snd.hpp"
#ifdef DQIV_NATIVE
#include "main/sound/Sound.hpp"
#include "MobileSound.hpp"
#endif

int data_0211fc68[2];
int g_seqStartVolume = 0x7f;

ARM SoundSeqHandle::SoundSeqHandle()
{
    volumeSys_ = 0x7f;
    volume_ = 0x7f;
    seqNo_ = 0;
    unk_14 = 0;
    seNo_ = 0;
    justStarted_ = 0;
}

ARM SoundSeqHandle::~SoundSeqHandle()
{
}

ARM void SoundSeqHandle::unkfunc_02086494(int a)
{
    func_0207344c(this);
    unk_04 = a;
}

ARM void SoundSeqHandle::unkfunc_020864ac(int bgm)
{
#ifdef DQIV_NATIVE
    // The smartphone recording instead of the sequence, when there is one.
    if (pc::mobileSoundStart(this, pc::mobileBgm, bgm, volumeSys_ * volume_ / 127, Sound::nativeBgmVolume(bgm))) {
        func_0207343c(this, 0);
        seqNo_ = bgm;
        return;
    }
#endif
    func_0207581c(this, bgm);
    seqNo_ = bgm;
    func_020734e0(this, g_seqStartVolume);
    unkfunc_020865b4();
}

ARM void SoundSeqHandle::unkfunc_020864e0(int fade)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, fade);
#endif
    func_0207343c(this, fade);
}

ARM int SoundSeqHandle::unkfunc_020864ec()
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundOwns(this)) {
        return pc::mobileSoundPlaying(this);
    }
#endif
    return func_02073478(seqNo_) != 0;
}

ARM void SoundSeqHandle::unkfunc_02086508(int a, int se)
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundStart(this, pc::mobileEffect, se, volumeSys_ * volume_ / 127, 0x7f)) {
        func_0207343c(this, 0);
    } else {
        func_02075864(this, a, se);
    }
#else
    func_02075864(this, a, se);
#endif
    unk_14 = a;
    seNo_ = se;
    justStarted_ = 1;
}

ARM int SoundSeqHandle::unkfunc_02086530(int a, int se)
{
    if (unk_14 != a) {
        return 0;
    }
    return seNo_ == se;
}

ARM void SoundSeqHandle::unkfunc_02086554(int fade)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, fade);
#endif
    func_0207343c(this, fade);
    unk_14 = -1;
    seNo_ = -1;
    justStarted_ = 0;
}

ARM int SoundSeqHandle::unkfunc_02086578()
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundOwns(this)) {
        return pc::mobileSoundPlaying(this) || justStarted_ == 1;
    }
#endif
    if (func_02073564(this) != 0) {
        return 1;
    }
    return justStarted_ == 1;
}

ARM void SoundSeqHandle::unkfunc_020865a4(int volume)
{
    volume_ = volume;
#ifdef DQIV_NATIVE
    pc::mobileSoundVolume(this, volumeSys_ * volume / 127);
#endif
    func_020734cc(this, volume);
}

ARM void SoundSeqHandle::unkfunc_020865b4()
{
#ifdef DQIV_NATIVE
    pc::mobileSoundVolume(this, volumeSys_ * volume_ / 127);
#endif
    func_020734cc(this, volumeSys_ * volume_ / 127);
}

ARM DsSoundSystem::DsSoundSystem()
{
    unk_98 = 0;
}

ARM DsSoundSystem::~DsSoundSystem()
{
}

ARM void DsSoundSystem::unkfunc_020865f4(void* file, int flag)
{
    arcFileName_ = file;
    useStream_ = flag;
    heapMemory_ = dssAlloc(&g_appHeap, 0x9b000, 0x20);
    heap_ = func_02074bd8(heapMemory_, 0x9b000);
    unkfunc_02086634();
}

ARM void DsSoundSystem::unkfunc_02086634()
{
    func_02074c60(heap_);
    func_02074550(arc_, arcFileName_, heap_, 0);
    func_02075780(heap_);
    if (useStream_ != 0) {
        func_02075aa4(0x14, heap_);
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

ARM void DsSoundSystem::unkfunc_020866d8()
{
    func_020731e4();
}

ARM void DsSoundSystem::unkfunc_020866e4(int bgm)
{
    if (bgm == 0x22) {
        unkfunc_02086634();
    }
    if (func_02074e24(bgm, heap_) == 0) {
        func_02074e24(bgm, heap_);
    }
}

ARM void DsSoundSystem::unkfunc_02086720(int a, int b)
{
    func_02074e50(a, heap_);
    func_02074e7c(b, heap_);
}

ARM SoundStreamHandle::SoundStreamHandle()
{
}

ARM SoundStreamHandle::~SoundStreamHandle()
{
}

ARM void SoundStreamHandle::unkfunc_02086750()
{
    func_02075d44(this);
}

ARM void SoundStreamHandle::unkfunc_0208675c(int value)
{
#ifdef DQIV_NATIVE
    if (pc::mobileSoundStart(this, pc::mobileStream, value, g_strmStartVolume, 0x7f)) {
        func_02075d14(this, 0);
        return;
    }
#endif
    func_02075cec(this, value, 0);
    func_02075d30(this, g_strmStartVolume);
}

ARM void SoundStreamHandle::unkfunc_02086784(int value)
{
#ifdef DQIV_NATIVE
    pc::mobileSoundStop(this, value);
#endif
    func_02075d14(this, value);
}

DsSoundSystem g_SoundSystem;
int g_strmStartVolume = 0x7f;
