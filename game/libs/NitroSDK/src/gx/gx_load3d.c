#include "gx_internal.h"

/* data_0210ce94 (0x20 bytes): texture / texture palette / clear image load state */
typedef struct {
    /* 0x00 */ s32 clearImageBank;
    /* 0x04 */ u32 texStartAddr;
    /* 0x08 */ u32 texPlttStartAddr;
    /* 0x0c */ s32 texPlttBank;
    /* 0x10 */ u32 clearImageAddr;
    /* 0x14 */ s32 texBank;
    /* 0x18 */ u32 texStartAddr2;
    /* 0x1c */ u32 texSize1;
} UnkGxLoad3dState;
UnkGxLoad3dState data_0210ce94;

/* texture palette start address per bank combination (in 4KB units) */
const u16 data_020ba490[8] = {0x0000, 0x6880, 0x6890, 0x6880, 0x6894, 0x0000, 0x6890, 0x6880};

/* per texture bank combination: start address, second block address and first block size (in 4KB units) */
const u16 data_020ba4a0[16][3] = {
    {0x0000, 0x0000, 0x00}, {0x6800, 0x0000, 0x00}, {0x6820, 0x0000, 0x00}, {0x6800, 0x0000, 0x00},
    {0x6840, 0x0000, 0x00}, {0x6800, 0x6840, 0x20}, {0x6820, 0x0000, 0x00}, {0x6800, 0x0000, 0x00},
    {0x6860, 0x0000, 0x00}, {0x6800, 0x6860, 0x20}, {0x6820, 0x6860, 0x20}, {0x6800, 0x6860, 0x40},
    {0x6840, 0x0000, 0x00}, {0x6800, 0x6840, 0x20}, {0x6820, 0x0000, 0x00}, {0x6800, 0x0000, 0x00},
};

void MI_CpuCopyU32(const void *src, void *dest, u32 size);
void func_0206714c(u32 dmaNo, const void *src, void *dest, u32 size);
void func_020672ec(u32 dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *), void *arg);

s32 GX_ResetBankForTex(void);
s32 func_0206493c(void);
s32 func_02064950(void);
void GX_SetBankForTex(s32 tex);
void GX_SetBankForTexPltt(s32 texPltt);
void GX_SetBankForClearImage(s32 clearImage);

static inline void UnkGxCopy32(u32 dmaNo, const void *src, void *dest, u32 size) {
#ifdef DQIV_NATIVE
    pcVideoCopy((u32)dest, src, size);
#else
    if (dmaNo != -1 && size > 0x30) {
        func_0206714c(dmaNo, src, dest, size);
    } else {
        MI_CpuCopyU32(src, dest, size);
    }
#endif
}

static inline void UnkGxCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size) {
#ifdef DQIV_NATIVE
    pcVideoCopy((u32)dest, src, size);
#else
    if (dmaNo != -1) {
        func_020672ec(dmaNo, src, dest, size, NULL, NULL);
    } else {
        MI_CpuCopyU32(src, dest, size);
    }
#endif
}

static inline void UnkGxWaitDma(u32 dmaNo) {
    if (dmaNo != -1) {
        func_02067384(dmaNo);
    }
}

void func_02066958(void) {
    s32 bank = GX_ResetBankForTex();
    data_0210ce94.texBank       = bank;
    data_0210ce94.texStartAddr  = data_020ba4a0[bank][0] << 12;
    data_0210ce94.texStartAddr2 = data_020ba4a0[bank][1] << 12;
    data_0210ce94.texSize1      = data_020ba4a0[bank][2] << 12;
}

void func_020669b4(const void *src, u32 destSlotAddr, u32 size) {
    void *dest;

    if (data_0210ce94.texStartAddr2 == 0) {
        dest = (void *)(data_0210ce94.texStartAddr + destSlotAddr);
    } else if (destSlotAddr + size < data_0210ce94.texSize1) {
        dest = (void *)(data_0210ce94.texStartAddr + destSlotAddr);
    } else if (destSlotAddr >= data_0210ce94.texSize1) {
        dest = (void *)(data_0210ce94.texStartAddr2 + destSlotAddr - data_0210ce94.texSize1);
    } else {
        void *dest2 = (void *)data_0210ce94.texStartAddr2;
        u32 size1   = data_0210ce94.texSize1 - destSlotAddr;
        UnkGxCopy32(data_020c3dbc, src, (void *)(data_0210ce94.texStartAddr + destSlotAddr), size1);
        UnkGxCopy32Async(data_020c3dbc, (u8 *)src + size1, dest2, size - size1);
        return;
    }
    UnkGxCopy32Async(data_020c3dbc, src, dest, size);
}

void func_02066af4(void) {
    UnkGxWaitDma(data_020c3dbc);
    GX_SetBankForTex(data_0210ce94.texBank);
    data_0210ce94.texSize1      = 0;
    data_0210ce94.texStartAddr2 = 0;
    data_0210ce94.texStartAddr  = 0;
    data_0210ce94.texBank       = 0;
}

void func_02066b40(void) {
    s32 bank = func_0206493c();
    data_0210ce94.texPlttBank      = bank;
    data_0210ce94.texPlttStartAddr = data_020ba490[bank >> 4] << 12;
}

void func_02066b74(const void *src, u32 destSlotAddr, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)(data_0210ce94.texPlttStartAddr + destSlotAddr), size);
}

void func_02066be0(void) {
    UnkGxWaitDma(data_020c3dbc);
    GX_SetBankForTexPltt(data_0210ce94.texPlttBank);
    data_0210ce94.texPlttBank      = 0;
    data_0210ce94.texPlttStartAddr = 0;
}

void func_02066c24(void) {
    data_0210ce94.clearImageBank = func_02064950();

    switch (data_0210ce94.clearImageBank) {
    case 0x02:
    case 0x03:
        data_0210ce94.clearImageAddr = 0x6800000;
        break;
    case 0x08:
    case 0x0c:
        data_0210ce94.clearImageAddr = 0x6840000;
        break;
    case 0x01:
        data_0210ce94.clearImageAddr = 0x67e0000;
        break;
    case 0x04:
        data_0210ce94.clearImageAddr = 0x6820000;
        break;
    default:
        break;
    }
}

void func_02066cb4(const void *src, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)data_0210ce94.clearImageAddr, size);
}

void func_02066d1c(const void *src, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)(data_0210ce94.clearImageAddr + 0x20000), size);
}

void func_02066d88(void) {
    UnkGxWaitDma(data_020c3dbc);
    GX_SetBankForClearImage(data_0210ce94.clearImageBank);
    data_0210ce94.clearImageBank = 0;
    data_0210ce94.clearImageAddr = 0;
}
