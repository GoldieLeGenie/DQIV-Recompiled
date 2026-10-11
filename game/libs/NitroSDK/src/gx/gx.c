#include "gx_internal.h"

u16 data_020c3db8 = TRUE;
#ifdef DQIV_NATIVE
u32 data_020c3dbc = (u32)-1;
#else
u32 data_020c3dbc = 3;
#endif

u16 data_0210ce5a; /* GX lock id */
u16 data_0210ce58; /* saved display mode */

void func_0206366c(void) {
}

void GX_Init(void) {
    REG_POWER_CNT |= 0x8000;
    REG_POWER_CNT = (REG_POWER_CNT & ~0x20e) | 0x20e;
    REG_POWER_CNT |= 1;
    func_02063960();

    while (data_0210ce5a == 0) {
        s32 lockId = OS_GetLockID();
        if (lockId == -3) {
            OS_Terminate();
        }
        data_0210ce5a = lockId;
    }

    REG_DISPSTAT = 0;
    REG_DISPCNT  = 0;

    if (data_020c3dbc != -1) {
        func_020670cc(data_020c3dbc, (void *)&REG_BG0CNT, 0, 0x60);
        REG_MASTER_BRIGHT = 0;
        func_020670cc(data_020c3dbc, (void *)&REG_DISPCNT_SUB, 0, 0x70);
    } else {
        func_0206785c(0, (void *)&REG_BG0CNT, 0x60);
        REG_MASTER_BRIGHT = 0;
        func_0206785c(0, (void *)&REG_DISPCNT_SUB, 0x70);
    }

    REG_BG2PA     = 0x100;
    REG_BG2PD     = 0x100;
    REG_BG3PA     = 0x100;
    REG_BG3PD     = 0x100;
    REG_BG2PA_SUB = 0x100;
    REG_BG2PD_SUB = 0x100;
    REG_BG3PA_SUB = 0x100;
    REG_BG3PD_SUB = 0x100;
}

s32 GX_VBlankIntr(BOOL enable) {
    s32 prev = REG_DISPSTAT & 8;
    if (enable) {
        REG_DISPSTAT |= 8;
    } else {
        REG_DISPSTAT &= ~8;
    }
    return prev;
}

void GX_DispOff(void) {
    u32 cnt = REG_DISPCNT;
    data_020c3db8 = FALSE;
    data_0210ce58 = (cnt & 0x30000) >> 16;
    REG_DISPCNT = cnt & ~0x30000;
}

void GX_DispOn(void) {
    data_020c3db8 = TRUE;
    if (data_0210ce58 != 0) {
        REG_DISPCNT = (REG_DISPCNT & ~0x30000) | (data_0210ce58 << 16);
    } else {
        REG_DISPCNT |= 0x10000;
    }
}

void GX_SetGraphicsMode(u32 dispMode, u32 bgMode, u32 bg0_2d3d) {
    u32 cnt = REG_DISPCNT;

    data_0210ce58 = dispMode;
    if (!data_020c3db8) {
        dispMode = 0;
    }
    REG_DISPCNT = (cnt & ~0xf000f) | (dispMode << 16) | bgMode | (bg0_2d3d << 3);
    if (data_0210ce58 == 0) {
        data_020c3db8 = FALSE;
    }
}

void GXS_SetGraphicsMode(u32 bgMode) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~7) | bgMode;
}

void func_020638f8(vu16 *reg, s32 brightness) {
    if (brightness == 0) {
        *reg = 0;
    } else if (brightness > 0) {
        *reg = brightness | 0x4000;
    } else {
        *reg = -brightness | 0x8000;
    }
}

u32 func_02063920(u32 dmaNo) {
    u32 prev = data_020c3dbc;
    u32 e;
    if (prev != -1) {
        func_02067384(prev);
    }
    e = OS_DisableIRQ();
    data_020c3dbc = dmaNo;
    OS_RestoreIRQ(e);
    return prev;
}
