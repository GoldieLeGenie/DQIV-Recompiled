#include "snd_internal.h"

BOOL data_021145a0;
UnkMutex data_021145a4;

void SND_Init(void) {
    if (data_021145a0) {
        return;
    }
    data_021145a0 = TRUE;
    OS_InitMutex(&data_021145a4);
    func_0207a744();
    func_0207ae54();
}

void func_0207a71c(void) {
    OS_LockMutex(&data_021145a4);
}

void func_0207a730(void) {
    OS_UnlockMutex(&data_021145a4);
}
