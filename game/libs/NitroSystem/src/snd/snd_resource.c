#include "../nns_internal.h"

typedef struct {
    u32 unk_00;                     /* 0x00 */
    u32 allocatedBits;              /* 0x04 */
    u32 lockedChannels;             /* 0x08 */
} UnkSndResourceState;

UnkSndResourceState data_021122e0;

void SND_LockChannel(u32 chBitMask, u32 flags);
void func_0207a53c(u32 chBitMask, u32 flags);

BOOL func_02073258(u32 chBitMask)
{
    if (chBitMask == 0) {
        return TRUE;
    }
    if (chBitMask & data_021122e0.lockedChannels) {
        return FALSE;
    }
    SND_LockChannel(chBitMask, 0);
    data_021122e0.lockedChannels |= chBitMask;
    return TRUE;
}

void func_020732a0(u32 chBitMask)
{
    if (chBitMask == 0) {
        return;
    }
    func_0207a53c(chBitMask, 0);
    data_021122e0.lockedChannels &= ~chBitMask;
}

void func_020732d0(u32 bitMask)
{
    data_021122e0.unk_00 &= ~bitMask;
}

int func_020732ec(void)
{
    int i;
    u32 bit = 1;

    for (i = 0; i < 8; i++) {
        if ((data_021122e0.allocatedBits & bit) == 0) {
            data_021122e0.allocatedBits |= bit;
            return i;
        }
        bit <<= 1;
    }
    return -1;
}

void func_02073334(int no)
{
    data_021122e0.allocatedBits &= ~(1 << no);
}

void func_02073354(void)
{
    data_021122e0.lockedChannels = 0;
    data_021122e0.unk_00 = 0;
    data_021122e0.allocatedBits = 0;
}
