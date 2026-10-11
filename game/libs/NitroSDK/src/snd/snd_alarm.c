#include "snd_internal.h"

UnkSndAlarm data_021160a0[8];

void func_0207ae54(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        data_021160a0[i].func = NULL;
        data_021160a0[i].arg = NULL;
        data_021160a0[i].id = 0;
    }
}

void func_0207ae84(s32 no) {
    UnkSndAlarm* alarm = &data_021160a0[no];
    alarm->id++;
}

u8 func_0207aea4(s32 no, UnkSndAlarmHandler func, void* arg) {
    UnkSndAlarm* alarm = &data_021160a0[no];

    alarm->func = func;
    alarm->arg = arg;
    alarm->id++;
    return alarm->id;
}

void func_0207aed4(s32 data) {
    s32 no = data & 0xff;
    u8 id = (u8)(data >> 8);
    UnkSndAlarm* alarm = &data_021160a0[no];

    if (id != alarm->id) {
        return;
    }
    if (alarm->func == NULL) {
        return;
    }
    alarm->func(alarm->arg);
}
