#ifndef GX_INTERNAL_H
#define GX_INTERNAL_H

#include <nitro/types.h>
#include <nitro/reg.h>
#ifdef DQIV_NATIVE
#include <nitro/native_video.h>
s32 OS_GetLockID(void);
void OS_Terminate(void);
u32 OS_DisableIRQ(void);
u32 OS_RestoreIRQ(u32);
#else
#include <nitro/os.h>
#endif

/* volatile views of DISPCNT (reg.h declares it non-volatile) */
#define REG_DISPCNT_V     (*VIDEO_REG(vu32, 0x04000000))
#define REG_DISPCNT_SUB_V (*VIDEO_REG(vu32, 0x04001000))

/* gx.c .data: display-on flag and default DMA channel (-1 = CPU copy) */
extern u16 data_020c3db8;
extern u32 data_020c3dbc;

extern u16 data_0210ce58; /* saved display mode */
extern u16 data_0210ce5a; /* GX lock id */

/* data_0210ce5c (0x1a bytes): VRAM bank allocation state */
typedef struct {
    /* 0x00 */ u16 lcdc;
    /* 0x02 */ u16 bg;
    /* 0x04 */ u16 obj;
    /* 0x06 */ u16 arm7;
    /* 0x08 */ u16 tex;
    /* 0x0a */ u16 texPltt;
    /* 0x0c */ u16 clearImage;
    /* 0x0e */ u16 bgExtPltt;
    /* 0x10 */ u16 objExtPltt;
    /* 0x12 */ u16 subBg;
    /* 0x14 */ u16 subObj;
    /* 0x16 */ u16 subBgExtPltt;
    /* 0x18 */ u16 subObjExtPltt;
} UnkGxVramState;
extern UnkGxVramState data_0210ce5c;

void func_02063960(void);
void func_020670cc(u32 dmaNo, void *dest, u32 data, u32 size);
void func_0206785c(u32 data, void *dest, u32 size);
void func_02067384(u32 dmaNo);

#endif
