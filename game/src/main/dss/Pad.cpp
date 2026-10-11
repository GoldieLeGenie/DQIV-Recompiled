#include "main/dss/Pad.hpp"
#include "main/dss/DssCore.hpp"
#include "nitro/os.hpp"
#include "nitro/pad.h"
#ifdef DQIV_NATIVE
#include <cstring>
#endif

dss::Pad dss::g_Pad;
unsigned short data_02116d94[7200];

ARM void dss::Pad::unkfunc_0207ed54()
{
#ifdef DQIV_NATIVE
    std::memset(this, 0, sizeof(*this));
#endif
    maskL_ = 0;
    mode_ = 0;
    playIndex_ = 0;
    recordCount_ = 0;
    unkfunc_0207f2b4(0);
}

ARM void dss::Pad::unkfunc_0207ed74()
{
    unsigned short keys = PAD_Read();
    if ((keys & (PAD_KEY_UP | PAD_KEY_DOWN)) == (PAD_KEY_UP | PAD_KEY_DOWN)) {
        keys ^= PAD_KEY_DOWN;
    }
    if ((keys & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) == (PAD_KEY_RIGHT | PAD_KEY_LEFT)) {
        keys ^= PAD_KEY_LEFT;
    }
    switch (mode_) {
    case 0:
        if (trig_ & 0x2000) {
            if (keys & PAD_BUTTON_B) {
                keys = 0;
                unkfunc_0207f320();
            } else {
                keys = 0;
                unkfunc_0207f2d4();
            }
        }
        break;
    case 1:
        if (wait_ != 0) {
            data_02116ce0.unkfunc_0207e88c(0x18, 0x17, "REC WAIT");
            keys = 0;
            wait_--;
        } else {
            data_02116ce0.unkfunc_0207e88c(0x18, 0x17, "REC %4d", 7200 - recordCount_);
            if (PAD_DetectFold()) {
                unkfunc_0207f2f0();
            }
            if (keys & PAD_BUTTON_SELECT) {
                unkfunc_0207f2f0();
            }
            if (keys & PAD_BUTTON_START) {
                unkfunc_0207f2f0();
            }
            if (mode_ == 1) {
                unkfunc_0207f39c(keys);
                if (recordCount_ == 7200) {
                    unkfunc_0207f2f0();
                }
            }
        }
        break;
    case 2:
        if (wait_ != 0) {
            data_02116ce0.unkfunc_0207e88c(0x17, 0x17, "PLAY WAIT");
            keys = 0;
            wait_--;
        } else {
            data_02116ce0.unkfunc_0207e88c(0x17, 0x17, "PLAY %4d", playCount_ + 1);
            data_02116ce0.unkfunc_0207e88c(0x17, 0x16, "%4d/%4d", playIndex_, recordCount_);
            if (PAD_DetectFold()) {
                unkfunc_0207f3bc();
            }
            if (keys & PAD_BUTTON_SELECT) {
                unkfunc_0207f3bc();
            }
            if (keys & PAD_BUTTON_START) {
                unkfunc_0207f3bc();
            }
            if (mode_ == 2) {
                keys = unkfunc_0207f360();
            }
        }
        break;
    }
    trig_ = keys & (keys ^ cont_);
    release_ = cont_ & (keys ^ cont_);
    cont_ = keys;
    for (unsigned int i = 0; i < 12; i++) {
        if (pad() & (1 << i)) {
            repeat_ &= ~(1 << i);
            if (repeatTimer_[i] == 0) {
                repeatTimer_[i] = 0x200000;
                repeat_ |= 1 << i;
            } else {
                if (repeatTimer_[i] & 0x80000000) {
                    repeat_ |= 1 << i;
                }
                repeatTimer_[i] <<= 1;
                if (repeatTimer_[i] == 0) {
                    repeatTimer_[i] = 0x2000000;
                }
            }
        } else {
            repeat_ &= ~(1 << i);
            repeatTimer_[i] = 0;
        }
    }
    if ((pad() & PAD_KEY_UP) && (pad() & PAD_KEY_RIGHT)) {
        dir_ = 1;
        return;
    }
    if ((pad() & PAD_KEY_RIGHT) && (pad() & PAD_KEY_DOWN)) {
        dir_ = 3;
        return;
    }
    if ((pad() & PAD_KEY_DOWN) && (pad() & PAD_KEY_LEFT)) {
        dir_ = 5;
        return;
    }
    if ((pad() & PAD_KEY_LEFT) && (pad() & PAD_KEY_UP)) {
        dir_ = 7;
        return;
    }
    if ((pad() & PAD_KEY_UP) && (pad() & PAD_KEY_UP)) {
        dir_ = 0;
        return;
    }
    if ((pad() & PAD_KEY_RIGHT) && (pad() & PAD_KEY_RIGHT)) {
        dir_ = 2;
        return;
    }
    if ((pad() & PAD_KEY_DOWN) && (pad() & PAD_KEY_DOWN)) {
        dir_ = 4;
        return;
    }
    if ((pad() & PAD_KEY_LEFT) && (pad() & PAD_KEY_LEFT)) {
        dir_ = 6;
    }
}

ARM void dss::Pad::unkfunc_0207f1f8()
{
    unkfunc_0207ed74();
    if (maskL_ == 0) {
        return;
    }
    mask_ = 0xfdff;
    if (!(cont_ & PAD_BUTTON_L)) {
        return;
    }
    do {
        mask_ = 0xfcff;
        unkfunc_0207ed74();
        OS_Wait();
        if (trig_ & PAD_BUTTON_R) {
            return;
        }
        if (!(cont_ & PAD_BUTTON_L)) {
            return;
        }
    } while (!unkfunc_0207f2a0());
}

ARM int dss::Pad::pad()
{
    return cont_ & mask_;
}

ARM int dss::Pad::padDir()
{
    return dir_;
}

ARM int dss::Pad::edge()
{
    return trig_ & mask_;
}

ARM int dss::Pad::unkfunc_0207f290()
{
    return repeat_ & mask_;
}

ARM int dss::Pad::unkfunc_0207f2a0()
{
    return cont_ == (PAD_BUTTON_L | PAD_BUTTON_R | PAD_BUTTON_SELECT | PAD_BUTTON_START);
}

ARM void dss::Pad::unkfunc_0207f2b4(int flag)
{
    maskL_ = flag;
    if (flag) {
        mask_ = 0xfdff;
    } else {
        mask_ = 0xffff;
    }
}

ARM void dss::Pad::unkfunc_0207f2d4()
{
    mode_ = 1;
    recordCount_ = 0;
    wait_ = 60;
}

ARM void dss::Pad::unkfunc_0207f2f0()
{
    data_02116ce0.unkfunc_0207e88c(0x18, 0x17, "        ");
    mode_ = 0;
}

ARM void dss::Pad::unkfunc_0207f320()
{
    mode_ = 2;
    playIndex_ = 0;
    playCount_ = 0;
    wait_ = 60;
}

ARM void dss::Pad::unkfunc_0207f340()
{
    mode_ = 2;
    playIndex_ = 0;
    playCount_++;
}

ARM unsigned short dss::Pad::unkfunc_0207f360()
{
    unsigned short keys = data_02116d94[playIndex_++];
    if (playIndex_ == recordCount_) {
        unkfunc_0207f340();
    }
    return keys;
}

ARM void dss::Pad::unkfunc_0207f39c(unsigned short keys)
{
    data_02116d94[recordCount_++] = keys;
}

ARM void dss::Pad::unkfunc_0207f3bc()
{
    data_02116ce0.unkfunc_0207e88c(0x17, 0x17, "         ");
    data_02116ce0.unkfunc_0207e88c(0x17, 0x16, "         ");
    mode_ = 0;
}

ARM void dss::Pad::unkfunc_0207f400()
{
    recordCount_ = 0;
}

ARM void dss::Pad::unkfunc_0207f40c(unsigned short keys, int count)
{
    for (int i = 0; i < count; i++) {
        data_02116d94[recordCount_++] = keys;
    }
}

ARM void dss::Pad::unkfunc_0207f44c(unsigned short keys, int count)
{
    for (int i = 0; i < count; i++) {
        unkfunc_0207f40c(keys, 10);
        unkfunc_0207f40c(0, 10);
    }
}
