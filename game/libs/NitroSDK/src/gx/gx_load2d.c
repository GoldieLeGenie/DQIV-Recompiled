#include "gx_internal.h"

/* data_0210ce78 (0x1c bytes): extended palette load state */
typedef struct {
    /* 0x00 */ s32 subBgExtPltt;
    /* 0x04 */ u32 objExtPlttAddr;
    /* 0x08 */ s32 objExtPltt;
    /* 0x0c */ u32 bgExtPlttOffset;
    /* 0x10 */ u32 bgExtPlttAddr;
    /* 0x14 */ s32 bgExtPltt;
    /* 0x18 */ s32 subObjExtPltt;
} UnkGxExtPlttState;
UnkGxExtPlttState data_0210ce78;

void MI_CpuCopyU16(const void *src, void *dest, u32 size);
void MI_CpuCopyU32(const void *src, void *dest, u32 size);
void func_0206714c(u32 dmaNo, const void *src, void *dest, u32 size);
void func_020671bc(u32 dmaNo, const void *src, void *dest, u32 size);
void func_020672ec(u32 dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *), void *arg);

s32 GX_ResetBankForBgExtPltt(void);
s32 GX_ResetBankForOBJExtPltt(void);
s32 GX_ResetBankForSubBgExtPltt(void);
s32 GX_ResetBankForSubObjExtPltt(void);
void GX_SetBankForBgExtPltt(s32 bgExtPltt);
void GX_SetBankForObjExtPltt(s32 objExtPltt);
void GX_SetBankForSubBgExtPltt(s32 subBgExtPltt);
void GX_SetBankForSubObjExtPltt(s32 subObjExtPltt);

void *func_02064ad0(void);
void *func_02064b04(void);
void *func_02064b24(void);
void *func_02064b58(void);
void *func_02064b78(void);
void *func_02064bfc(void);
void *func_02064c70(void);
void *func_02064cf4(void);
void *func_02064d68(void);
void *func_02064d9c(void);
void *func_02064dbc(void);
void *func_02064df0(void);
void *func_02064e10(void);
void *func_02064e60(void);
void *func_02064ea0(void);
void *func_02064ef8(void);

/* Copy with DMA when a DMA channel is set and the size is large enough, else with the CPU */
static inline void UnkGxCopy16(u32 dmaNo, const void *src, void *dest, u32 size) {
#ifdef DQIV_NATIVE
    pcVideoCopy((u32)dest, src, size);
#else
    if (dmaNo != -1 && size > 0x1c) {
        func_020671bc(dmaNo, src, dest, size);
    } else {
        MI_CpuCopyU16(src, dest, size);
    }
#endif
}

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

static inline void *UnkGetObjCharPtr(void) {
    return (void *)0x6400000;
}

static inline void *UnkGetSubObjCharPtr(void) {
    return (void *)0x6600000;
}

void GX_LoadBGPltt(const void *src, u32 offset, u32 size) {
    UnkGxCopy16(data_020c3dbc, src, (void *)(0x5000000 + offset), size);
}

void GXS_LoadBGPltt(const void *src, u32 offset, u32 size) {
    UnkGxCopy16(data_020c3dbc, src, (void *)(0x5000400 + offset), size);
}

void GX_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    UnkGxCopy16(data_020c3dbc, src, (void *)(0x5000200 + offset), size);
}

void GXS_LoadOBJPltt(const void *src, u32 offset, u32 size) {
    UnkGxCopy16(data_020c3dbc, src, (void *)(0x5000600 + offset), size);
}

void GX_LoadOAM(const void *src, u32 offset, u32 size) {
    UnkGxCopy32(data_020c3dbc, src, (void *)(0x7000000 + offset), size);
}

void GXS_LoadOAM(const void *src, u32 offset, u32 size) {
    UnkGxCopy32(data_020c3dbc, src, (void *)(0x7000400 + offset), size);
}

/* OBJ character data */
void func_02065ee0(const void *src, u32 offset, u32 size) {
    u8 *base = (u8 *)UnkGetObjCharPtr();
    UnkGxCopy32(data_020c3dbc, src, base + offset, size);
}

void func_02065f38(const void *src, u32 offset, u32 size) {
    u8 *base = (u8 *)UnkGetSubObjCharPtr();
    UnkGxCopy32(data_020c3dbc, src, base + offset, size);
}

/* BG screen data */
void func_02065f90(const void *src, u32 offset, u32 size) {
    void *dest = func_02064ad0();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02065ff0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064b04();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066050(const void *src, u32 offset, u32 size) {
    void *dest = func_02064b24();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_020660b0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064b58();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066110(const void *src, u32 offset, u32 size) {
    void *dest = func_02064b78();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066170(const void *src, u32 offset, u32 size) {
    void *dest = func_02064bfc();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_020661d0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064c70();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066230(const void *src, u32 offset, u32 size) {
    void *dest = func_02064cf4();
    UnkGxCopy16(data_020c3dbc, src, (u8 *)dest + offset, size);
}

/* BG character data */
void func_02066290(const void *src, u32 offset, u32 size) {
    void *dest = func_02064d68();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_020662f0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064d9c();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066350(const void *src, u32 offset, u32 size) {
    void *dest = func_02064dbc();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_020663b0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064df0();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066410(const void *src, u32 offset, u32 size) {
    void *dest = func_02064e10();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066470(const void *src, u32 offset, u32 size) {
    void *dest = func_02064e60();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_020664d0(const void *src, u32 offset, u32 size) {
    void *dest = func_02064ea0();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void func_02066530(const void *src, u32 offset, u32 size) {
    void *dest = func_02064ef8();
    UnkGxCopy32(data_020c3dbc, src, (u8 *)dest + offset, size);
}

void GX_BeginLoadBGExtPltt(void) {
    data_0210ce78.bgExtPltt = GX_ResetBankForBgExtPltt();

    switch (data_0210ce78.bgExtPltt) {
    case 0x00:
        break;
    case 0x10:
        data_0210ce78.bgExtPlttAddr   = 0x6880000;
        data_0210ce78.bgExtPlttOffset = 0;
        break;
    case 0x40:
        data_0210ce78.bgExtPlttAddr   = 0x6894000;
        data_0210ce78.bgExtPlttOffset = 0x4000;
        break;
    case 0x20:
    case 0x60:
        data_0210ce78.bgExtPlttAddr   = 0x6890000;
        data_0210ce78.bgExtPlttOffset = 0;
        break;
    default:
        break;
    }
}

void GX_LoadBGExtPltt(const void *src, u32 destSlotAddr, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src,
                     (void *)(data_0210ce78.bgExtPlttAddr + destSlotAddr - data_0210ce78.bgExtPlttOffset), size);
}

void GX_EndLoadBGExtPltt(void) {
    if (data_020c3dbc != -1) {
        func_02067384(data_020c3dbc);
    }
    GX_SetBankForBgExtPltt(data_0210ce78.bgExtPltt);
    data_0210ce78.bgExtPltt       = 0;
    data_0210ce78.bgExtPlttAddr   = 0;
    data_0210ce78.bgExtPlttOffset = 0;
}

void GX_BeginLoadOBJExtPltt(void) {
    data_0210ce78.objExtPltt = GX_ResetBankForOBJExtPltt();

    switch (data_0210ce78.objExtPltt) {
    case 0x00:
        break;
    case 0x40:
        data_0210ce78.objExtPlttAddr = 0x6894000;
        break;
    case 0x20:
        data_0210ce78.objExtPlttAddr = 0x6890000;
        break;
    }
}

void GX_LoadOBJExtPltt(const void *src, u32 destSlotAddr, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)(data_0210ce78.objExtPlttAddr + destSlotAddr), size);
}

void GX_EndLoadOBJExtPltt(void) {
    if (data_020c3dbc != -1) {
        func_02067384(data_020c3dbc);
    }
    GX_SetBankForObjExtPltt(data_0210ce78.objExtPltt);
    data_0210ce78.objExtPltt     = 0;
    data_0210ce78.objExtPlttAddr = 0;
}

void GXS_BeginLoadBGExtPltt(void) {
    data_0210ce78.subBgExtPltt = GX_ResetBankForSubBgExtPltt();
}

void GXS_LoadBGExtPltt(const void *src, u32 destSlotAddr, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)(0x6898000 + destSlotAddr), size);
}

void GXS_EndLoadBGExtPltt(void) {
    if (data_020c3dbc != -1) {
        func_02067384(data_020c3dbc);
    }
    GX_SetBankForSubBgExtPltt(data_0210ce78.subBgExtPltt);
    data_0210ce78.subBgExtPltt = 0;
}

void GXS_BeginLoadOBJExtPltt(void) {
    data_0210ce78.subObjExtPltt = GX_ResetBankForSubObjExtPltt();
}

void GXS_LoadOBJExtPltt(const void *src, u32 destSlotAddr, u32 size) {
    UnkGxCopy32Async(data_020c3dbc, src, (void *)(0x68a0000 + destSlotAddr), size);
}

void GXS_EndLoadOBJExtPltt(void) {
    if (data_020c3dbc != -1) {
        func_02067384(data_020c3dbc);
    }
    GX_SetBankForSubObjExtPltt(data_0210ce78.subObjExtPltt);
    data_0210ce78.subObjExtPltt = 0;
}
