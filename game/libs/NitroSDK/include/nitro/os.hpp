#pragma once

extern unsigned char data_027e0000[];             // DTCM start

extern "C" {
    void OS_Wait(void);
    void OS_Terminate(void);
    void* OS_GetArenaHi(int id);
    void* OS_GetArenaLo(int id);
    void* OS_AllocFromArenaLo(int id, unsigned int size, int align);
    void OS_EnableMainExArena(void);
    void OS_InitAllSystems(void);
    void OS_InitThread(void);
    void DC_CleanAll(void);
    void DC_CleanRange(const void* addr, unsigned int count);
#ifdef DQIV_NATIVE
    unsigned short* pcMasterInterruptRegister(void);
#endif
}

inline unsigned short OS_EnableIrq() {
#ifdef DQIV_NATIVE
    auto* master=pcMasterInterruptRegister();
    const auto previous=*master;*master=1;return previous;
#else
    unsigned short prep = *(volatile unsigned short*)0x04000208;
    *(volatile unsigned short*)0x04000208 = 1;
    return prep;
#endif
}
