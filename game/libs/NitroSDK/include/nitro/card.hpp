#pragma once
#include <stddef.h>

#define CARD_BACKUP_TYPE_EEPROM_512KBITS 0x1001

extern "C" {
    void func_0205d6f8(unsigned short lockID);                                       // CARD_LockBackup
    void func_0205d708(unsigned short lockID);                                       // CARD_UnlockBackup
    int func_0205dc74(unsigned int type);                                           // CARD_IdentifyBackup
    int func_0205d6ac(void);                                                        // CARD_GetResultCode
    int func_0205ddac(void);                                                        // CARD_TryWaitBackupAsync
    int func_0205db78(unsigned int src, unsigned int dst, unsigned int len, void* callback, void* arg,
                      int isAsync, int reqType, int reqRetry, int reqMode);         // CARDi_RequestStreamCommand
}

inline int CARD_ReadBackup(unsigned int src, void* dst, unsigned int len)
{
    return func_0205db78(src, (unsigned int)dst, len, NULL, NULL, 0, 6, 1, 0);
}

inline int CARD_WriteAndVerifyBackup(unsigned int dst, const void* src, unsigned int len)
{
    return func_0205db78((unsigned int)src, dst, len, NULL, NULL, 0, 8, 10, 2);
}

inline int CARD_WriteAndVerifyBackupAsync(unsigned int dst, const void* src, unsigned int len, void* callback, void* arg)
{
    return func_0205db78((unsigned int)src, dst, len, callback, arg, 1, 8, 10, 2);
}
