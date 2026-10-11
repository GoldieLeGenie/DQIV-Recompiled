#pragma once

// sleep callback of the PM library
struct UnkPmSleepCallback {
    void (*func_)(void* arg);                   // 0x00
    void* arg_;                                 // 0x04
    UnkPmSleepCallback* next_;                  // 0x08
};

// PM functions without a name in the libs headers
extern "C" {
    void func_0207bdb4(unsigned int trigger, unsigned int logic, unsigned int keyPattern);  // go to sleep mode
    int func_0207c0b8(int on);                  // LCD power
    int func_0207c0d8(void);                    // LCD power state
    void func_0207c1c4(UnkPmSleepCallback* info);   // add a pre-sleep callback
    void func_0207c1f4(UnkPmSleepCallback* info);   // add a post-sleep callback
}
