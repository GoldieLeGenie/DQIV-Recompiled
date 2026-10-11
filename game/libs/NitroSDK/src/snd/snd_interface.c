#include "snd_internal.h"

void func_0207a390(u32 arg0) {
    func_0207a6a0(1, arg0, 0, 0, 0);
}

void func_0207a3b0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0207a6a0(2, arg0, arg1, arg2, arg3);
}

void func_0207a3d8(u32 arg0) {
    func_0207a6a0(3, arg0, 0, 0, 0);
}

void func_0207a3f8(u32 arg0, u32 arg1) {
    func_0207a678(arg0, 6, arg1, 2);
}

void func_0207a410(u32 arg0, u32 arg1) {
    func_0207a678(arg0, 4, arg1, 1);
}

void func_0207a428(u32 arg0, u32 arg1, u32 arg2) {
    func_0207a6a0(9, arg0, arg1, arg2, 0);
}

void func_0207a450(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0207a6a0(12, arg0, arg1, arg2, arg3);
}

void func_0207a478(u32 arg0, u32 arg1, u32 alarmMask, u32 arg3) {
    s32 i;
    u32 mask = alarmMask;

    for (i = 0; i < 8 && mask != 0; i++, mask >>= 1) {
        if (mask & 1) {
            func_0207ae84(i);
        }
    }
    func_0207a6a0(13, arg0, arg1, alarmMask, arg3);
}

void SND_SetupAlarm(s32 alarmNo, u32 tick, u32 period, UnkSndAlarmHandler func, void* arg) {
    func_0207a6a0(18, alarmNo, tick, period, func_0207aea4(alarmNo, func, arg));
}

void SND_LockChannel(u32 chMask, u32 flags) {
    func_0207a6a0(26, chMask, flags, 0, 0);
}

void func_0207a53c(u32 chMask, u32 flags) {
    func_0207a6a0(27, chMask, flags, 0, 0);
}

void SND_SetChannelVolume(u32 chMask, s32 volume, s32 shift) {
    func_0207a6a0(20, chMask, volume, shift, 0);
}

void SND_SetChannelPan(u32 chMask, s32 pan) {
    func_0207a6a0(21, chMask, pan, 0, 0);
}

void SND_SetupChannelPcm(s32 chNo, s32 format, const void* dataAddr, s32 loop, s32 loopStart, s32 loopLen,
                         s32 volume, s32 shift, s32 timer, s32 pan) {
    func_0207a6a0(14, chNo | (timer << 16), (u32)dataAddr, loopLen | ((volume << 24) | (shift << 22)),
                  loopStart | ((loop << 26) | (format << 24) | (pan << 16)));
}

void func_0207a5f0(u32 arg0, u32 arg1) {
    func_0207a6a0(30, arg0, arg1, 0, 0);
}

void func_0207a610(u32 arg0, u32 arg1) {
    func_0207a6a0(31, arg0, arg1, 0, 0);
}

void func_0207a630(u32 arg0, u32 arg1) {
    func_0207a6a0(32, arg0, arg1, 0, 0);
}

void func_0207a650(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0207a6a0(25, arg0, arg1, arg2, arg3);
}

void func_0207a678(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0207a6a0(6, arg0, arg1, arg2, arg3);
}

void func_0207a6a0(s32 id, u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    UnkSndCommand* cmd = func_0207a928(1);
    if (cmd == NULL) {
        return;
    }
    cmd->id = id;
    cmd->arg[0] = arg0;
    cmd->arg[1] = arg1;
    cmd->arg[2] = arg2;
    cmd->arg[3] = arg3;
    func_0207a9b0(cmd);
}
